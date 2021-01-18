#include <Arduino.h>
#include <unity.h>

void meter_connector_work_full_data_success(void);
void meter_connector_work_full_data_fail_invalid_crc(void);
void meter_connector_work_full_data_idle_incomplete_only_first(void);
void meter_connector_work_full_data_idle_incomplete_only_second_half(void);


void setup() {
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(meter_connector_work_full_data_success);
  RUN_TEST(meter_connector_work_full_data_fail_invalid_crc);
  RUN_TEST(meter_connector_work_full_data_idle_incomplete_only_first);
  RUN_TEST(meter_connector_work_full_data_idle_incomplete_only_second_half);

  UNITY_END();
}

void loop() {
}
