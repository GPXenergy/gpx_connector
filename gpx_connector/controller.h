#ifndef GPXCONNECTOR_CONTROLLER_HPP
#define GPXCONNECTOR_CONTROLLER_HPP

#include <Handler.hpp>
#include <Supervisor.hpp>

#include "local_storage.h"

/**
 * Controller for the system, handles workers and handlers, manipulates the system
 */
class Controller : public Supervisor {
 public:

  explicit Controller(LocalStorage& config);
  virtual ~Controller() = default;

  /**
   * Handle the report, specifically the ModeSwitch worker
   * @param workers
   * @param handlers
   */
  void handle_report(const worker_map_t& workers, const handler_map_t& handlers) override;
 private:

  /**
   * Reset the configurations and restart the system
   * Currently unused until the feature is added
   */
  void reset_system();

  LocalStorage& _config;
};

#endif //GPXCONNECTOR_CONTROLLER_HPP
