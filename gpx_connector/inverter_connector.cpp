#include <driver/adc.h>

#include "inverter_data.h"
#include "inverter_connector.h"
#include "debugger.h"

#define SENSOR_SAMPLE_SIZE 1480

#define HOME_VOLTAGE_1_PHASE_POWER 230.0
#define HOME_VOLTAGE_3_PHASE_POWER 690.0
#define AMPS_THRESHOLD 0.5

// Force EmonLib to use 10bit ADC resolution
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)

InverterConnector::InverterConnector(LocalStorage& config)
    : Worker<InverterData>(100), _energyMonitor(), _config(config), _amps(0) {
}

void InverterConnector::initialize() {
  // Setup ACD pin
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
}

bool InverterConnector::activate(bool retry) {
  if(!_config.get_inverter_enabled()) {
    return false;
  }
  _amps = static_cast<double>(_config.get_current_sensor_amps());
  // configure monitor library
  _energyMonitor.current(INVERTER_ADC_INPUT_PIN, _amps);
  return true;
}

int8_t InverterConnector::produce_data() {
  double amps = _energyMonitor.calcIrms(SENSOR_SAMPLE_SIZE); // Calculate Irms only
  amps = amps < AMPS_THRESHOLD ? 0 : amps;
  // Calc wattage based on voltage
  double kWatts = 0;
  switch(_config.get_phase_type()) {
    case 1:
      kWatts = amps * HOME_VOLTAGE_1_PHASE_POWER / 1000;
      break;
    case 3:
      // Using 690V for 3 phase power
      kWatts = amps * HOME_VOLTAGE_3_PHASE_POWER / 1000;
      break;
    default:
      // Invalid settings
      return Worker::e_worker_error;
  }

  data.set_solar(kWatts);

  return Worker::e_worker_data_read;
}
