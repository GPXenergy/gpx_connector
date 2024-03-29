#include <Arduino.h>
#include <unity.h>

void meter_connector_work_full_data_success(void);
void meter_connector_work_full_data_with_as_on_bus_2_success(void);
void meter_connector_work_full_data_dsmr_22_success(void);
void meter_connector_work_full_data_fail_invalid_crc(void);
void meter_connector_work_full_data_idle_incomplete_only_first(void);
void meter_connector_work_full_data_idle_incomplete_only_second_half(void);
void meter_connector_work_full_data_real_meter_multiple_lines(void);


void setup() {
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(meter_connector_work_full_data_success);
  RUN_TEST(meter_connector_work_full_data_with_as_on_bus_2_success);
  RUN_TEST(meter_connector_work_full_data_dsmr_22_success);
  RUN_TEST(meter_connector_work_full_data_fail_invalid_crc);
  RUN_TEST(meter_connector_work_full_data_idle_incomplete_only_first);
  RUN_TEST(meter_connector_work_full_data_idle_incomplete_only_second_half);
  RUN_TEST(meter_connector_work_full_data_real_meter_multiple_lines);

  UNITY_END();
}

void loop() {
}
