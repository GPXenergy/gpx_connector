#include <unity.h>
#include <http_pages.h>
#include <debugger.h>

void get_home_page_render_success() {
  TEST_ASSERT_NOT_NULL(HttpPages::get_home_page(
  ));
}

void get_update_page_render_success() {
  TEST_ASSERT_NOT_NULL(HttpPages::get_update_page(
  ));
}

void get_config_device_page_render_success() {
  TEST_ASSERT_NOT_NULL(HttpPages::get_config_device_page(
      false,
      "some ap ssid",
      "some ap password",
      115200,
      e_meter_parity_8N1,
      true,
      50
  ));
  TEST_ASSERT_NOT_NULL(HttpPages::get_config_device_page(
      true,
      "",
      "",
      9800,
      e_meter_parity_7E1,
      true,
      50
  ));
}

void get_config_connection_page_render_success() {
  TEST_ASSERT_NOT_NULL(HttpPages::get_config_connection_page(
      false,
      "some wifi ssid",
      "some wifi password",
      "some api key",
      false
  ));
  TEST_ASSERT_NOT_NULL(HttpPages::get_config_connection_page(
      true,
      "new wifi ssid",
      "new wifi password",
      "new pai key",
      true
  ));
}

void get_status_page_render_success() {
  TEST_ASSERT_NOT_NULL(HttpPages::get_status_page());
}

void get_pure_css() {
  TEST_ASSERT_NOT_NULL(HttpPages::pure_css);
}

void get_favicon() {
  TEST_ASSERT_NOT_NULL(HttpPages::favicon);
}
