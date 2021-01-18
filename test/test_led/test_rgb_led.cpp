#include <Arduino.h>

#include <rgb_led.h>
#include <user_config.h>

const int full_brightness = 30;

/**
 * Testing the LED, just look at the pretty colors :-)
 */
void test_rgb_led() {
  RGBLed led(RGB_LED_PIN_R, RGB_LED_PIN_G, RGB_LED_PIN_B);
  led.initialize();

  RGBLed::RGB all_full{full_brightness, full_brightness, full_brightness};
  RGBLed::RGB all_half{full_brightness / 2, full_brightness / 2, full_brightness / 2};

  led.set(all_full);
  delay(2000);
  led.set(all_half);
  delay(2000);
  led.off();
  delay(2000);
  led.set_r(full_brightness);
  delay(2000);
  led.set_r(0);
  led.set_g(full_brightness);
  delay(2000);
  led.set_g(0);
  led.set_b(full_brightness);
  delay(2000);
  led.set({full_brightness, full_brightness, 0});  // red/green (yellow-ish)
  delay(2000);
  led.set({full_brightness, 0, full_brightness});  // red/blue (purple-ish)
  delay(2000);
  led.set({0, full_brightness, full_brightness});  // green/blue (aqua-ish)
  delay(2000);
  led.off();
}
