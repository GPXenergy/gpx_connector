#ifndef GPXCONNECTOR_ESP_CONFIG_H
#define GPXCONNECTOR_ESP_CONFIG_H

#include <Preferences.h>

#include <Handler.hpp>
#include <user_config.h>

#define CONFIG_VAL_MAX 32


/**
 * Configurations for the ESP32, stored in the flash memory
 */
class LocalStorage {
 public:

  /**
   * Construct local storage object
   */
  LocalStorage();
  virtual ~LocalStorage() = default;

  /**
   * Reset settings to default (defined in user_config.h)
   */
  void reset_defaults();

  /**
   * Read all settings
   */
  virtual bool initialize();

  // Getters and setters
  virtual const char* get_ap_ssid() const final;
  virtual const char* get_ap_password() const final;
  virtual const char* get_wifi_ssid() const final;
  virtual const char* get_wifi_password() const final;
  virtual const char* get_api_key() const final;
  virtual uint32_t get_meter_baud() const final;
  virtual MeterParity get_meter_parity() const final;
  virtual bool get_inverter_enabled() const final;
  virtual uint8_t get_inverter_sensor_amps() const final;
  virtual bool get_use_dev() const final;


  virtual void set_ap_ssid(const char* ap_ssid, bool force);
  virtual void set_ap_password(const char* ap_password, bool force);
  virtual void set_wifi_ssid(const char* wifi_ssid, bool force);
  virtual void set_wifi_password(const char* wifi_password, bool force);
  virtual void set_api_key(const char* api_key, bool force);
  virtual void set_meter_baud(uint32_t meter_baud, bool force);
  virtual void set_meter_parity(MeterParity meter_parity, bool force);
  virtual void set_inverter_enabled(bool inverter_enabled, bool force);
  virtual void set_inverter_sensor_amps(uint8_t inverter_sensor_amps, bool force);
  virtual void set_use_dev(bool use_dev, bool force);

 protected:
  virtual bool clear();

  // Access point config (for config server)
  char _ap_ssid[CONFIG_VAL_MAX];
  char _ap_password[CONFIG_VAL_MAX];

  // Wifi config (to connect to the internet)
  char _wifi_ssid[CONFIG_VAL_MAX];
  char _wifi_password[CONFIG_VAL_MAX];

  // API config (to connect to the API)
  char _api_key[CONFIG_VAL_MAX];

  // Device config, meter
  uint32_t _meter_baud;
  MeterParity _meter_parity;

  // Device config, inverter
  bool _inverter_enabled;
  uint8_t _inverter_sensor_amps;

  // Other config
  bool _use_dev;

 private:
  Preferences _memory;
};

#endif //GPXCONNECTOR_ESP_CONFIG_H
