#pragma once
//
//    FILE: MAX31855.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
// PURPOSE: Arduino library for MAX31855 chip for K type thermocouple
//    DATE: 2014-01-01
//     URL: https://github.com/RobTillaart/MAX31855_RT
//          http://forum.arduino.cc/index.php?topic=208061
//

#include "Arduino.h"

#define MAX31855_VERSION "0.2.0"

// STATE constants returnd by read()
#define STATUS_OK               0x00
#define STATUS_OPEN_CIRCUIT     0x01
#define STATUS_SHORT_TO_GND     0x02
#define STATUS_SHORT_TO_VCC     0x04
#define STATUS_ERROR            0x07
#define STATUS_NOREAD           0x80

//  Thermocouples working is based upon Seebeck effect.
//  Different TC have a different Seebeck Coefficient  (µV/°C)
//  See http://www.analog.com/library/analogDialogue/archives/44-10/thermocouple.html
//
//  As the MAX31855 is designed for K type sensors, one can calculate
//  the factor needed to convert other sensors measurements. 
//  NOTE: this is only a linear approximation.

//  Seebeck Coefficients (sensitivity) from the MAX31855 datasheet page 8
//  to be used in setSeebeckCoefficient()
//      TYPE    COEFFICIENT
#define E_TC    76.373
#define J_TC    57.953
#define K_TC    41.276
#define N_TC    36.256
#define R_TC    10.506
#define S_TC    9.587
#define T_TC    52.18

class MAX31855
{
public:
  MAX31855(uint8_t SCLK, uint8_t CS, uint8_t MISO);
  void begin();

  // returns state - bitfield: 0 = STATUS_OK
  uint8_t read();

  float   getInternal(void) const     { return _internal; }
  float   getTemperature(void);

  uint8_t getStatus(void) const       { return _status; };
  // next 3 applies to last read
  inline  bool shortToGND()  { return (_status & STATUS_SHORT_TO_GND) != 0; };
  inline  bool shortToVCC()  { return (_status & STATUS_SHORT_TO_VCC) != 0; };
  inline  bool openCircuit() { return (_status & STATUS_OPEN_CIRCUIT) != 0; };

  // use offset to callibrate the TC.
  void    setOffset(const float  t)   { _offset = t; };
  float   getOffset() const           { return _offset; };

  //  set the above E_TC (etc) Seebrecht Coefficients
  //  one can also set your own optimized values. 
  void    setSeebeckCoefficient(const float SC) { _SC = SC; };
  float   getSeebeckCoefficient() const         { return _SC; };

  uint32_t lastRead()                 { return _lastRead; };
  uint32_t getRawData()               { return _rawData;};

private:
  uint32_t _read();
  uint8_t  _status;
  float    _internal;
  float    _temperature;
  float    _offset;
  float    _SC;
  uint32_t _lastRead;
  uint32_t _rawData;

  uint8_t  _sclk;
  uint8_t  _miso;
  uint8_t  _cs;
};

// -- END OF FILE --
