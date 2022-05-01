#pragma once

#include <Wire.h>
#include "pinout.h"

#define TCAADDR 0x70

// https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/wiring-and-test
static inline void tcaselect(uint8_t i) {
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
};

static inline void selectLCD() {
    tcaselect(I2C_MUX_LCD);
};

static inline void selectLeftColor() {
    tcaselect(I2C_MUX_COLOR_LEFT);
};

static inline void selectRightColor() {
    tcaselect(I2C_MUX_COLOR_RIGHT);
};
