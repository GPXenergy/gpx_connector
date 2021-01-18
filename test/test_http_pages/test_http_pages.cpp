#include <Arduino.h>
#include <unity.h>

void get_home_page_render_success();
void get_update_page_render_success();
void get_config_device_page_render_success();
void get_config_connection_page_render_success();
void get_status_page_render_success();
void get_pure_css();
void get_favicon();

void new_wifi_ssid_success();
void new_wifi_password_success();

void setup() {
  delay(2000);
  // Unit test start
  UNITY_BEGIN();

  RUN_TEST(get_home_page_render_success);
  RUN_TEST(get_update_page_render_success);
  RUN_TEST(get_config_device_page_render_success);
  RUN_TEST(get_config_connection_page_render_success);
  RUN_TEST(get_status_page_render_success);
  RUN_TEST(get_pure_css);
  RUN_TEST(get_favicon);

  RUN_TEST(new_wifi_ssid_success);
  RUN_TEST(new_wifi_password_success);

  // Unit test done
  UNITY_END();
}

void loop() {
}
