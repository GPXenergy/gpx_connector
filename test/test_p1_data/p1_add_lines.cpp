#include <unity.h>
#include <p1_data.h>

/**
 * Test parsing of a valid reading
 */
void full_p1data_parse_success(void) {
  const char* lines[26] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000000.000*kWh)\r\n",
      "1-0:2.8.2(000000.000*kWh)\r\n",
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
      "!1626\r\n"
  };

  P1Data data;
  for(int i = 0; i < 26; i++) {
    data.add_line(lines[i]);
  }

  TEST_ASSERT_EQUAL(data.get_status(), P1Data::k_p1_valid);

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
 * Test parsing of a invalid reading (crc mismatch)
 */
void full_p1data_parse_fail_invalid_crc(void) {
  const char* lines[26] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000000.000*kWh)\r\n",
      "1-0:2.8.2(000000.000*kWh)\r\n",
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
      "!1111\r\n"
  };

  P1Data data;
  for(int i = 0; i < 26; i++) {
    data.add_line(lines[i]);
  }

  TEST_ASSERT_EQUAL(data.get_status(), P1Data::k_p1_invalid);

}


/**
 * Test parsing of incomplete data
 */
void half_p1data_parse_incomplete_gathering(void) {
  // Incomplete data, missing 10 lines
  const char* lines[16] = {
      "/XMX5LGBBFG1009321760\r\n",
      "\r\n",
      "1-3:0.2.8(42)\r\n",
      "0-0:1.0.0(200727151422S)\r\n",
      "0-0:96.1.1(4530303331303033323035383733373136)\r\n",
      "1-0:1.8.1(007155.388*kWh)\r\n",
      "1-0:1.8.2(007088.718*kWh)\r\n",
      "1-0:2.8.1(000000.000*kWh)\r\n",
      "1-0:2.8.2(000000.000*kWh)\r\n",
      "0-0:96.14.0(0002)\r\n",
      "1-0:1.7.0(00.276*kW)\r\n",
      "1-0:2.7.0(00.000*kW)\r\n",
      "0-0:96.7.21(00009)\r\n",
      "0-0:96.7.9(00004)\r\n",
      "1-0:99.97.0(4)(0-0:96.7.19)(180619133556S)(0000000314*s)(180219134256W)(0000000633*s)(180219133039W)(0000007917*s)(160704090617S)(0000004176*s)\r\n",
      "1-0:32.32.0(00000)\r\n"
  };
  P1Data data;

  // When
  for(int i = 0; i < 16; i++) {
    data.add_line(lines[i]);
  }

  // Then
  TEST_ASSERT_EQUAL(data.get_status(), P1Data::k_p1_gathering);
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
  // these are still empty
  TEST_ASSERT_EQUAL_STRING("", data.get_sn_gas());
  TEST_ASSERT_EQUAL_STRING("", data.get_gas_timestamp());
  TEST_ASSERT_EQUAL_STRING("", data.get_gas_import());

}

