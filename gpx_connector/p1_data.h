#ifndef GPXCONNECTOR_READING_H
#define GPXCONNECTOR_READING_H

#include <Arduino.h>


/**
 * Container for a reading from the smart meter, with some extra functions
 */
class P1Data {
 public:
  /**
   * enum for state of the read data
   */
  enum P1DataStatus {
    k_p1_empty,  // no data in this object
    k_p1_gathering,  // gathering data, started but not yet finished
    k_p1_invalid,  // P1 data is not valid (checksum failed)
    k_p1_valid,  // P1 data is valid and ready to be used
  } ;

  /**
   * Create a new empty reading
   */
  P1Data();

  virtual ~P1Data() = default;

  /**
   * Copy constructor
   * @param copy
   */
  P1Data(const P1Data& copy);

  /**
   * Assignment operator
   * @param copy
   */
  P1Data& operator=(const P1Data& other);

  /**
   * Add a new line to the object
   */
  void add_line(const String& line);

  /**
   * Check if the reading is 100% valid (parsed and validated)
   * @return true if valid
   */
  bool valid_reading() const;

  /**
   * Check if the reading is complete
   * @return true if valid or invalid
   */
  bool is_complete() const;

  P1DataStatus get_status() const;

  /**
   * Clear this data object, called when adding a header line
   */
  void reset();

  /// Getters
  uint8_t get_version() const;
  const char* get_power_timestamp() const;
  const char* get_sn_power() const;
  const char* get_import_1() const;
  const char* get_import_2() const;
  const char* get_export_1() const;
  const char* get_export_2() const;
  uint8_t get_tariff() const;
  const char* get_actual_import() const;
  const char* get_actual_export() const;
  const char* get_sn_gas() const;
  const char* get_gas_timestamp() const;
  const char* get_gas_import() const;

 private:

  /**
   * Add data line to the crc calculation (stored in `_running_crc`).
   * @param buff
   */
  void crc_16(const String& buff);

  /**
   * Try to parse a new header line. If it is a header (starting with `/`), it will reset this reading and start
   * gathering the rest of the data
   * @param header_line: check
   */
  void add_header_line(const String& header_line);

  /**
   * Try to parse a new data line. If the line is a closing line (`!crc`), it finishes up the data object and validates
   * @param data_line: line to parse
   */
  void add_data_line(const String& data_line);

  P1DataStatus _status;
  uint16_t _running_crc;
  bool _expectingGas22;  // For DSMR2.2 gas value is on a separate line

  // P1Data content
  uint16_t _version;
  char _power_timestamp[16];
  char _serial_number_power[40];
  char _import_tariff_1[11];
  char _import_tariff_2[11];
  char _export_tariff_1[11];
  char _export_tariff_2[11];
  uint16_t _tariff;
  char _actual_import[11];
  char _actual_export[11];
  char _serial_number_gas[40];
  char _gas_timestamp[16];
  char _gas_import[11];
};

#endif //GPXCONNECTOR_READING_H
