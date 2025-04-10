#pragma once
//
//  FILE: ADS7883.cpp
//  AUTHOR: Zue
//  VERSION: 0.1.0
//  DATE: 2025-04-10
//  PURPOSE: Arduino library for ADS7883 12 bit ADC (SPI)


#include "Arduino.h"
#include "SPI.h"


#define ADS7883_LIB_VERSION       (F("0.1.0"))


#ifndef __SPI_CLASS__
  //  MBED must be tested before RP2040
  #if defined(ARDUINO_ARCH_MBED)
  #define __SPI_CLASS__   SPIClass
  #elif defined(ARDUINO_ARCH_RP2040)
  #define __SPI_CLASS__   SPIClassRP2040
  #else
  #define __SPI_CLASS__   SPIClass
  #endif
#endif


class ADS7883
{
public:
  //       HARDWARE SPI
  ADS7883(__SPI_CLASS__ * mySPI = &SPI);
  //       SOFTWARE SPI
  //       datain = MISO
  ADS7883(uint8_t dataIn, uint8_t clock);

  void     begin(uint8_t select);

  uint16_t maxValue();
  uint16_t read();

  //       speed in Hz
  void     setSPIspeed(uint32_t speed);
  uint32_t getSPIspeed();

  //       debugging
  bool     usesHWSPI();
  uint32_t count();  //  number of channels read.

  void     lowPower();
  void     wakeUp();
  bool     isLowPower();

protected:
  uint8_t  _dataIn;
  uint8_t  _clock;
  uint8_t  _select;
  bool     _hwSPI;

  uint16_t _maxValue;
  uint16_t readADC();
  void     shutDown();

  uint16_t swSPI_transfer16(uint16_t m = 0x8000);

  //  1 MHz is a safe value (datasheet); in a test 4 MHz worked.
  uint32_t        _SPIspeed = 1000000;
  __SPI_CLASS__ * _mySPI;
  SPISettings     _spi_settings;

  uint32_t _count;
  bool     _isLowPower;
};


//////////////////////////////////////////////////////////////////////
//
//  DERIVED CLASSES
//


class ADS7883S : public ADS7883
{
public:
  ADS7883S(__SPI_CLASS__ * mySPI = &SPI);
  ADS7883S(uint8_t data, uint8_t clock);
  uint16_t read();
};


//  -- END OF FILE --

