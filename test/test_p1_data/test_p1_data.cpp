#include <Arduino.h>
#include <unity.h>

void full_p1data_parse_success(void);
void full_p1data_parse_fail_invalid_crc(void);
void half_p1data_parse_incomplete_gathering(void);


void setup() {
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(full_p1data_parse_success);
  RUN_TEST(full_p1data_parse_fail_invalid_crc);
  RUN_TEST(half_p1data_parse_incomplete_gathering);

  UNITY_END();
}

void loop() {
}
