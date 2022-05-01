#include "pinout.h"
#include "ColorSensors.h"
#include "I2CMux.h"

ColorSensors::ColorSensors(Adafruit_NeoPixel *pixels) :
        pixels{pixels},
        leftSensor{TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X},
        rightSensor{TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X} {
    // do nothing
}

void ColorSensors::updateSelected() {
    // read pot & write to internal variable
    const int pot = analogRead(PIN_POTENTIOMETER);
    setColor = pot > 750 ? YELLOW : pot > 250 ? BLUE : RED;

    // write to LED
    // yellow is red + green
    pixels->setPixelColor(PIXEL_COLOR_INDEX,
        pixels->Color(
            PIXEL_BRIGHTNESS * (setColor != BLUE),      // red if red or yellow
            PIXEL_BRIGHTNESS * (setColor == YELLOW),    // green if yellow
            PIXEL_BRIGHTNESS * (setColor == BLUE)));    // blue if blue
    pixels->show();
}


bool ColorSensors::checkRight() {
    // read color sensors
    selectRightColor();
    byte red, green, blue;
    uint16_t lux = rightSensor.getRGB(&red, &green, &blue);

    Serial.print("Right Color: ");
    Serial.print(lux);
    Serial.print("L ");
    Serial.print(red);
    Serial.print("R ");
    Serial.print(green);
    Serial.print("G ");
    Serial.print(blue);
    Serial.println("B");


    // comparison
    Color measuredColor = ERROR;
    if (red > MIN_RED)
        measuredColor = RED;
    else if (blue > MIN_BLUE)
        measuredColor = BLUE;
    else if (red + green > MIN_YELLOW)
        measuredColor = YELLOW;

    return setColor == measuredColor;
}
bool ColorSensors::checkLeft() {
    // read color sensors
    selectLeftColor();
    byte red, green, blue;
    uint16_t lux = leftSensor.getRGB(&red, &green, &blue);

    Serial.print("Right Color: ");
    Serial.print(lux);
    Serial.print("L ");
    Serial.print(red);
    Serial.print("R ");
    Serial.print(green);
    Serial.print("G ");
    Serial.print(blue);
    Serial.println("B");

    // comparison
    Color measuredColor = ERROR;
    if (red > MIN_RED)
        measuredColor = RED;
    else if (blue > MIN_BLUE)
        measuredColor = BLUE;
    else if (red + green > MIN_YELLOW)
        measuredColor = YELLOW;

    return setColor == measuredColor;
}
