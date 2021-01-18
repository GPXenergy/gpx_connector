#ifndef BGEIGIE_POINTCAST_TEST_CONFIG_H
#define BGEIGIE_POINTCAST_TEST_CONFIG_H

#include <string.h>

#include <local_storage.h>
#include <user_config.h>

/**
 * Test class to inject in various classes, mocks the local storage so does not change existing settings
 */
class MockLocalStorage : public LocalStorage {
 public:
  MockLocalStorage() = default;
  virtual ~MockLocalStorage() = default;

  void set_ap_ssid(const char* ap_ssid, bool force) override {
    if(force || (ap_ssid != nullptr && strlen(ap_ssid) < CONFIG_VAL_MAX)) {
      strcpy(_ap_ssid, ap_ssid);
    }
  }

  void set_ap_password(const char* ap_password, bool force) override {
    if(force || (ap_password != nullptr && strlen(ap_password) < CONFIG_VAL_MAX)) {
      strcpy(_ap_password, ap_password);
    }
  }

  void set_wifi_ssid(const char* wifi_ssid, bool force) override {
    if(force || (wifi_ssid != nullptr && strlen(wifi_ssid) < CONFIG_VAL_MAX)) {
      strcpy(_wifi_ssid, wifi_ssid);
    }
  }

  void set_wifi_password(const char* wifi_password, bool force) override {
    if(force || (wifi_password != nullptr && strlen(wifi_password) < CONFIG_VAL_MAX)) {
      strcpy(_wifi_password, wifi_password);
    }
  }

  void set_api_key(const char* api_key, bool force) override {
    if(force || (api_key != nullptr && strlen(api_key) < CONFIG_VAL_MAX)) {
      strcpy(_api_key, api_key);
    }
  }

  void set_meter_baud(uint32_t meter_baud, bool force) override {
    if(force || (_meter_baud != meter_baud)) {
      _meter_baud = meter_baud;
    }
  }

  void set_meter_parity(MeterParity meter_parity, bool force) override {
    if(force || (_meter_parity != meter_parity)) {
      _meter_parity = meter_parity;
    }
  }

  void set_inverter_enabled(bool inverter_enabled, bool force) override {
    if(force || (_inverter_enabled != inverter_enabled)) {
      _inverter_enabled = inverter_enabled;
    }
  }

  void set_inverter_sensor_amps(uint8_t inverter_sensor_amps, bool force) override {
    if(force || (_inverter_sensor_amps != inverter_sensor_amps)) {
      _inverter_sensor_amps = inverter_sensor_amps;
    }
  }

  void set_use_dev(bool use_dev, bool force) override {
    if(force || (use_dev != _use_dev)) {
      _use_dev = use_dev;
    }
  }

  bool clear() override {
    return true;
  }

  bool initialize() override {
    strcpy(_ap_ssid, D_ACCESS_POINT_SSID);
    strcpy(_ap_password, D_ACCESS_POINT_PASSWORD);
    strcpy(_wifi_ssid, D_WIFI_SSID);
    strcpy(_wifi_password, D_WIFI_PASSWORD);
    strcpy(_api_key, D_APIKEY);
    _meter_baud = D_METER_BAUD;
    _meter_parity = D_METER_PARITY;
    _inverter_enabled = D_INVERTER_ENABLED;
    _inverter_sensor_amps = D_INVERTER_SENSOR_AMPS;
    _use_dev = D_USE_DEV_SERVER;
    return true;
  }
};

#endif //BGEIGIE_POINTCAST_TEST_CONFIG_H
