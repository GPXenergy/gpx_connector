#include "controller.h"
#include "identifiers.h"
#include "debugger.h"
#include "mode_switch.h"
#include "button.h"

Controller::Controller(LocalStorage& config) :
    Supervisor(),
    _config(config) {

}

void Controller::reset_system() {
  DEBUG_PRINTLN("\n RESETTING AND RESTARTING ESP...\n");
  _config.reset_defaults();
  ESP.restart();
}

void Controller::handle_report(const worker_map_t& workers, const handler_map_t& handlers) {
  const auto& resetButton = workers.worker<Button>(k_worker_reset_button);
  if(resetButton->is_fresh() && resetButton->get_data().state) {
    // Button was pressed
    if (resetButton->get_data().pressDuration > 5000) {
      reset_system();
    }
  }
  const auto& modeSwitch = workers.worker<ModeSwitch>(k_worker_mode_switch);
  if(modeSwitch->is_fresh() && modeSwitch->get_data() == e_mode_active) {
    // System set to active mode
    // Disable access point
    workers.at(k_worker_wifi_access_point)->set_active(false);
    // Enable meter readers and api connector
    workers.at(k_worker_meter_connector)->set_active(true);
    handlers.at(k_handler_api_reporter)->set_active(true);
    // Enable inverter connector if its enabled through config
    if(_config.get_inverter_enabled()) {
      workers.at(k_worker_inverter_connector)->set_active(true);
    }
  } else if(modeSwitch->is_fresh() && modeSwitch->get_data() == e_mode_config) {
    // System set to config mode
    // Disable meter readers and api connector
    workers.at(k_worker_meter_connector)->set_active(false);
    workers.at(k_worker_inverter_connector)->set_active(false);
    handlers.at(k_handler_api_reporter)->set_active(false);
    // Enable access point
    workers.at(k_worker_wifi_access_point)->set_active(true);
  }
}

