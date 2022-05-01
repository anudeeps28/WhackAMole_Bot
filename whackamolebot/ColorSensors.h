#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"

#define PIXEL_BRIGHTNESS (40)
#define MIN_RED (80)
#define MIN_BLUE (10)
#define MIN_YELLOW (40)

class ColorSensors {
public:
    // constructor
    ColorSensors(Adafruit_NeoPixel *pixels);

    // update the selected color
    // read potentiometer
    // write to internal variable and LED
    void updateSelected();

    // check if the right and left sensors see the correct color
    bool checkRight();
    bool checkLeft();

private:
    // enum for possible colors
    enum Color : byte {
        RED,
        BLUE,
        YELLOW,
        ERROR
    };

    // currently selected color
    Color setColor;

    // pointer to neopixels
    Adafruit_NeoPixel *const pixels;

    // color sensors
    Adafruit_TCS34725 leftSensor;
    Adafruit_TCS34725 rightSensor;
};