#include <Arduino.h>
#include <unity.h>

void test_rgb_led();

void setup() {
  delay(2000);
  // Unit test start
  UNITY_BEGIN();

  RUN_TEST(test_rgb_led);

  // Unit test done
  UNITY_END();
}

void loop() {
}
