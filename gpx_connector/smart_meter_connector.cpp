#include "smart_meter_connector.h"
#include "debugger.h"

SmartMeterConnector::SmartMeterConnector(Stream& serial_connection) :
    Worker<P1Data>(),
    _meter_connection_stream(serial_connection),
    _buffer(""),
    _last_read(0) {
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
      DEBUG_PRINT(_buffer.c_str());
      _buffer.clear();
      if(data.is_complete()) {
        _last_read = millis();
//        return Worker::e_worker_idle;
        return data.valid_reading() ? Worker::e_worker_data_read : Worker::e_worker_error;
      }
    }
  }
  return Worker::e_worker_idle;
}
