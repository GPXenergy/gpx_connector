#include <unity.h>
#include <inverter_data.h>

/**
 *
 */
void data_get_solar_no_measurements_success(void) {
  /// Given
  InverterData inverter_data;

  /// When
  // Nothing

  /// Then
  TEST_ASSERT_EQUAL(0, inverter_data.get_solar());
}

/**
 *
 */
void data_get_solar_one_measurement_success(void) {
  /// Given
  InverterData inverter_data;

  /// When
  inverter_data.set_solar(55.33);

  /// Then
  TEST_ASSERT_EQUAL(55.33, inverter_data.get_solar());
}

/**
 *
 */
void data_get_solar_multiple_measurement_success(void) {
  /// Given
  InverterData inverter_data;

  /// When
  inverter_data.set_solar(1.11);
  inverter_data.set_solar(55.82);
  inverter_data.set_solar(4132.612);
  inverter_data.set_solar(5521.642);
  inverter_data.set_solar(2500.42);
  inverter_data.set_solar(4.133);

  /// Then
  TEST_ASSERT_EQUAL(2500.42, inverter_data.get_solar());
}

/**
 *
 */
void data_get_solar_multiple_measurement_overflow_ignored_success(void) {
  /// Given
  InverterData inverter_data;

  /// When
  inverter_data.set_solar(1.11);
  inverter_data.set_solar(55.82);
  inverter_data.set_solar(4132.612);
  inverter_data.set_solar(5521.642);
  inverter_data.set_solar(4728.144);
  inverter_data.set_solar(1);  // From here its no longer ignored
  inverter_data.set_solar(1);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);
  inverter_data.set_solar(0);

  /// Then
  TEST_ASSERT_EQUAL(0, inverter_data.get_solar());
}
