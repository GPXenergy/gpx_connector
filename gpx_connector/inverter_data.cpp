#include <Arduino.h>

#include "inverter_data.h"
#include "debugger.h"

InverterData::InverterData() :
    _solar{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    _index(0),
    _size(0) {
}

void InverterData::set_solar(double solar) {
  _solar[_index] = solar;
  _index += 1;
  _index %= SOLAR_LIST_SIZE;
  _size = _size < SOLAR_LIST_SIZE ? _size + 1 : _size;

  if(is_valid()) {
    DEBUG_PRINTF("CURRENT SOLAR VAL: %f\r\n", get_solar());
  }
}

double InverterData::get_solar() const {
  double avg = 0;
  double closestToAvg = 0;
  for(uint8_t i = 0; i < _size; ++i) {
    avg += _solar[i];
  }
  avg /= _size;

  for(uint8_t i = 0; i < _size; ++i) {
    if(abs(avg - _solar[i]) < abs(avg - closestToAvg)) {
      closestToAvg = _solar[i];
    }
  }
  return closestToAvg;
}

bool InverterData::is_valid() const {
  // valid after 60 seconds, first few measurements are overpowered
  return millis() > 60000;
}
