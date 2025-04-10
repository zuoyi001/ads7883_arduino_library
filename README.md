# ADS7883 Arduino Library

An Arduino library for the **ADS7883** 12-bit SPI Analog-to-Digital Converter (ADC).  
Supports both **hardware SPI** and **software SPI** modes with configurable SPI speed and low-power management.

> Author: Zue  
> Version: 0.1.0  
> Date: 2025-04-10

---

## 📦 Features

- 12-bit ADC readings from the **ADS7883** chip
- Hardware SPI or software (bit-banged) SPI support
- Customizable SPI speed (default: 1 MHz)
- Low-power mode (shutdown and wake-up support)
- Built-in read counter for diagnostics
- Easy-to-extend base class with `ADS7883S` variant

---

## 🧰 Requirements

- Arduino IDE (1.8.x or 2.x)
- A board with SPI support (e.g., Uno, Mega, ESP32C3, RP2040)
- ADS7883 ADC chip


---

## 🔌 Wiring (Hardware SPI Example)

| ADS7883 Pin | ESP32C3 |
|-------------|--------------|
| VDD         | 3.3V / 5V    |
| GND         | GND          |
| CS          | GPIO07       |
| SCLK        | GPIO02 (SCK) |
| DOUT        | GPIO10 (MISO)|

---

## 🚀 Quick Start

```cpp
#include <Arduino.h>
#include "ADS7883.h"

ADS7883 adc;  // Use hardware SPI by default

// Output pin for the Slave Select SPI line of the ADS7883.
int ADS7883_nCS_pin = 7;

void setup() {
  Serial.begin(115200);
  while (!Serial); 

  Serial.println("ADS7883");

  if (adc.usesHWSPI())
  {
    SPI.begin();
  }

  adc.begin(ADS7883_nCS_pin);  //  select pin
  adc.setSPIspeed(16000000);   //16MHz  ESP32C3 max 40MHz

  Serial.println(adc.maxValue());
}

void loop() {
  uint16_t value = adc.read();
  Serial.println(value);
  delay(500);
}


