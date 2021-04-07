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

void new_wifi_ssid_success() {
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

void new_wifi_password_success() {
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
  const char* new_api_key = "koekoek?";
  const char* new_wifi_ssid = "newwifissid!";
  const char* new_wifi_password = "newwifipassworD";

  sprintf(
      buffer,
      "%s=%s&"
      "%s=%s&"
      "%s=%s&"
      "%s=%s&"
      "%s=%s&",
      FORM_NAME_WIFI_SSID, new_wifi_ssid,
      FORM_NAME_WIFI_PASS, new_wifi_password,
      FORM_NAME_API_KEY, new_api_key,
      FORM_NAME_AP_SSID, new_ap_ssid,
      FORM_NAME_AP_PASS, new_ap_password
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
  TEST_ASSERT_EQUAL_STRING(new_ap_ssid, config.get_ap_ssid());
  TEST_ASSERT_EQUAL_STRING(new_ap_password, config.get_ap_password());

  // cleanup
  ap_worker.set_active(false);
  server.set_active(false);
  vTaskDelete(xHandle);
}
