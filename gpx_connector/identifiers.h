#ifndef GPXCONNECTOR_IDENTIFIERS_H_
#define GPXCONNECTOR_IDENTIFIERS_H_

/**
 * The worker identifiers in the system
 */
enum DataWorkers {
  k_worker_meter_connector,
  k_worker_inverter_connector,
  k_worker_wifi_access_point,
  k_worker_wifi_config_server,
  k_worker_mode_switch,
};

/**
 * The handler identifiers in the system
 */
enum DataReporters {
  k_handler_api_reporter,
};

#endif //GPXCONNECTOR_IDENTIFIERS_H_
