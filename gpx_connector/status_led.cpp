
#include "status_led.h"
#include "user_config.h"
#include "debugger.h"
#include "mode_switch.h"
#include "identifiers.h"
#include "smart_meter_connector.h"
#include "api_connector.h"
#include "configuration_server.h"

// seconds passed after last meter data was read to go back to idle state LED: 20 seconds
#define LED_ACTIVE_TO_IDLE_DURATION 20000

StatusLed::StatusLed() :
    RGBLed(RGB_LED_PIN_R, RGB_LED_PIN_G, RGB_LED_PIN_B, RGB_STATE_LED_REVERSED),
    Supervisor(),
    _color(ModeColor::mode_color_off),
    _frequency(0),
    _percentage_on(0),
    _color_types{
        // R    G    B
        {000, 000, 000}, // color off
        {000, 127, 127}, // color config
        {000, 000, 127}, // color config_updating
        {063, 063, 063}, // color active_idle
        {000, 127, 000}, // color active_connected
        {255, 000, 000}, // color active_disconnected
        {255, 000, 000}, // color active_data_error
        {127, 000, 127}, // color active_remote_error
    } {
  // No other constructor implementation
}

void StatusLed::set_values(StatusLed::ModeColor color, double frequency, uint8_t percentage_on) {
  _frequency = frequency;
  _percentage_on = percentage_on;
  _color = color;
  if(_frequency == 0){
    set_color(_color);
  }
}

void StatusLed::loop() {
  if(_frequency > 0) {
    // Blink LED
    double blink_millis = 1000 / _frequency;
    auto cycle_now = millis() % static_cast<uint32_t>(blink_millis);
    auto threshold = static_cast<uint32_t>(blink_millis * (_percentage_on / 100.0));
    if(cycle_now < threshold) {
      set_color(_color);
    } else if(cycle_now > threshold) {
      set_color(StatusLed::mode_color_off);
    }
  }
}

void StatusLed::set_color(StatusLed::ModeColor color) {
  static ModeColor _last = mode_color_off;
  if(color != _last) {
    _last = color;
    set(_color_types[color]);
  }
}

void StatusLed::initialize() {
  RGBLed::initialize();
}

void StatusLed::handle_report(const worker_map_t& workers, const handler_map_t& handlers) {
  const auto& mode_switch = workers.worker<ModeSwitch>(k_worker_mode_switch);
  // LED first based on mode
  switch(mode_switch->get_data()) {
    case Mode::e_mode_config: {
      const auto& server = workers.worker<ConfigWebServer>(k_worker_wifi_config_server);
      // Mode is config, show config color if fresh
      if (mode_switch->is_fresh() || server->is_fresh()) {
        DEBUG_PRINTLN("LED CHANGED TO mode_color_config");
        return set_values(mode_color_config, 0);
      }
      return;
    }
    case Mode::e_mode_active: {
      // Mode is active, color based on meter status
      if (mode_switch->is_fresh()) {
        // First mode switch
        DEBUG_PRINTLN("LED CHANGED TO mode_color_active_idle");
        return set_values(mode_color_active_idle, 0.25, 10);
      }

      const auto& meter = workers.worker<SmartMeterConnector>(k_worker_meter_connector);
      switch(meter->get_status()) {
        case SmartMeterConnector::e_worker_data_read: {
          // Meter data is valid, color based on api connector status
          const auto& api = handlers.handler<ApiConnector>(k_handler_api_reporter);

          if(api->get_active_state() == ApiConnector::e_state_activating_failed){
            // Wifi is not connected
            DEBUG_PRINTLN("LED CHANGED TO mode_color_active_disconnected");
            return set_values(mode_color_active_disconnected);
          }

          switch(api->get_status()) {
            case ApiConnector::e_api_reporter_send_success:
              // Data was successfully send to API
              DEBUG_PRINTLN("LED CHANGED TO mode_color_active_connected");
              return set_values(mode_color_active_connected, 1, 10);
            case ApiConnector::e_api_reporter_error_not_connected:
              // Wifi is not connected
              DEBUG_PRINTLN("LED CHANGED TO mode_color_active_disconnected");
              return set_values(mode_color_active_disconnected);
            case ApiConnector::e_api_reporter_error_server_rejected_post_400:
              // Server rejected post (auth error / data error / server error) TODO split
              DEBUG_PRINTLN("LED CHANGED TO mode_color_active_remote_error");
              return set_values(mode_color_active_remote_error, 0.5, 50);
            case ApiConnector::e_api_reporter_error_server_rejected_post_401:
            case ApiConnector::e_api_reporter_error_server_rejected_post_403:
              // Server rejected post (auth error / data error / server error) TODO split
              DEBUG_PRINTLN("LED CHANGED TO mode_color_active_remote_error");
              return set_values(mode_color_active_remote_error);
            case ApiConnector::e_api_reporter_error_server_rejected_post_5xx:
            case ApiConnector::e_api_reporter_error_remote_not_available:
              // Server is not available / couldn't be reached
              DEBUG_PRINTLN("LED CHANGED TO mode_color_active_remote_error");
              return set_values(mode_color_active_remote_error, 0.25, 10);
            default:
              // No update
              return;
          }
        }
        case SmartMeterConnector::e_worker_error:
          DEBUG_PRINTLN("LED CHANGED TO mode_color_active_data_error");
          return set_values(mode_color_active_data_error, 2, 50);
        case SmartMeterConnector::e_worker_idle:
          if (millis() - meter->get_last_read() > LED_ACTIVE_TO_IDLE_DURATION && _color != mode_color_active_idle) {
            // last read been over 20 seconds ago
            DEBUG_PRINTLN("LED CHANGED TO mode_color_active_idle because no data for some time");
            return set_values(mode_color_active_idle, 0.25, 10);
          }
          return;
        default:
          // No update
          return;
      }
    }
    default:
      // No mode yet
      return;
  }
}

void StatusLed::set_ota_updating() {
  set_values(mode_color_config_updating, 0.5, 10);
}
