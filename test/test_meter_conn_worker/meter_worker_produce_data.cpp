#include <Arduino.h>
#include <unity.h>
#include <smart_meter_connector.h>

/**
 * Test parsing of a valid reading
 */
void meter_connector_work_full_data_success(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/XMX5LGBBFG1009321760\r\n"
      "\r\n"
      "1-3:0.2.8(42)\r\n"
      "0-0:1.0.0(200727151422S)\r\n"
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n"
      "1-0:1.8.1(007155.388*kWh)\r\n"
      "1-0:1.8.2(007088.718*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.276*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00009)\r\n"
      "0-0:96.7.9(00004)\r\n"
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n"
      "1-0:32.32.0(00000)\r\n"
      "1-0:32.36.0(00000)\r\n"
      "0-0:96.13.1()\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:31.7.0(001*A)\r\n"
      "1-0:21.7.0(00.276*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(4730303139333430323834343236393136)\r\n"
      "0-1:24.2.1(200727150000S)(06044.581*m3)\r\n"
      "!1626\r\n");

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);

  const P1Data& data = meter_connector.get_data();
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, data.get_status());
  TEST_ASSERT_EQUAL(42, data.get_version());
  TEST_ASSERT_EQUAL_STRING("200727151422S", data.get_power_timestamp());
  TEST_ASSERT_EQUAL_STRING("4530303331303033323035383733373136", data.get_sn_power());
  TEST_ASSERT_EQUAL_STRING("007155.388", data.get_import_1());
  TEST_ASSERT_EQUAL_STRING("007088.718", data.get_import_2());
  TEST_ASSERT_EQUAL_STRING("000000.000", data.get_export_1());
  TEST_ASSERT_EQUAL_STRING("000000.000", data.get_export_2());
  TEST_ASSERT_EQUAL(2, data.get_tariff());
  TEST_ASSERT_EQUAL_STRING("00.276", data.get_actual_import());
  TEST_ASSERT_EQUAL_STRING("00.000", data.get_actual_export());
  TEST_ASSERT_EQUAL_STRING("4730303139333430323834343236393136", data.get_sn_gas());
  TEST_ASSERT_EQUAL_STRING("200727150000S", data.get_gas_timestamp());
  TEST_ASSERT_EQUAL_STRING("06044.581", data.get_gas_import());

}

/**
 * Test parsing of a valid reading with gas data on bus 2
 */
void meter_connector_work_full_data_with_as_on_bus_2_success(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/XMX5LGBBFG1009321760\r\n"
      "\r\n"
      "1-3:0.2.8(42)\r\n"
      "0-0:1.0.0(200727151422S)\r\n"
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n"
      "1-0:1.8.1(007155.388*kWh)\r\n"
      "1-0:1.8.2(007088.718*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.276*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00009)\r\n"
      "0-0:96.7.9(00004)\r\n"
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n"
      "1-0:32.32.0(00000)\r\n"
      "1-0:32.36.0(00000)\r\n"
      "0-0:96.13.1()\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:31.7.0(001*A)\r\n"
      "1-0:21.7.0(00.276*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303139333430323834343236393136)\r\n"
      "0-2:24.2.1(200727150000S)(06044.581*m3)\r\n"
      "!\r\n");

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);

  const P1Data& data = meter_connector.get_data();
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, data.get_status());
  TEST_ASSERT_EQUAL(42, data.get_version());
  TEST_ASSERT_EQUAL_STRING("200727151422S", data.get_power_timestamp());
  TEST_ASSERT_EQUAL_STRING("4530303331303033323035383733373136", data.get_sn_power());
  TEST_ASSERT_EQUAL_STRING("007155.388", data.get_import_1());
  TEST_ASSERT_EQUAL_STRING("007088.718", data.get_import_2());
  TEST_ASSERT_EQUAL_STRING("000000.000", data.get_export_1());
  TEST_ASSERT_EQUAL_STRING("000000.000", data.get_export_2());
  TEST_ASSERT_EQUAL(2, data.get_tariff());
  TEST_ASSERT_EQUAL_STRING("00.276", data.get_actual_import());
  TEST_ASSERT_EQUAL_STRING("00.000", data.get_actual_export());
  TEST_ASSERT_EQUAL_STRING("4730303139333430323834343236393136", data.get_sn_gas());
  TEST_ASSERT_EQUAL_STRING("200727150000S", data.get_gas_timestamp());
  TEST_ASSERT_EQUAL_STRING("06044.581", data.get_gas_import());

}

/**
 * Test parsing of a valid reading
 */
void meter_connector_work_full_data_dsmr_22_success(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/ISk5\\2ME382-1003\r\n"
      "\r\n"
      "0-0:96.1.1(4B414C37303035313039373438383132)\r\n"
      "1-0:1.8.1(18336.285*kWh)\r\n"
      "1-0:1.8.2(19558.900*kWh)\r\n"
      "1-0:2.8.1(00000.000*kWh)\r\n"
      "1-0:2.8.2(00000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(0000.57*kW)\r\n"
      "1-0:2.7.0(0000.00*kW)\r\n"
      "0-0:17.0.0(0999.00*kW)\r\n"
      "0-0:96.3.10(1)\r\n"
      "0-0:96.13.1()\r\n"
      "0-0:96.13.0()\r\n"
      "0-1:24.1.0(3)\r\n"
      "0-1:96.1.0(3238303131303031323139323339353132)\r\n"
      "0-1:24.3.0(210413190000)(00)(60)(1)(0-1:24.2.1)(m3)\r\n"
      "(16083.008)\r\n"
      "0-1:24.4.0(1)\r\n"
      "!\r\n"
  );

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);

  const P1Data& data = meter_connector.get_data();
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, data.get_status());
  TEST_ASSERT_EQUAL(0, data.get_version());
  TEST_ASSERT_EQUAL_STRING("now", data.get_power_timestamp());
  TEST_ASSERT_EQUAL_STRING("4B414C37303035313039373438383132", data.get_sn_power());
  TEST_ASSERT_EQUAL_STRING("18336.285", data.get_import_1());
  TEST_ASSERT_EQUAL_STRING("19558.900", data.get_import_2());
  TEST_ASSERT_EQUAL_STRING("00000.000", data.get_export_1());
  TEST_ASSERT_EQUAL_STRING("00000.000", data.get_export_2());
  TEST_ASSERT_EQUAL(2, data.get_tariff());
  TEST_ASSERT_EQUAL_STRING("0000.57", data.get_actual_import());
  TEST_ASSERT_EQUAL_STRING("0000.00", data.get_actual_export());
  TEST_ASSERT_EQUAL_STRING("3238303131303031323139323339353132", data.get_sn_gas());
  TEST_ASSERT_EQUAL_STRING("210413190000", data.get_gas_timestamp());
  TEST_ASSERT_EQUAL_STRING("16083.008", data.get_gas_import());

}

/**
 * Test parsing of an invalid reading
 */
void meter_connector_work_full_data_fail_invalid_crc(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/XMX5LGBBFG1009321760\r\n"
      "\r\n"
      "1-3:0.2.8(42)\r\n"
      "0-0:1.0.0(200727151422S)\r\n"
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n"
      "1-0:1.8.1(007155.388*kWh)\r\n"
      "1-0:1.8.2(007088.718*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.276*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00009)\r\n"
      "0-0:96.7.9(00004)\r\n"
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n"
      "1-0:32.32.0(00000)\r\n"
      "1-0:32.36.0(00000)\r\n"
      "0-0:96.13.1()\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:31.7.0(001*A)\r\n"
      "1-0:21.7.0(00.276*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(4730303139333430323834343236393136)\r\n"
      "0-1:24.2.1(200727150000S)(06044.581*m3)\r\n"
      "!1627\r\n");

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(SmartMeterConnector::Status::e_worker_error, meter_connector.get_status());

  const P1Data& data = meter_connector.get_data();
  TEST_ASSERT_EQUAL(P1Data::k_p1_invalid, data.get_status());
}

/**
 * Test parsing of an incomplete reading
 */
void meter_connector_work_full_data_idle_incomplete_only_first(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/XMX5LGBBFG1009321760\r\n"
      "\r\n"
      "1-3:0.2.8(42)\r\n"
      "0-0:1.0.0(200727151422S)\r\n"
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n"
      "1-0:1.8.1(007155.388*kWh)\r\n"
      "1-0:1.8.2(007088.718*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.276*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00009)\r\n"
      "0-0:96.7.9(00004)\r\n"
      // ... "missing" the rest of the lines
  );

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_idle);

  const P1Data& data = meter_connector.get_data();
  TEST_ASSERT_EQUAL(P1Data::k_p1_gathering, data.get_status());
}

/**
 * Test parsing of an incomplete reading only received second half
 */
void meter_connector_work_full_data_idle_incomplete_only_second_half(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      // ... "missing" the first half of the complete telegram message
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n"
      "1-0:32.32.0(00000)\r\n"
      "1-0:32.36.0(00000)\r\n"
      "0-0:96.13.1()\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:31.7.0(001*A)\r\n"
      "1-0:21.7.0(00.276*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(4730303139333430323834343236393136)\r\n"
      "0-1:24.2.1(200727150000S)(06044.581*m3)\r\n"
      "!1627\r\n");

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  // When
  bool result = meter_connector.work();

  // Then
  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_idle);

  const P1Data& data = meter_connector.get_data();
  // data is still empty, it only starts gathering after the header line is read
  TEST_ASSERT_EQUAL(P1Data::k_p1_empty, data.get_status());
}

/**
 * Test for meter that gives errors
 */
void meter_connector_work_full_data_real_meter_multiple_lines(void) {
  // Given
  StreamString meter_stream;
  meter_stream.print(
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150620S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!628E\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150621S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!A473\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150622S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!AF77\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150623S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!698A\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150624S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!B97F\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150625S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!7F82\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150626S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.9*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!7486\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150627S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.8*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!4E16\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150628S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.8*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!6902\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150629S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.8*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!AFFF\r\n"
      "\r\n"
      "/XMX5LGF0000443965944\r\n"
      "1-3:0.2.8(50)\r\n"
      "0-0:1.0.0(210915150630S)\r\n"
      "0-0:96.1.1(4530303531303034333936353934343138)\r\n"
      "1-0:1.8.1(001116.939*kWh)\r\n"
      "1-0:1.8.2(001018.626*kWh)\r\n"
      "1-0:2.8.1(000000.000*kWh)\r\n"
      "1-0:2.8.2(000000.000*kWh)\r\n"
      "0-0:96.14.0(0002)\r\n"
      "1-0:1.7.0(00.124*kW)\r\n"
      "1-0:2.7.0(00.000*kW)\r\n"
      "0-0:96.7.21(00016)\r\n"
      "0-0:96.7.9(00002)\r\n"
      "1-0:99.97.0(1)(0-0:96.7.19)(180928065508S)(0000000895*s)\r\n"
      "1-0:32.32.0(00006)\r\n"
      "1-0:32.36.0(00001)\r\n"
      "0-0:96.13.0()\r\n"
      "1-0:32.7.0(234.6*V)\r\n"
      "1-0:31.7.0(000*A)\r\n"
      "1-0:21.7.0(00.124*kW)\r\n"
      "1-0:22.7.0(00.000*kW)\r\n"
      "0-1:24.1.0(003)\r\n"
      "0-1:96.1.0(0000000000000000000000000000000000)\r\n"
      "0-2:24.1.0(003)\r\n"
      "0-2:96.1.0(4730303339303031393036353833343139)\r\n"
      "0-2:24.2.1(210915150459S)(01792.269*m3)\r\n"
      "!F5EE\r\n"
  );

  SmartMeterConnector meter_connector(meter_stream);
  meter_connector.set_active(true);

  bool result;

  // When
  result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, meter_connector.get_data().get_status());

  // When
  result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, meter_connector.get_data().get_status());
  TEST_ASSERT_EQUAL_STRING("210915150620S", meter_connector.get_data().get_power_timestamp());

  // When
  result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, meter_connector.get_data().get_status());
  TEST_ASSERT_EQUAL_STRING("210915150622S", meter_connector.get_data().get_power_timestamp());

  // When
  result = meter_connector.work();

  // Then
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(meter_connector.get_status(), SmartMeterConnector::Status::e_worker_data_read);
  TEST_ASSERT_EQUAL_STRING("210915150623S", meter_connector.get_data().get_power_timestamp());
  TEST_ASSERT_EQUAL(P1Data::k_p1_valid, meter_connector.get_data().get_status());
}
