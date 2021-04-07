#ifndef GPXCONNECTOR_STATE_LED_H
#define GPXCONNECTOR_STATE_LED_H

#include "rgb_led.h"
#include "local_storage.h"

#include <Supervisor.hpp>

/**
 * Controls the LED to visualize the current mode and status of the system
 */
class StatusLed : private RGBLed, public Supervisor {
 public:
  typedef enum {
    mode_color_off,
    mode_color_config,
    mode_color_config_updating,
    mode_color_active_idle,
    mode_color_active_connected,
    mode_color_active_disconnected,
    mode_color_active_data_error,
    mode_color_active_remote_error,
    mode_color_COUNT // items in this enum
  } ModeColor;

  explicit StatusLed();
  virtual ~StatusLed() = default;

  /**
   * Set color
   * @param color : color type (ModeColor)
   * @param frequency : times per second to update if blink (0 for not blinking)
   *    - example, frequency of 1 will blink once a second
   * @param percentage_on : if frequency > 0, it will
   *    - example, percentage of 25 with 1 frequency, will 0.25 second LED on then 0.75 second LED off
   */
  void set_values(ModeColor color, double frequency = 0, uint8_t percentage_on = 50);

  void set_ota_updating();

  /**
   * Will handle blinking led
   */
  void loop();

  /**
   * Handles report as a supervisor, setting the LED according to the status of the system
   * @param workers
   * @param handlers
   */
  void handle_report(const worker_map_t& workers, const handler_map_t& handlers) override;

  /**
   * Initializes the rgb led
   */
  void initialize() override;
 private:

  /**
   * Sets rgb led to given color, if it is not that color already
   * @param color : color to set
   */
  void set_color(ModeColor color);

  ModeColor _color;
  double _frequency;
  uint8_t _percentage_on;
  const RGB _color_types[ModeColor::mode_color_COUNT];
};

#endif //GPXCONNECTOR_STATE_LED_H
