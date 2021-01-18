#include "mode_switch.h"
#include "user_config.h"
#include "debugger.h"

#define SWITCH_JITTER_PROTECTION_TIMEOUT 500

ModeSwitch::ModeSwitch()
    : Worker<Mode>(Mode::e_mode_undefined), _detected_mode(e_mode_undefined), _last_change_detected(0) {

}

void ModeSwitch::initialize() {
  // Set pin mode ofr the mode switch pin,  input pulldown
  pinMode(MODE_SWITCH_PIN, INPUT_PULLDOWN);
}

int8_t ModeSwitch::produce_data() {
  // Some protection when toggling the switch slowly, will cause jitter. If switch signal is changed and remains
  // in that state for 500 millis, we switch the state.  Also prevents the system from switching modes faster than
  // once every 500 millis. flipping the switch back and forth quickly (within 500 millis) will also result in no
  // mode change
  if(_detected_mode != data && millis() - _last_change_detected > SWITCH_JITTER_PROTECTION_TIMEOUT) {
    data = _detected_mode;
    DEBUG_PRINTF("val now %s\r\n", _detected_mode == e_mode_config ? "config" : "active");
    return BaseWorker::e_worker_data_read;
  }
  bool high = digitalRead(MODE_SWITCH_PIN) == HIGH;

  // Set the _read_mode to what it currently read, only if its different from what it has read before
  if(high && _detected_mode != e_mode_config) {
    // High for config mode,
    _detected_mode = e_mode_config;
    _last_change_detected = millis();
  } else if(!high && _detected_mode != e_mode_active) {
    // Low for active mode,
    _detected_mode = e_mode_active;
    _last_change_detected = millis();
  }
  return BaseWorker::e_worker_idle;
}
