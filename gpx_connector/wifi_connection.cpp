#include <WiFi.h>
#include <Arduino.h>
#include <ESPmDNS.h>

#include "wifi_connection.h"
#include "debugger.h"

bool WiFiConnection::connect_wifi(const char* ssid, const char* password) {
  // Requires a SSID to connect to
  if(!ssid || strlen(ssid) == 0) {
    DEBUG_PRINTLN("WiFi: No SSID to connect to!");
    return false;
  }
  DEBUG_PRINTF("WiFi: Trying to connect to wifi %s (%s)... \n", ssid, password);
  switch(WiFi.status()) {
    case WL_CONNECTED:
      return true;
    case WL_DISCONNECTED:
      WiFi.reconnect();
      for (int i = 0; i < 10 && !WiFi.isConnected(); ++i) {
        delay(100);
      }
      return WiFi.isConnected();
    default:
      DEBUG_PRINTF("Connect WiFi, current status: %d\n", WiFi.status());
      // connect to the wifi
      password ? WiFi.begin(ssid, password) : WiFi.begin(ssid);
      for (int i = 0; i < 10 && !WiFi.isConnected(); ++i) {
        delay(100);
      }
      DEBUG_PRINTF("WiFi status: %d\n", WiFi.status());
      return WiFi.isConnected();
  }
}

void WiFiConnection::disconnect_wifi() {
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_MODE_NULL);
}

bool WiFiConnection::wifi_connected() {
  return WiFi.isConnected();
}

bool WiFiConnection::start_ap_server(const char* host_ssid, const char* password) {
  // setup wifi access point
  set_hostname(host_ssid);
  WiFi.softAP(host_ssid, password);
  WiFi.softAPsetHostname(host_ssid);
  delay(100); // wait a for the AP to start

  // configure the static IP address
  IPAddress ip(ACCESS_POINT_IP);
  IPAddress n_mask(ACCESS_POINT_NMASK);
  WiFi.softAPConfig(ip, ip, n_mask);

  delay(100); // Make sure everything is up and running

  if(ap_server_up()) {
    DEBUG_PRINTF("WiFi: Access point is up at: %s -> %s\n", host_ssid, WiFi.softAPIP().toString().c_str());
    return true;
  }
  return false;
}

void WiFiConnection::stop_ap_server() {
  WiFi.softAPdisconnect(true);
  delay(20);
}

bool WiFiConnection::ap_server_up() {
  return (WiFi.getMode() & WIFI_MODE_AP) != 0;
}

void WiFiConnection::set_hostname(const char* hostname) {
  WiFi.setHostname(hostname);
}
