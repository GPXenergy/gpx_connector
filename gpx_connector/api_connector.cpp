
#include "api_connector.h"
#include "debugger.h"
#include "identifiers.h"
#include "smart_meter_connector.h"
#include "inverter_connector.h"

#define RETRY_TIMEOUT 7500  // Wait 7.5 seconds before retrying wifi connection
#define HANDLE_DELAY 10000  // Handle every 10 seconds

ApiConnector::ApiConnector(LocalStorage& config) :
    Handler(),
    _config(config),
    _last_handle(),
    _payload{""} {
}

bool ApiConnector::time_to_send() const {
  return millis() - _last_handle > HANDLE_DELAY;
}

bool ApiConnector::activate(bool retry) {
  static uint32_t last_try = 0;
  if(WiFiConnection::wifi_connected()) {
    return true;
  }
  if(retry && millis() - last_try < RETRY_TIMEOUT) {
    return false;
  }
  last_try = millis();
  WiFiConnection::connect_wifi(_config.get_wifi_ssid(), _config.get_wifi_password());

  return WiFi.isConnected();
}

void ApiConnector::deactivate() {
  WiFiConnection::disconnect_wifi();
}

int8_t ApiConnector::handle_produced_work(const worker_map_t& workers) {
  if(!WiFi.isConnected() && !activate(true)) {
    // Wifi is no longer connected and was not able to reconnect
    return e_api_reporter_error_not_connected;
  }
  const auto& meter_worker = (SmartMeterConnector*) workers.at(k_worker_meter_connector);
  const auto& inverter_worker = (InverterConnector*) workers.at(k_worker_inverter_connector);
  if(!inverter_worker || !meter_worker) {
    // no new meter data or not yet time to send (or one of the workers is actually missing!)
    DEBUG_PRINTLN("Something serious is going on, we're missing an important worker!");
    return e_api_reporter_idle;
  }
  if(!time_to_send()) {
    // no new meter data or not yet time to send (or one of the workers is actually missing!)
    DEBUG_PRINTLN("Not yet time to send...");
    return e_api_reporter_idle;
  }
  if(!meter_worker->is_fresh()) {
    // no new meter data or not yet time to send (or one of the workers is actually missing!)
    DEBUG_PRINTLN("No fresh data available");
    return e_api_reporter_idle;
  }
  const auto p1_data = &meter_worker->get_data();
  const auto inverter_data = inverter_worker->active() ? &inverter_worker->get_data() : nullptr;
  if(!p1_data || !p1_data->valid_reading()) {
    // Data is invalid
    return e_api_reporter_error_invalid_data;
  }
  // All good, start to handle the data
  _last_handle = millis();
  generate_payload(p1_data, inverter_data, _payload);
  return send_payload(_payload);
}

ApiConnector::ApiHandlerStatus ApiConnector::send_payload(const char* payload) {
  HTTPClient http;
  DEBUG_PRINTLN("Trying to send data");

  //Specify destination for HTTP request
  bool begin = _config.get_use_dev() ?
               http.begin(API_MEASUREMENTS_TEST_ENDPOINT) : http.begin(API_MEASUREMENTS_ENDPOINT);
  if(!begin) {
    DEBUG_PRINTLN("Unable to begin url connection");
    http.end();  //Free resources
    return e_api_reporter_error_remote_not_available;
  }

  char content_length[5];
  sprintf(content_length, "%d", strlen(payload));

  char auth[40];
  sprintf(auth, "Token %s", _config.get_api_key());

  http.addHeader("Host", API_HOST);
  http.addHeader("Content-Type", HEADER_API_CONTENT_TYPE);
  http.addHeader("User-Agent", HEADER_API_USER_AGENT);
  http.addHeader("Content-Length", content_length);
  http.addHeader("Authorization", auth);

  // Send the POST request
  int httpResponseCode = http.POST(payload);

  String response = http.getString();
  DEBUG_PRINTF("POST complete, status %d\r\nrepsonse: \r\n\r\n%s\r\n\r\n", httpResponseCode, response.c_str());
  http.end();  //Free resources

  switch(httpResponseCode) {
    case 201:
      return e_api_reporter_send_success;
    case 400:
      return e_api_reporter_error_server_rejected_post_400;
    case 401:
      return e_api_reporter_error_server_rejected_post_401;
    case 403:
      return e_api_reporter_error_server_rejected_post_403;
    case 500 ... 599:
      return e_api_reporter_error_server_rejected_post_5xx;
    default:
      return e_api_reporter_error_remote_not_available;
  }
}

uint16_t ApiConnector::generate_payload(const P1Data* p1_data, const InverterData* inverter_data, char* out) {
  // Write payload
  /// Format:  (the {...} part is written in by add_x_payload methods
  /// {
  ///   "power": {...},
  ///   "gas": {...},
  ///   "solar": {...},
  /// }
  sprintf(out, "{\"power\":");
  add_power_payload(p1_data, &out[strlen(out)]);
  sprintf(&out[strlen(out)], ",\"gas\":");
  add_gas_payload(p1_data, &out[strlen(out)]);
  sprintf(&out[strlen(out)], ",\"solar\":");
  add_solar_payload(inverter_data, p1_data ? p1_data->get_power_timestamp() : nullptr, &out[strlen(out)]);
  sprintf(&out[strlen(out)], "}");
  return strlen(out);
}

uint16_t ApiConnector::add_power_payload(const P1Data* data, char* out) {
  /// Power json format
  /// {
  ///    "sn": value,
  ///    "timestamp": value,
  ///    "import_1": value,
  ///    "import_2": value,
  ///    "export_1": value,
  ///    "export_2": value,
  ///    "tariff": value,
  ///    "actual_import": value,
  ///    "actual_export": value
  ///  }
  if(data == nullptr) {
    return sprintf(out, "null");
  }
  return sprintf(
      out,
      "{"
      "\"sn\":\"%s\","
      "\"timestamp\":\"%s\","
      "\"import_1\":\"%s\","
      "\"import_2\":\"%s\","
      "\"export_1\":\"%s\","
      "\"export_2\":\"%s\","
      "\"tariff\":%d,"
      "\"actual_import\":\"%s\","
      "\"actual_export\":\"%s\""
      "}",
      data->get_sn_power(),
      data->get_power_timestamp(),
      data->get_import_1(),
      data->get_import_2(),
      data->get_export_1(),
      data->get_export_2(),
      data->get_tariff(),
      data->get_actual_import(),
      data->get_actual_export()
  );
}

uint16_t ApiConnector::add_gas_payload(const P1Data* data, char* out) {
  /// Gas json format (or null if gas is not available)
  /// {
  ///    "sn": value,
  ///    "timestamp": value,
  ///    "gas": value
  ///  }
  if(data == nullptr || strlen(data->get_sn_gas()) == 0) {
    // No gas values
    return sprintf(out, "null");
  }
  return sprintf(
      out,
      "{"
      "\"sn\":\"%s\","
      "\"timestamp\":\"%s\","
      "\"gas\":\"%s\""
      "}",
      data->get_sn_gas(),
      data->get_gas_timestamp(),
      data->get_gas_import()
  );
}

uint16_t ApiConnector::add_solar_payload(const InverterData* data, const char* timestamp, char* out) {
  /// Solar json format (or null if solar is not available)
  /// {
  ///    "timestamp": value,
  ///    "solar": value
  ///  }
  if(data == nullptr || !data->is_valid()) {
    // No solar values
    return sprintf(out, "null");
  }
  return sprintf(
      out,
      "{"
      "\"timestamp\":\"%s\","
      "\"solar\":\"%.3f\""
      "}",
      timestamp,
      data->get_solar()
  );
}
