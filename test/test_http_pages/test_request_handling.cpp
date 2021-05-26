#include <Arduino.h>
#include <unity.h>
#include <HTTPClient.h>

#include <access_point.h>
#include <configuration_server.h>
#include <http_pages.h>

#include "../mock_local_storage.h"


// Used to run server in separate thread
void run_server(void* server_arg) {
  ConfigWebServer* server = (ConfigWebServer*) server_arg;
  while(server && server->active()) {
    server->work();
    delay(10);
  }
  vTaskDelete(nullptr);
}

void get_home_page_success() {
  /// Given
  MockLocalStorage config;
  StatusLed led;
  AccessPoint ap_worker(config);
  ConfigWebServer server(config, led);

  // initialize and activate the config, ap server and config server
  config.initialize();
  server.initialize();
  ap_worker.initialize();
  ap_worker.set_active(true);
  server.set_active(true);

  // Create thread to run the server in
  TaskHandle_t xHandle = nullptr;
  xTaskCreate(run_server, "WebServer", 50000, &server, 1, &xHandle);
  TEST_ASSERT_NOT_NULL(xHandle); // Make sure thread is created

  // Client to connect to the server
  HTTPClient client;

  /// When
  bool client_status = client.begin("http://127.0.0.1/");

  /// Then
  TEST_ASSERT_TRUE(client_status);
  TEST_ASSERT_EQUAL(200, client.GET());

  // cleanup
  ap_worker.set_active(false);
  server.set_active(false);
  vTaskDelete(xHandle);
}

void post_new_device_config_success() {
  /// Given
  MockLocalStorage config;
  StatusLed led;
  AccessPoint ap_worker(config);
  ConfigWebServer server(config, led);

  // initialize and activate the config, ap server and config server
  config.initialize();
  server.initialize();
  ap_worker.initialize();
  ap_worker.set_active(true);
  server.set_active(true);

  // Create thread to run the server in
  TaskHandle_t xHandle = nullptr;
  xTaskCreate(run_server, "WebServer", 50000, &server, 1, &xHandle);
  TEST_ASSERT_NOT_NULL(xHandle); // Make sure thread is created

  // Client to connect to the server
  HTTPClient client;
  char buffer[500];

  // Set post data
  const char* new_ap_ssid = "newapssid!!!";
  const char* new_ap_password = "newappassword!!!";
  const char* new_meter_baud = "9800";
  const MeterParity new_meter_parity = MeterParity::e_meter_parity_7E1;
  const char* new_inverter_enabled = "1";
  const char* new_inverter_current_sensor = "50";
  const char* new_phase_type = "3";

  sprintf(
      buffer,
      "%s=%s&"
      "%s=%s&"
      "%s=%s&"
      "%s=%d&"
      "%s=%s&"
      "%s=%s&"
      "%s=%s",
      FORM_NAME_AP_SSID, new_ap_ssid,
      FORM_NAME_AP_PASS, new_ap_password,
      FORM_NAME_METER_BAUD, new_meter_baud,
      FORM_NAME_METER_PARITY, new_meter_parity,
      FORM_NAME_INVERTER_ENABLED, new_inverter_enabled,
      FORM_NAME_CURRENT_SENSOR_AMPS, new_inverter_current_sensor,
      FORM_NAME_PHASE_TYPE, new_phase_type
  );

  /// When
  bool client_status = client.begin("http://127.0.0.1/save");
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int response_code = client.POST(reinterpret_cast<uint8_t*>(buffer), strlen(buffer));

  /// Then
  TEST_ASSERT_TRUE(client_status);
  TEST_ASSERT_EQUAL(302, response_code);

  // Check new settings
  TEST_ASSERT_EQUAL_STRING(new_ap_ssid, config.get_ap_ssid());
  TEST_ASSERT_EQUAL_STRING(new_ap_password, config.get_ap_password());
  TEST_ASSERT_EQUAL(9800, config.get_meter_baud());
  TEST_ASSERT_EQUAL(e_meter_parity_7E1, config.get_meter_parity());
  TEST_ASSERT_TRUE(config.get_inverter_enabled());
  TEST_ASSERT_EQUAL(50, config.get_current_sensor_amps());
  TEST_ASSERT_EQUAL(3, config.get_phase_type());

  // cleanup
  ap_worker.set_active(false);
  server.set_active(false);
  vTaskDelete(xHandle);
}

void post_new_connection_config_success() {
  /// Given
  MockLocalStorage config;
  StatusLed led;
  AccessPoint ap_worker(config);
  ConfigWebServer server(config, led);

  // initialize and activate the config, ap server and config server
  config.initialize();
  server.initialize();
  ap_worker.initialize();
  ap_worker.set_active(true);
  server.set_active(true);

  // Create thread to run the server in
  TaskHandle_t xHandle = nullptr;
  xTaskCreate(run_server, "WebServer", 50000, &server, 1, &xHandle);
  TEST_ASSERT_NOT_NULL(xHandle); // Make sure thread is created

  // Client to connect to the server
  HTTPClient client;
  char buffer[500];

  // Set post data
  const char* new_wifi_ssid = "newwifissid!";
  const char* new_wifi_password = "newwifipassworD";
  const char* new_api_key = "someAPIKEY321";
  const char* new_use_dev = "1";

  sprintf(
      buffer,
      "%s=%s&"
      "%s=%s&"
      "%s=%s&"
      "%s=%s",
      FORM_NAME_WIFI_SSID, new_wifi_ssid,
      FORM_NAME_WIFI_PASS, new_wifi_password,
      FORM_NAME_API_KEY, new_api_key,
      FORM_NAME_USE_DEV, new_use_dev
  );

  /// When
  bool client_status = client.begin("http://127.0.0.1/save");
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int response_code = client.POST(reinterpret_cast<uint8_t*>(buffer), strlen(buffer));

  /// Then
  TEST_ASSERT_TRUE(client_status);
  TEST_ASSERT_EQUAL(302, response_code);

  // Check new settings
  TEST_ASSERT_EQUAL_STRING(new_wifi_ssid, config.get_wifi_ssid());
  TEST_ASSERT_EQUAL_STRING(new_wifi_password, config.get_wifi_password());
  TEST_ASSERT_EQUAL_STRING(new_api_key, config.get_api_key());
  TEST_ASSERT_TRUE(config.get_use_dev());

  // cleanup
  ap_worker.set_active(false);
  server.set_active(false);
  vTaskDelete(xHandle);
}
