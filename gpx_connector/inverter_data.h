#ifndef GPXCONNECTOR_INVERTER_DATA_H
#define GPXCONNECTOR_INVERTER_DATA_H

#include <Arduino.h>

#define SOLAR_LIST_SIZE 10

/**
 * Container for a reading from the inverter
 */
class InverterData {
 public:
  /**
   * Create a new empty data container
   */
  InverterData();

  virtual ~InverterData() = default;
  InverterData(const InverterData& copy) = delete;
  InverterData& operator=(const InverterData& other) = delete;

  // Getters
  void set_solar(double solar);
  double get_solar() const;

  bool is_valid() const;

 private:

  // InverterData content
  double _solar[SOLAR_LIST_SIZE];
  uint8_t _index;
  uint8_t _size;
};

#endif //GPXCONNECTOR_READING_H
