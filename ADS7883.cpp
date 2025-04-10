//
//  FILE: ADS7883.cpp
//  AUTHOR: Zue
//  VERSION: 0.1.0
//  DATE: 2025-04-10
//  PURPOSE: Arduino library for ADS7883 12 bit ADC (SPI)

#include "ADS7883.h"


//       HARDWARE SPI
ADS7883::ADS7883(__SPI_CLASS__ * mySPI)
{
  _dataIn     = 255;
  _clock      = 255;
  _select     = 255;
  _hwSPI      = true;
  _mySPI      = mySPI;
  _maxValue   = 255;
  _isLowPower = false;
}


//       SOFTWARE SPI
ADS7883::ADS7883(uint8_t dataIn, uint8_t clock)
{
  _dataIn     = dataIn;
  _clock      = clock;
  _select     = 255;
  _hwSPI      = false;
  _mySPI      = NULL;
  _maxValue   = 255;
  _isLowPower = false;
}


void ADS7883::begin(uint8_t select)
{
  _select = select;
  pinMode(_select, OUTPUT);
  digitalWrite(_select, HIGH);
  digitalWrite(_select, LOW);
  digitalWrite(_select, HIGH);

  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);

  if (_hwSPI)          //  hardware SPI
  {
    //  _mySPI->end();
    //  _mySPI->begin();
  }
  else                 //  software SPI
  {
    pinMode(_dataIn, INPUT);
    pinMode(_clock,  OUTPUT);
    digitalWrite(_clock, HIGH);
  }
}


uint16_t ADS7883::maxValue()
{
  return _maxValue;
}


uint32_t ADS7883::count()
{
  return _count;
}


uint16_t ADS7883::read()
{
  return readADC() >> 2;  //  remove 2 trailing zero's
}


void ADS7883::setSPIspeed(uint32_t speed)
{
  _SPIspeed = speed;
  _spi_settings = SPISettings(_SPIspeed, MSBFIRST, SPI_MODE0);
}


uint32_t ADS7883::getSPIspeed()
{
  return _SPIspeed;
}


bool ADS7883::usesHWSPI()
{
  return _hwSPI;
}


void ADS7883::lowPower()
{
  _isLowPower = true;
  shutDown();
}


void ADS7883::wakeUp()
{
  readADC();
  _isLowPower = false;
}


bool ADS7883::isLowPower()
{
  return _isLowPower;
}


/////////////////////////////////////////////////////////////////////////////
//
//  PROTECTED
//
uint16_t ADS7883::readADC()
{
  _count++;

  uint16_t data = 0;

  if (_hwSPI)  //  hardware SPI
  {
    _mySPI->beginTransaction(_spi_settings);
    digitalWrite(_select, LOW);
    data = _mySPI->transfer16(0);
    digitalWrite(_select, HIGH);
    _mySPI->endTransaction();
  }
  else  //  Software SPI
  {
    digitalWrite(_select, LOW);
    data = swSPI_transfer16();
    digitalWrite(_select, HIGH);
  }
  return data;
}


void ADS7883::shutDown()
{
  if (_hwSPI)  //  hardware SPI
  {
    _mySPI->beginTransaction(_spi_settings);
    digitalWrite(_select, LOW);
    _mySPI->transfer(0);        //  8 pulses
    digitalWrite(_select, HIGH);
    _mySPI->endTransaction();
  }
  else  //  Software SPI
  {
    digitalWrite(_select, LOW);
    swSPI_transfer16(0x0010);  //  4 pulses is enough
    digitalWrite(_select, HIGH);
  }
}


//  MSBFIRST
uint16_t ADS7883::swSPI_transfer16(uint16_t m)
{
  uint8_t clk = _clock;
  uint8_t dai = _dataIn;

  uint16_t rv = 0;
  for (uint16_t mask = m; mask; mask >>= 1)
  {
    digitalWrite(clk, LOW);
    digitalWrite(clk, HIGH);
    if (digitalRead(dai) == HIGH) rv |= mask;
  }
  return rv;
}




//////////////////////////////////////////////////////////////////////
//
//  DERIVED CLASS ADC121S
//
ADS7883S::ADS7883S(__SPI_CLASS__ * mySPI) : ADS7883(mySPI)
{
  _maxValue = 4095;
}

ADS7883S::ADS7883S(uint8_t data, uint8_t clock) : ADS7883(data, clock)
{
  _maxValue = 4095;
}

uint16_t ADS7883S::read()
{
  return readADC();
}


//  -- END OF FILE --

