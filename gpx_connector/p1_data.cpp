#include <string.h>
#include <stdio.h>
#include <math.h>
#include <HardwareSerial.h>

#include "p1_data.h"
#include "debugger.h"

P1Data::P1Data() :
    _status(k_p1_empty),
    _running_crc(0),
    _expectingGas22(false),
    _version(0),
    _power_timestamp{""},
    _serial_number_power{""},
    _import_tariff_1{""},
    _import_tariff_2{""},
    _export_tariff_1{""},
    _export_tariff_2{""},
    _tariff(0),
    _actual_import{""},
    _actual_export{""},
    _serial_number_gas{""},
    _gas_timestamp{""},
    _gas_import{""} {
}

P1Data::P1Data(const P1Data& copy) :
    _status(copy._status),
    _running_crc(copy._running_crc),
    _expectingGas22(copy._expectingGas22),
    _version(copy._version),
    _tariff(copy._tariff) {
  strcpy(_power_timestamp, copy._power_timestamp);
  strcpy(_serial_number_power, copy._serial_number_power);
  strcpy(_import_tariff_1, copy._import_tariff_1);
  strcpy(_import_tariff_2, copy._import_tariff_2);
  strcpy(_export_tariff_1, copy._export_tariff_1);
  strcpy(_export_tariff_2, copy._export_tariff_2);;
  strcpy(_actual_import, copy._actual_import);
  strcpy(_actual_export, copy._actual_export);
  strcpy(_serial_number_gas, copy._serial_number_gas);
  strcpy(_gas_timestamp, copy._gas_timestamp);
  strcpy(_gas_import, copy._gas_import);
}

P1Data& P1Data::operator=(const P1Data& other) {
  if(&other != this) {
    _status = other._status;
    _running_crc = other._running_crc;
    _expectingGas22 = other._expectingGas22;
    _version = other._version;
    strcpy(_power_timestamp, other._power_timestamp);
    strcpy(_serial_number_power, other._serial_number_power);
    strcpy(_import_tariff_1, other._import_tariff_1);
    strcpy(_import_tariff_2, other._import_tariff_2);
    strcpy(_export_tariff_1, other._export_tariff_1);
    strcpy(_export_tariff_2, other._export_tariff_2);
    _tariff = other._tariff;
    strcpy(_actual_import, other._actual_import);
    strcpy(_actual_export, other._actual_export);
    strcpy(_serial_number_gas, other._serial_number_gas);
    strcpy(_gas_timestamp, other._gas_timestamp);
    strcpy(_gas_import, other._gas_import);
  }
  return *this;
}

void P1Data::reset() {
  _running_crc = 0;
  _expectingGas22 = false;
  _status = k_p1_empty;
  _version = 0;
  _power_timestamp[0] = '\0';
  _serial_number_power[0] = '\0';
  _import_tariff_1[0] = '\0';
  _import_tariff_2[0] = '\0';
  _export_tariff_1[0] = '\0';
  _export_tariff_2[0] = '\0';
  _tariff = 0;
  _actual_import[0] = '\0';
  _actual_export[0] = '\0';
  _serial_number_gas[0] = '\0';
  _gas_import[0] = '\0';
}

void P1Data::add_line(const String& line) {
  if(is_complete()) {
    // Added a new line when the data was already complete, resetting all
    reset();
  }
  if(_status == k_p1_empty) {
    // p1 data is waiting for the header line to initiate the measurement
    add_header_line(line);
  } else {
    // p1 data is gathering data
    add_data_line(line);
  }
}

bool P1Data::valid_reading() const {
  return _status == k_p1_valid;
}

bool P1Data::is_complete() const {
  return _status == k_p1_valid || _status == k_p1_invalid;
}

P1Data::P1DataStatus P1Data::get_status() const {
  return _status;
}

uint8_t P1Data::get_version() const {
  return _version;
}

const char* P1Data::get_power_timestamp() const {
  return _power_timestamp;
}

const char* P1Data::get_sn_power() const {
  return _serial_number_power;
}

const char* P1Data::get_import_1() const {
  return _import_tariff_1;
}

const char* P1Data::get_import_2() const {
  return _import_tariff_2;
}

const char* P1Data::get_export_1() const {
  return _export_tariff_1;
}

const char* P1Data::get_export_2() const {
  return _export_tariff_2;
}

uint8_t P1Data::get_tariff() const {
  return _tariff;
}

const char* P1Data::get_actual_import() const {
  return _actual_import;
}

const char* P1Data::get_actual_export() const {
  return _actual_export;
}

const char* P1Data::get_sn_gas() const {
  return _serial_number_gas;
}

const char* P1Data::get_gas_timestamp() const {
  return _gas_timestamp;
}

const char* P1Data::get_gas_import() const {
  return _gas_import;
}

void P1Data::add_header_line(const String& header_line) {
  if(header_line.startsWith(F("/"))) {
    crc_16(header_line);
    Serial.println("HEADER");
    _status = k_p1_gathering;
    return;
  }
}

void P1Data::add_data_line(const String& data_line) {
  if(data_line.startsWith(F("!"))) {
    if (data_line.length() > 3) {
      // End, add `!` to crc calc and compare
      crc_16("!");

      uint32_t message_crc;
      sscanf(data_line.c_str(), "!%x", &message_crc);

      // Set status based on crc
      _status = message_crc == _running_crc ? k_p1_valid : k_p1_invalid;
      Serial.printf("MESSAGE %s CRC %d, RUNNING %d  - status %d\r\n", data_line.c_str(), message_crc, _running_crc, _status);
    } else {
      // No CRC to check
      _status = k_p1_valid;
      Serial.printf("NO MESSAGE CRC status %d\r\n", _status);

      if(strlen(_power_timestamp) == 0) {
        // this was a 2.2 message, which doesnt have power timestamps. set "now" for api to handle
        strcpy(_power_timestamp, "now");
      }
    }
    return;
  }

  // Add line to crc calc
  crc_16(data_line);

  if(data_line.startsWith(F("1-3:0.2.8"))) {
    // Version - 1-3:0.2.8(42)
    sscanf(data_line.c_str(), "1-3:0.2.8(%hu)", &_version);
  } else if(data_line.startsWith(F("0-0:1.0.0"))) {
    // Moment - 0-0:1.0.0(170108161107W)
    sscanf(data_line.c_str(), "0-0:1.0.0(%15[^)])", _power_timestamp);
  } else if(data_line.startsWith(F("0-0:96.1.1"))) {
    // Serial number power - 0-0:96.1.1(4530303331303033303031363939353135)
    sscanf(data_line.c_str(), "0-0:96.1.1(%39[^)])", _serial_number_power);
  } else if(data_line.startsWith(F("1-0:1.8.1"))) {
    // Import tariff 1 - 1-0:1.8.1(002074.842*kWh)
    sscanf(data_line.c_str(), "1-0:1.8.1(%10[0-9.])", _import_tariff_1);
  } else if(data_line.startsWith(F("1-0:1.8.2"))) {
    // Import tariff 2 - 1-0:1.8.2(000881.383*kWh)
    sscanf(data_line.c_str(), "1-0:1.8.2(%10[0-9.])", _import_tariff_2);
  } else if(data_line.startsWith(F("1-0:2.8.1"))) {
    // Export tariff 1 - 1-0:2.8.1(000010.981*kWh)
    sscanf(data_line.c_str(), "1-0:2.8.1(%10[0-9.])", _export_tariff_1);
  } else if(data_line.startsWith(F("1-0:2.8.2"))) {
    // Export tariff 2 - 1-0:2.8.2(000028.031*kWh)
    sscanf(data_line.c_str(), "1-0:2.8.2(%10[0-9.])", _export_tariff_2);
  } else if(data_line.startsWith(F("0-0:96.14.0"))) {
    // Tariff - 0-0:96.14.0(0001)
    sscanf(data_line.c_str(), "0-0:96.14.0(%hu)", &_tariff);
  } else if(data_line.startsWith(F("1-0:1.7.0"))) {
    // Actual import - 1-0:1.7.0(00.494*kW)
    sscanf(data_line.c_str(), "1-0:1.7.0(%10[0-9.])", _actual_import);
  } else if(data_line.startsWith(F("1-0:2.7.0"))) {
    // Actual export - 1-0:2.7.0(00.000*kW)
    sscanf(data_line.c_str(), "1-0:2.7.0(%10[0-9.])", _actual_export);
  } else if(data_line.startsWith(F("0-1:96.1.0"))) {
    // Serial number gas - 0-1:96.1.0(4730303139333430323231313938343135)
    sscanf(data_line.c_str(), "0-1:96.1.0(%39[^)])", _serial_number_gas);
  } else if(data_line.startsWith(F("0-1:24.2.1"))) {
    // Gas timestamp / gas import - 0-1:24.2.1(170108160000W)(01234.000*m3)
    sscanf(data_line.c_str(), "0-1:24.2.1(%15[^)])(%10[0-9.])", _gas_timestamp, _gas_import);
  } else if(data_line.startsWith(F("0-1:24.3.0"))) {
    // Gas timestamp / gas import - 0-1:24.3.0(210413190000)(00)(60)(1)(0-1:24.2.1)(m3)
    sscanf(data_line.c_str(), "0-1:24.3.0(%15[^)])", _gas_timestamp);
    _expectingGas22 = true;
  } else if(data_line.startsWith(F("(")) && _expectingGas22) {
    // Gas timestamp / gas import - 0-1:24.3.0(210413190000)(00)(60)(1)(0-1:24.2.1)(m3)
    sscanf(data_line.c_str(), "(%10[0-9.])", _gas_import);
    _expectingGas22 = false;
  }
}

void P1Data::crc_16(const String& buff) {
  // From http://domoticx.com/p1-poort-slimme-meter-uitlezen-hardware/#crc-berekenen
  for(int pos = 0; pos < buff.length(); pos++) {
    _running_crc ^= (unsigned int) buff.charAt(pos);  // XOR byte into least sig. byte of crc
    for(int i = 8; i != 0; i--) {             // Loop over each bit
      if((_running_crc & 0x0001u) != 0) {     // If the LSB is set
        _running_crc >>= 1u;                  // Shift right and XOR 0xA001
        _running_crc ^= 0xA001u;
      } else {                                // Else LSB is not set
        _running_crc >>= 1u;                  // Just shift right
      }
    }
  }
}
