
#include "access_point.h"
#include "wifi_connection.h"
#include "debugger.h"
#include "http_pages.h"

AccessPoint::AccessPoint(LocalStorage& config) :
    Worker<bool>(false, 0),
    _config(config) {
}

int8_t AccessPoint::produce_data() {
  return BaseWorker::e_worker_idle;
}

bool AccessPoint::activate(bool) {
  // In access point mode there is no internet access
  HttpPages::internet_access = false;
  return WiFiConnection::start_ap_server(_config.get_ap_ssid(), _config.get_ap_password());
}

void AccessPoint::deactivate() {
  // re-set internet access to true, as access point is no longer in use
  HttpPages::internet_access = true;
  WiFiConnection::stop_ap_server();
}
