#ifndef GPXCONNECTOR_HTTP_PAGES_H
#define GPXCONNECTOR_HTTP_PAGES_H

#define TRANMISSION_SIZE 4098

#define PURE_CSS_SIZE 3929
#define FAVICON_SIZE 696

// Device config form names
#define FORM_NAME_AP_SSID "d_as"
#define FORM_NAME_AP_PASS "d_ap"
#define FORM_NAME_METER_BAUD "d_mb"
#define FORM_NAME_METER_PARITY "d_mp"
#define FORM_NAME_INVERTER_ENABLED "d_ie"
#define FORM_NAME_CURRENT_SENSOR_AMPS "d_ic"
#define FORM_NAME_PHASE_TYPE "d_ip"

// Connection config form names
#define FORM_NAME_WIFI_SSID "c_ws"
#define FORM_NAME_WIFI_PASS "c_wp"
#define FORM_NAME_API_KEY "c_ak"
#define FORM_NAME_USE_DEV "c_ud"

class HttpPages {
 public:
  HttpPages() = delete;

  /**
   * Render the home page
   * @return complete page rendered
   */
  static const char* get_home_page();

  /**
   * Render the update firmware page
   * @return complete page rendered
   */
  static const char* get_update_page();

  /**
   * Render the configuration page for device settings
   * @param display_success
   * @param device_ssid
   * @param device_password
   * @param meter_conn_baud
   * @param meter_conn_parity
   * @param inverter_enabled
   * @param inverter_sensor_amps
   * @return complete page rendered
   */
  static const char* get_config_device_page(
      bool display_success,
      const char* device_ssid,
      const char* device_password,
      uint32_t meter_conn_baud,
      uint16_t meter_conn_parity,
      bool inverter_enabled,
      uint8_t current_sensor_amps,
      uint8_t phase_type
  );

  /**
   *
   * Render the configuration page for connection settings
   * @param display_success
   * @param wifi_ssid
   * @param wifi_password
   * @param api_key
   * @param use_dev
   * @return complete page rendered
   */
  static const char* get_config_connection_page(
      bool display_success,
      const char* wifi_ssid,
      const char* wifi_password,
      const char* api_key,
      bool use_dev
  );

  /**
   * Render status page from report
   * TODO: implement the report itself
   * @return complete page rendered
   */
  static const char* get_status_page();

  static const uint8_t pure_css[PURE_CSS_SIZE];
  static const uint8_t favicon[FAVICON_SIZE];
  static const char* pure_js;

  /// If set to true, the pages will render with remote resources, else local resources will be used
  static bool internet_access;

 private:

  static const char* render_full_page(const char* page_name, const char* content_format, ...);
  static char transmission_buffer[TRANMISSION_SIZE];

};

#endif //GPXCONNECTOR_HTTP_PAGES_H
