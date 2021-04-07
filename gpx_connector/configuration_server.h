#ifndef GPXCONNECTOR_SERVER_H
#define GPXCONNECTOR_SERVER_H

#include <WiFi.h>
#include <WebServer.h>

#include <Worker.hpp>
#include <Supervisor.hpp>

#include "local_storage.h"
#include "wifi_connection.h"
#include "status_led.h"

/**
 * Class to host a web server for configuring the ESP32. Will set up an access
 * point based on user_config.h "Access point settings".
 */
class ConfigWebServer : public Worker<bool> {
 public:
  explicit ConfigWebServer(LocalStorage& config, StatusLed& status_led);
  virtual ~ConfigWebServer() = default;

  /**
   * Checks if there are requests and handles them
   */
  int8_t produce_data() override ;

  /**
   * Initialize the web server and endpoints
   */
  void initialize() override;

 protected:
  /**
   * Initialize the web server, does nothing if it is already initialized.
   * @return true if initialization was success
   */
  bool activate(bool retry) override;

  /**
   * Stops the web server
   */
  void deactivate() override;

 private:

  /**
   * Sets the url endpoint methods on the web server
   */
  void add_urls();

  /**
   * Handles request for `/save`
   */
  void handle_save();

  /**
   * Handles request for `/update` post
   */
  void handle_firmware_OTA_upload();

  WebServer _server;
  LocalStorage& _config;

  // Uses status LED directly to set OTA LED, because
  // the loop doesnt get to the supervisor during the upload
  StatusLed& _status_led;
};

#endif //GPXCONNECTOR_SERVER_H
