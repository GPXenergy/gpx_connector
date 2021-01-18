#include <Arduino.h>
#include <unity.h>

void api_connector_handler_generate_payload_full_success(void);
void api_connector_handler_generate_payload_no_solar_success(void);
void api_connector_handler_generate_payload_no_gas_success(void);
void api_connector_handler_generate_payload_no_power_gas_solar_success(void);


void setup() {
  delay(2000);

  UNITY_BEGIN();

  RUN_TEST(api_connector_handler_generate_payload_full_success);
  RUN_TEST(api_connector_handler_generate_payload_no_solar_success);
  RUN_TEST(api_connector_handler_generate_payload_no_gas_success);
  RUN_TEST(api_connector_handler_generate_payload_no_power_gas_solar_success);

  UNITY_END();
}

void loop() {
}
