#ifndef GPXCONNECTOR_ESP_LED_H
#define GPXCONNECTOR_ESP_LED_H

#include <Arduino.h>

/**
 * Some reusable RGB led class
 */
class RGBLed {
 public:
  /// struct that can be used to predefine colors
  typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } RGB;

  /**
   * Create a RGB led connector
   * @param pin_r: pin connected to RED
   * @param pin_g: pin connected to GREEN
   * @param pin_b: pin connected to BLUE
   * @param reversed: true for anode, false for cathode (default cathode)
   */
  RGBLed(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, bool reversed = false);
  virtual ~RGBLed() = default;

  /**
   * Sets the LED channels and turns led off by default
   */
  void initialize();

  /**
   * Set given color
   * @param values
   */
  void set(const RGB& values);

  /**
   * Turn the LED off
   */
  void off();

  /**
   * set the R value of the RGB LED
   * @param value : value (0-255)
   */
  void set_r(uint8_t value);

  /**
   * set the G value of the RGB LED
   * @param value : value (0-255)
   */
  void set_g(uint8_t value);

  /**
   * set the B value of the RGB LED
   * @param value : value (0-255)
   */
  void set_b(uint8_t value);

  /**
   * Set the intensity,  0 is always off, 255 is VERY bright, defaults to 30
   * @param intensity : new value (0-255)
   */
  virtual void set_intensity(uint8_t intensity);
  virtual uint8_t get_intensity() const;
 private:

  /**
   * apply a value to the RGB LED
   * @param channel : rgb channel id
   * @param value : value
   */
  void set_channel(uint8_t channel, uint8_t value);

  bool _reversed;
  uint8_t _intensity;
};

#endif //GPXCONNECTOR_ESP_LED_H
