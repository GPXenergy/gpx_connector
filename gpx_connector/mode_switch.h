#ifndef GPX_CONNECTOR_GPX_CONNECTOR_MODE_SWITCH_H_
#define GPX_CONNECTOR_GPX_CONNECTOR_MODE_SWITCH_H_

#include <Worker.hpp>

/**
 * Mode enum
 */
enum Mode {
  e_mode_undefined,  // Initial value
  e_mode_config,
  e_mode_active,
};

/**
 * The mode switch, a toggle switch the user uses to change between:
 * - active mode (read meter and send to api)
 * - config mode (running an access point where the user can configure the device)
 */
class ModeSwitch : public Worker<Mode> {
 public:

  ModeSwitch();
  virtual ~ModeSwitch() = default;

  /**
   * Initializes the switch pin
   */
  void initialize() override;

  /**
   * Checks the mode switch pin input and makes sure that the current data is a stable mode
   * - pin low = active mode
   * - pin high = config mode
   * @return status of the work (idle when no change)
   */
  int8_t produce_data() override;

 private:
  Mode _detected_mode;  // the mode that is currently read from the pin
  uint32_t _last_change_detected;  // the time in millis the detected mode was last changed
};

#endif //GPX_CONNECTOR_GPX_CONNECTOR_MODE_SWITCH_H_
