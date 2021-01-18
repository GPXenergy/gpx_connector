#ifndef GPXCONNECTOR_ACCESS_POINT_SERVER_H_
#define GPXCONNECTOR_ACCESS_POINT_SERVER_H_

#include <Worker.hpp>

#include "local_storage.h"

/**
 * Access point worker, activates and deactivates the wifi access point for the configuration server
 */
class AccessPoint : public Worker<bool> {
 public:
  explicit AccessPoint(LocalStorage& config);
  virtual ~AccessPoint() = default;

  virtual int8_t produce_data() override;
 protected:
  /**
   * Start the access point
   * @param retry
   * @return
   */
  bool activate(bool retry) override;

  /**
   * Stop the access point
   */
  void deactivate() override;

 private:
  // Uses local storage to get the SSID and password for the Access Point
  LocalStorage& _config;
};

#endif //GPXCONNECTOR_ACCESS_POINT_SERVER_H_
