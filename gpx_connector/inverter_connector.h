#ifndef GPXCONNECTOR_INVERTER_CONNECTOR_H
#define GPXCONNECTOR_INVERTER_CONNECTOR_H

#include <Arduino.h>
#include <EmonLib.h>

#include <Worker.hpp>

#include "p1_data.h"
#include "inverter_data.h"
#include "local_storage.h"
#include <StreamString.h>

/**
 * Connect the system to the solar inverter to read the actual current
 * (Current sensor implementation)
 */
class InverterConnector : public Worker<InverterData> {
 public:
  explicit InverterConnector(LocalStorage& config);
  virtual ~InverterConnector() = default;
  void initialize() override;

 protected:
  bool activate(bool retry) override;

 private:
  /**
   * Reads the analog connection to the inverter, returns current status of the worker,
   */
  int8_t produce_data() override;

  EnergyMonitor _energyMonitor;
  LocalStorage& _config;
  double _amps;
};

#endif //GPXCONNECTOR_INVERTER_CONNECTOR_H
