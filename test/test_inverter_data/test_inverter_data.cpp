#include <Arduino.h>
#include <unity.h>

void data_get_solar_no_measurements_success(void);
void data_get_solar_one_measurement_success(void);
void data_get_solar_multiple_measurement_success(void);
void data_get_solar_multiple_measurement_overflow_ignored_success(void);


void setup() {
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(data_get_solar_no_measurements_success);
  RUN_TEST(data_get_solar_one_measurement_success);
  RUN_TEST(data_get_solar_multiple_measurement_success);
  RUN_TEST(data_get_solar_multiple_measurement_overflow_ignored_success);

  UNITY_END();
}

void loop() {
}
