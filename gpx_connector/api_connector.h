#ifndef GPXCONNECTOR_APICONNECTOR_H
#define GPXCONNECTOR_APICONNECTOR_H

#include <WiFi.h>
#include <HTTPClient.h>

#include <Handler.hpp>

#include "local_storage.h"
#include "user_config.h"
#include "wifi_connection.h"
#include "p1_data.h"
#include "inverter_data.h"

#define PAYLOAD_BUFFER_SIZE 700

/**
 * Connects over WiFi to the API to send readings
 */
class ApiConnector : public Handler {
 public:
  /**
   * Collection of statuses for the api data handling, default is idle.
   */
  enum ApiHandlerStatus {
    e_api_reporter_idle,
    e_api_reporter_send_success,
    e_api_reporter_error_invalid_data,
    e_api_reporter_error_not_connected,
    e_api_reporter_error_remote_not_available,
    e_api_reporter_error_server_rejected_post_400,
    e_api_reporter_error_server_rejected_post_401,
    e_api_reporter_error_server_rejected_post_403,
    e_api_reporter_error_server_rejected_post_5xx,
  };

  explicit ApiConnector(LocalStorage& config);
  virtual ~ApiConnector() = default;

  /**
   * Fill the _payload with p1 and inverter data
   * @param p1_data: p1 data from the smart meter connection
   * @param inverter_data: inverter data object
   * @return size of written paylaod
   */
  uint16_t generate_payload(const P1Data* p1_data, const InverterData* inverter_data, char* out);

 protected:

  /**
   * Check if enough time has passed to send the latest reading to api
   * @return
   */
  bool time_to_send() const;

  /**
   * Initialize the connection
   * @param initial: set to false if its for reconnect / connect in error
   * @return true if connection with the wifi was made
   */
  bool activate(bool retry) override;

  /**
   * Stop the connection
   */
  void deactivate() override;

  /**
   * handle the worker data, sending power/gas/solar data to the API
   * @param workers
   * @return status
   */
  int8_t handle_produced_work(const worker_map_t& workers) override;

 private:
  /**
   * Add power json data to given payload
   * @param data: P1 data containing the power values
   * @param out: the payload container
   * @return number of written characters
   */
  uint16_t add_power_payload(const P1Data* data, char* out);

  /**
   * Add gas json data to given payload
   * @param data: P1 data containing the power values
   * @param out: the payload container
   * @return number of written characters
   */
  uint16_t add_gas_payload(const P1Data* data, char* out);

  /**
   * Add solar json data to given payload
   * @param data: inverter data object
   * @param out: the payload container
   * @return number of written characters
   */
  uint16_t add_solar_payload(const InverterData* data, const char* timestamp, char* out);

  /**
   * Send a reading to the API
   * @param payload: data to send
   * @return: true if the API call was successful
   */
  ApiHandlerStatus send_payload(const char* payload);

  LocalStorage& _config;
  uint32_t _last_handle;
  char _payload[PAYLOAD_BUFFER_SIZE];
};

#endif //GPXCONNECTOR_APICONNECTOR_H
