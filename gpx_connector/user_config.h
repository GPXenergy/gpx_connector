#ifndef USER_CONFIG_H
#define USER_CONFIG_H


enum MeterParity {
  e_meter_parity_7E1 = 0,
  e_meter_parity_8N1 = 1,
};


#ifndef GPX_CONN_VERSION
#define GPX_CONN_VERSION "1.0.dev"
#endif

/** Debug config **/
#define ENABLE_DEBUG 1
#define DEBUG_BAUD 115200

/** Hardware pins settings **/
#define RGB_LED_PIN_R 14
#define RGB_LED_PIN_G 12
#define RGB_LED_PIN_B 13
// Set this to true if we use anode LED
#define RGB_STATE_LED_REVERSED false

#define MODE_SWITCH_PIN 18

#define INVERTER_ADC_INPUT 34

/** API connector settings **/
#define API_HOST "dashboard.gpx.nl"
#define HEADER_API_CONTENT_TYPE "application/json"
#define HEADER_API_USER_AGENT "GPXCONN/" GPX_CONN_VERSION
#define API_MEASUREMENTS_ENDPOINT "https://" API_HOST "/api/meters/measurement/"
#define API_MEASUREMENTS_TEST_ENDPOINT API_MEASUREMENTS_ENDPOINT "test/"

/** Access point settings **/
#define SERVER_WIFI_PORT        80
#define ACCESS_POINT_IP         {192, 168, 5, 1}
#define ACCESS_POINT_NMASK      {255, 255, 255, 0}

/** Default ESP configurations **/
#define D_ACCESS_POINT_SSID     "GPXConnector"
#define D_ACCESS_POINT_PASSWORD "smartgpx"
#define D_WIFI_SSID             ""
#define D_WIFI_PASSWORD         ""
#define D_APIKEY                ""
#define D_METER_BAUD            115200
#define D_METER_PARITY          MeterParity::e_meter_parity_8N1
#define D_INVERTER_ENABLED      false
#define D_INVERTER_SENSOR_AMPS  50
#define D_APIKEY                ""
#define D_USE_DEV_SERVER        false

#endif