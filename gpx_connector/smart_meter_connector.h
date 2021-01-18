#ifndef GPXCONNECTOR_METER_CONNECTOR_H
#define GPXCONNECTOR_METER_CONNECTOR_H

#include <Arduino.h>

#include <Worker.hpp>

#include "p1_data.h"
#include <StreamString.h>

/**
 * Connect the system to the smart meter to read meter data
 */
class SmartMeterConnector : public Worker<P1Data> {
 public:
  explicit SmartMeterConnector(Stream& _serial_connection);
  virtual ~SmartMeterConnector() = default;

  uint32_t get_last_read() const;

 private:
  /**
   * Reads the serial connection to the smart meter, returns current status of the worker,
   * @return idle if no data read / still gathering data
   *         data_read if data has been completed and validated
   *         error is data is not validated through CRC
   */
  int8_t produce_data() override;

  Stream& _meter_connection_stream;
  String _buffer;
  uint32_t _last_read;
};

#endif //GPXCONNECTOR_METER_CONNECTOR_H
