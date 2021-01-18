
#include "local_storage.h"
#include "debugger.h"
#include "p1_data.h"

const char* memory_name = "data";

// Keys for config
const char* key_ap_ssid = "ap_ssid";
const char* key_ap_password = "ap_password";
const char* key_wifi_ssid = "wifi_ssid";
const char* key_wifi_password = "wifi_password";
const char* key_api_key = "api_key";
const char* key_meter_baud = "meter_baud";
const char* key_meter_parity = "meter_parity";
const char* key_inverter_enabled = "inv_enabled";
const char* key_inverter_sensor_amps = "inv_amps";
const char* key_use_dev = "use_dev";

LocalStorage::LocalStorage() :
    _ap_ssid(""),
    _ap_password(""),
    _wifi_ssid(""),
    _wifi_password(""),
    _api_key(""),
    _meter_baud(D_METER_BAUD),
    _meter_parity(D_METER_PARITY),
    _inverter_enabled(D_INVERTER_ENABLED),
    _inverter_sensor_amps(D_INVERTER_SENSOR_AMPS),
    _use_dev(D_USE_DEV_SERVER),
    _memory() {
}

void LocalStorage::reset_defaults() {
  if(clear()) {
    set_ap_ssid(D_ACCESS_POINT_SSID, true);
    set_ap_password(D_ACCESS_POINT_PASSWORD, true);
    set_wifi_ssid(D_WIFI_SSID, true);
    set_wifi_password(D_WIFI_PASSWORD, true);
    set_api_key(D_APIKEY, true);
    set_meter_baud(D_METER_BAUD, true);
    set_meter_parity(D_METER_PARITY, true);
    set_inverter_enabled(D_INVERTER_ENABLED, true);
    set_inverter_sensor_amps(D_INVERTER_SENSOR_AMPS, true);
    set_use_dev(D_USE_DEV_SERVER, true);
  }
}

const char* LocalStorage::get_ap_ssid() const {
  return _ap_ssid;
}

const char* LocalStorage::get_ap_password() const {
  return _ap_password;
}

const char* LocalStorage::get_wifi_ssid() const {
  return _wifi_ssid;
}

const char* LocalStorage::get_wifi_password() const {
  return _wifi_password;
}

const char* LocalStorage::get_api_key() const {
  return _api_key;
}

uint32_t LocalStorage::get_meter_baud() const {
  return _meter_baud;
}

MeterParity LocalStorage::get_meter_parity() const {
  return _meter_parity;
}

bool LocalStorage::get_inverter_enabled() const {
  return _inverter_enabled;
}

uint8_t LocalStorage::get_inverter_sensor_amps() const {
  return _inverter_sensor_amps;
}

bool LocalStorage::get_use_dev() const {
  return _use_dev;
}

void LocalStorage::set_ap_ssid(const char* ap_ssid, bool force) {
  if(force || (ap_ssid != nullptr && strlen(ap_ssid) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_ap_ssid, ap_ssid);
      _memory.putString(key_ap_ssid, _ap_ssid);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_ssid");
    }
  }
}

void LocalStorage::set_ap_password(const char* ap_password, bool force) {
  if(force || (ap_password != nullptr && strlen(ap_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_ap_password, ap_password);
      _memory.putString(key_ap_password, _ap_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_password");
    }
  }
}

void LocalStorage::set_wifi_ssid(const char* wifi_ssid, bool force) {
  if(force || (wifi_ssid != nullptr && strlen(wifi_ssid) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_ssid, wifi_ssid);
      _memory.putString(key_wifi_ssid, _wifi_ssid);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_ssid");
    }
  }
}

void LocalStorage::set_wifi_password(const char* wifi_password, bool force) {
  if(force || (wifi_password != nullptr && strlen(wifi_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_password, wifi_password);
      _memory.putString(key_wifi_password, _wifi_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_password");
    }
  }
}

void LocalStorage::set_api_key(const char* api_key, bool force) {
  if(force || (api_key != nullptr && strlen(api_key) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_api_key, api_key);
      _memory.putString(key_api_key, _api_key);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for api_key");
    }
  }
}

void LocalStorage::set_meter_baud(uint32_t meter_baud, bool force) {
  if(force || (meter_baud != _meter_baud)) {
    if(_memory.begin(memory_name)) {
      _meter_baud = meter_baud;
      _memory.putULong(key_meter_baud, _meter_baud);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_ssid");
    }
  }
}

void LocalStorage::set_meter_parity(MeterParity meter_parity, bool force) {
  if(force || meter_parity != _meter_parity) {
    if(_memory.begin(memory_name)) {
      _meter_parity = meter_parity;
      _memory.putUShort(key_meter_parity, _meter_parity);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_password");
    }
  }
}

void LocalStorage::set_inverter_enabled(bool inverter_enabled, bool force) {
  if(force || (inverter_enabled != _inverter_enabled)) {
    if(_memory.begin(memory_name)) {
      _inverter_enabled = inverter_enabled;
      _memory.putBool(key_inverter_enabled, _inverter_enabled);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for inverter_enabled");
    }
  }
}

void LocalStorage::set_inverter_sensor_amps(uint8_t inverter_sensor_amps, bool force) {
  if(force || (inverter_sensor_amps != _inverter_sensor_amps)) {
    if(_memory.begin(memory_name)) {
      _inverter_sensor_amps = inverter_sensor_amps;
      _memory.putUChar(key_inverter_sensor_amps, _inverter_sensor_amps);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for inverter_sensor_amps");
    }
  }
}

void LocalStorage::set_use_dev(bool use_dev, bool force) {
  if(force || (use_dev != _use_dev)) {
    if(_memory.begin(memory_name)) {
      _use_dev = use_dev;
      _memory.putBool(key_use_dev, _use_dev);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for use_dev");
    }
  }
}

bool LocalStorage::clear() {
  if(_memory.begin(memory_name)) {
    _memory.clear();
    _memory.end();
    return true;
  }
  return false;
}

bool LocalStorage::initialize() {
  _memory.begin(memory_name, true);
  if(_memory.getString(key_ap_ssid, _ap_ssid, CONFIG_VAL_MAX) == 0) {
    // No ap_ssid in memory, using default
    strcpy(_ap_ssid, D_ACCESS_POINT_SSID);
  }
  if(_memory.getString(key_ap_password, _ap_password, CONFIG_VAL_MAX) == 0) {
    // No ap_password in memory, using default
    strcpy(_ap_password, D_ACCESS_POINT_PASSWORD);
  }
  if(_memory.getString(key_wifi_ssid, _wifi_ssid, CONFIG_VAL_MAX) == 0) {
    // No wifi_ssid in memory, using default
    strcpy(_wifi_ssid, D_WIFI_SSID);
  }
  if(_memory.getString(key_wifi_password, _wifi_password, CONFIG_VAL_MAX) == 0) {
    // No wifi_password in memory, using default
    strcpy(_wifi_password, D_WIFI_PASSWORD);
  }
  if(_memory.getString(key_api_key, _api_key, CONFIG_VAL_MAX) == 0) {
    // No api_key in memory, using default
    strcpy(_api_key, D_APIKEY);
  }
  _meter_baud = _memory.getULong(key_meter_baud, D_METER_BAUD);
  _meter_parity = static_cast<MeterParity>(_memory.getUShort(key_meter_parity, D_METER_PARITY));
  _inverter_enabled = _memory.getBool(key_inverter_enabled, D_INVERTER_ENABLED);
  _inverter_sensor_amps = _memory.getUChar(key_inverter_sensor_amps, D_INVERTER_SENSOR_AMPS);
  _use_dev = _memory.getBool(key_use_dev, D_USE_DEV_SERVER);
  _memory.end();
  return true;
}
