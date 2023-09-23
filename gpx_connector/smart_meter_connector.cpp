#include "smart_meter_connector.h"
#include "debugger.h"

SmartMeterConnector::SmartMeterConnector(Stream& serial_connection) :
    Worker<P1Data>(),
    _meter_connection_stream(serial_connection),
    _buffer(""),
    _last_read(0),
    _consecutive_fails(0) {
}

uint32_t SmartMeterConnector::get_last_read() const {
  return _last_read;
}

int8_t SmartMeterConnector::produce_data() {
  while(_meter_connection_stream.available() > 0) {
    char c = static_cast<char>(_meter_connection_stream.read());
    _buffer += c;
    if(c == '\n') {
      data.add_line(_buffer);
//      DEBUG_PRINT(_buffer.c_str());
      _buffer.clear();
      if(data.is_complete()) {
        _last_read = millis();
        if (data.valid_reading()) {
          _consecutive_fails = 0;
          return Worker::e_worker_data_read;
        } else if (_consecutive_fails > 2) {
          return Worker::e_worker_error;
        } else {
          // Missing 1 is no issue, missing multiple in a row must throw error
          ++_consecutive_fails;
          return Worker::e_worker_idle;
        }
      }
    }
  }
  return Worker::e_worker_idle;
}
