#include <driver/adc.h>

#include "inverter_data.h"
#include "inverter_connector.h"
#include "debugger.h"

#define SENSOR_SAMPLE_SIZE 1480
// (50 amps / 1 volt)
#define MAX_SENSOR_CURRENT 50.0
// Everything lower than the threshold is zeroed, 100th of max
#define AMPS_THRESHOLD MAX_SENSOR_CURRENT / 100

// 400 * sqrt(3) ,  for three-phase current in the inverter ( 400 line current )
#define HOME_VOLTAGE 692.820323028

// Force EmonLib to use 10bit ADC resolution
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)

InverterConnector::InverterConnector(LocalStorage& config)
    : Worker<InverterData>(100), energyMonitor(), config(config) {
}

void InverterConnector::initialize() {
  // Setup ACD pin
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
}

bool InverterConnector::activate(bool retry) {
  if(!config.get_inverter_enabled()) {
    return false;
  }
  // configure monitor library
  energyMonitor.current(INVERTER_ADC_INPUT, static_cast<double>(config.get_inverter_sensor_amps()));
  return true;
}

int8_t InverterConnector::produce_data() {
  double amps = energyMonitor.calcIrms(SENSOR_SAMPLE_SIZE); // Calculate Irms only
  amps = amps < AMPS_THRESHOLD ? 0 : amps;
  // Calc wattage based on voltage
  double kWatts = amps * HOME_VOLTAGE / 1000;
  data.set_solar(kWatts);

  return Worker::e_worker_data_read;
}
