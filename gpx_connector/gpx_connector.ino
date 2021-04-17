#ifndef UNIT_TEST

#include <Arduino.h>
#include <Aggregator.hpp>

#include "api_connector.h"
#include "access_point.h"
#include "debugger.h"
#include "controller.h"
#include "smart_meter_connector.h"
#include "configuration_server.h"
#include "status_led.h"
#include "identifiers.h"
#include "mode_switch.h"
#include "inverter_connector.h"

HardwareSerial& SmartMeterSerialConnection = Serial2;

LocalStorage config;
Aggregator aggregator;

// Supervisors
StatusLed status_led;
Controller controller(config);

// Workers
SmartMeterConnector meter_connector(SmartMeterSerialConnection);
InverterConnector inverter_connector(config);
ConfigWebServer config_server(config, status_led);
AccessPoint access_point(config);
ModeSwitch mode_switch;

// Data handlers
ApiConnector api_reporter(config);


void status_led_loop(void* param) {
  for (;;) {
    status_led.loop();
    delay(50);
  }
}


void setup() {
  delay(2000);
  DEBUG_BEGIN(DEBUG_BAUD);

  /// Software configurations
  // init config
  config.initialize();

  /// Hardware configurations
  // Start serial connection to meter
  switch (config.get_meter_parity()) {
    case e_meter_parity_7E1:
      SmartMeterSerialConnection.begin(config.get_meter_baud(), SERIAL_7E1);
      break;
    case e_meter_parity_8N1:
      SmartMeterSerialConnection.begin(config.get_meter_baud(), SERIAL_8N1);
      break;
  }

  SmartMeterSerialConnection.setRxBufferSize(4096);

  // Setup aggregator, which will initialize the workers/handlers/supervisors
  aggregator.register_worker(k_worker_wifi_access_point, access_point);
  aggregator.register_worker(k_worker_meter_connector, meter_connector);
  aggregator.register_worker(k_worker_inverter_connector, inverter_connector);
  aggregator.register_worker(k_worker_wifi_config_server, config_server);
  aggregator.register_worker(k_worker_mode_switch, mode_switch);

  aggregator.register_handler(k_handler_api_reporter, api_reporter);

  aggregator.register_supervisor(status_led);
  aggregator.register_supervisor(controller);

  // Enable default workers and handlers
  aggregator.set_worker_active(k_worker_mode_switch, true);
  aggregator.set_worker_active(k_worker_wifi_config_server, true);

  xTaskCreate(status_led_loop, "led_loop", configMINIMAL_STACK_SIZE * 3, nullptr, 2, nullptr);

  DEBUG_PRINTLN("GPX-Connector initialized!");
}

void loop() {
  aggregator.run();
}


#endif
