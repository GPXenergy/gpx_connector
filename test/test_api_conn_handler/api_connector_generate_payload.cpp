#include <Arduino.h>
#include <unity.h>
#include <smart_meter_connector.h>
#include <api_connector.h>
#include "../mock_local_storage.h"

/**
 * Test json payload, power gas and solar
 */
void api_connector_handler_generate_payload_full_success(void) {
  // Given
  MockLocalStorage config;
  ApiConnector api_connector(config);
  char payload[PAYLOAD_BUFFER_SIZE];

  const char* lines[] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000004.000*kWh)\r\n",
      "1-0:2.8.2(000005.000*kWh)\r\n",
      "0-0:96.14.0(0002)\r\n",
      "1-0:1.7.0(00.276*kW)\r\n",
      "1-0:2.7.0(00.000*kW)\r\n",
      "0-0:96.7.21(00009)\r\n",
      "0-0:96.7.9(00004)\r\n",
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n",
      "1-0:32.32.0(00000)\r\n",
      "1-0:32.36.0(00000)\r\n",
      "0-0:96.13.1()\r\n",
      "0-0:96.13.0()\r\n",
      "1-0:31.7.0(001*A)\r\n",
      "1-0:21.7.0(00.276*kW)\r\n",
      "1-0:22.7.0(00.000*kW)\r\n",
      "0-1:24.1.0(003)\r\n",
      "0-1:96.1.0(4730303139333430323834343236393136)\r\n",
      "0-1:24.2.1(200727150000S)(06044.581*m3)\r\n",
      "!D4BF\r\n"
  };

  P1Data p1_data;
  for(const auto& line : lines) {
    p1_data.add_line(line);
  }

  InverterData inverter_data;
  inverter_data.set_solar(120.32);

  // When
  api_connector.generate_payload(&p1_data, &inverter_data, payload);

  // Then
  TEST_ASSERT_EQUAL_STRING(
      "{"
      "\"power\":{"
      "\"sn\":\"4530303331303033323035383733373136\","
      "\"timestamp\":\"200727151422S\","
      "\"import_1\":\"007155.388\","
      "\"import_2\":\"007088.718\","
      "\"export_1\":\"000004.000\","
      "\"export_2\":\"000005.000\","
      "\"tariff\":2,"
      "\"actual_import\":\"00.276\","
      "\"actual_export\":\"00.000\""
      "},\"gas\":{"
      "\"sn\":\"4730303139333430323834343236393136\","
      "\"timestamp\":\"200727150000S\","
      "\"gas\":\"06044.581\""
      "},\"solar\":{"
      "\"timestamp\":\"200727151422S\","
      "\"solar\":\"120.320\""
      "}}",
      payload
  );

}

/**
 * Test json payload, without solar data
 */
void api_connector_handler_generate_payload_no_solar_success(void) {
  // Given
  MockLocalStorage config;
  ApiConnector api_connector(config);
  char payload[PAYLOAD_BUFFER_SIZE];

  const char* lines[] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000004.000*kWh)\r\n",
      "1-0:2.8.2(000005.000*kWh)\r\n",
      "0-0:96.14.0(0002)\r\n",
      "1-0:1.7.0(00.276*kW)\r\n",
      "1-0:2.7.0(00.000*kW)\r\n",
      "0-0:96.7.21(00009)\r\n",
      "0-0:96.7.9(00004)\r\n",
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n",
      "1-0:32.32.0(00000)\r\n",
      "1-0:32.36.0(00000)\r\n",
      "0-0:96.13.1()\r\n",
      "0-0:96.13.0()\r\n",
      "1-0:31.7.0(001*A)\r\n",
      "1-0:21.7.0(00.276*kW)\r\n",
      "1-0:22.7.0(00.000*kW)\r\n",
      "0-1:24.1.0(003)\r\n",
      "0-1:96.1.0(4730303139333430323834343236393136)\r\n",
      "0-1:24.2.1(200727150000S)(06044.581*m3)\r\n",
      "!D4BF\r\n"
  };

  P1Data p1_data;
  for(const auto& line : lines) {
    p1_data.add_line(line);
  }

  // When
  api_connector.generate_payload(&p1_data, nullptr, payload);

  // Then
  TEST_ASSERT_EQUAL_STRING(
      "{"
      "\"power\":{"
      "\"sn\":\"4530303331303033323035383733373136\","
      "\"timestamp\":\"200727151422S\","
      "\"import_1\":\"007155.388\","
      "\"import_2\":\"007088.718\","
      "\"export_1\":\"000004.000\","
      "\"export_2\":\"000005.000\","
      "\"tariff\":2,"
      "\"actual_import\":\"00.276\","
      "\"actual_export\":\"00.000\""
      "},\"gas\":{"
      "\"sn\":\"4730303139333430323834343236393136\","
      "\"timestamp\":\"200727150000S\","
      "\"gas\":\"06044.581\""
      "},\"solar\":null}",
      payload
  );

}

/**
 * Test json payload, without gas data
 */
void api_connector_handler_generate_payload_no_gas_success(void) {
  // Given
  MockLocalStorage config;
  ApiConnector api_connector(config);
  char payload[PAYLOAD_BUFFER_SIZE];

  const char* lines[] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000004.000*kWh)\r\n",
      "1-0:2.8.2(000005.000*kWh)\r\n",
      "0-0:96.14.0(0002)\r\n",
      "1-0:1.7.0(00.276*kW)\r\n",
      "1-0:2.7.0(00.000*kW)\r\n",
      "0-0:96.7.21(00009)\r\n",
      "0-0:96.7.9(00004)\r\n",
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n",
      "1-0:32.32.0(00000)\r\n",
      "1-0:32.36.0(00000)\r\n",
      "0-0:96.13.1()\r\n",
      "0-0:96.13.0()\r\n",
      "1-0:31.7.0(001*A)\r\n",
      "1-0:21.7.0(00.276*kW)\r\n",
      "1-0:22.7.0(00.000*kW)\r\n",
      "0-1:24.1.0(003)\r\n",
      "!2AE6\r\n"
  };

  P1Data p1_data;
  for(const auto& line : lines) {
    p1_data.add_line(line);
  }

  TEST_ASSERT_EQUAL(P1Data::P1DataStatus::k_p1_valid,p1_data.get_status());
  TEST_ASSERT_TRUE(p1_data.valid_reading());
  TEST_ASSERT_EQUAL(2,p1_data.get_tariff());

  // When
  api_connector.generate_payload(&p1_data, nullptr, payload);

  // Then
  TEST_ASSERT_EQUAL_STRING(
      "{"
      "\"power\":{"
      "\"sn\":\"4530303331303033323035383733373136\","
      "\"timestamp\":\"200727151422S\","
      "\"import_1\":\"007155.388\","
      "\"import_2\":\"007088.718\","
      "\"export_1\":\"000004.000\","
      "\"export_2\":\"000005.000\","
      "\"tariff\":2,"
      "\"actual_import\":\"00.276\","
      "\"actual_export\":\"00.000\""
      "},\"gas\":null,\"solar\":null"
      "}",
      payload
  );

}

/**
 * Test json payload, without power, gas and solar data
 */
void api_connector_handler_generate_payload_no_power_gas_solar_success(void) {
  // Given
  MockLocalStorage config;
  ApiConnector api_connector(config);
  char payload[PAYLOAD_BUFFER_SIZE];

  // When
  api_connector.generate_payload(nullptr, nullptr, payload);

  // Then
  TEST_ASSERT_EQUAL_STRING(
      "{"
      "\"power\":null,\"gas\":null,\"solar\":null"
      "}",
      payload
  );

}
