#include "Adafruit_TCS34725.h"

// pins
const byte PIN_LED = 18;
const byte PIN_START = 19;
const byte PIN_ESTOP = 17;
const byte PIN_SONAR = 21;
const byte PIN_POT = A0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);

enum Color : byte {
  RED,
  BLUE,
  GREEN
};

void setup() {
  Serial.begin(19200);
}

void loop() {
  // read color sensors
  byte red, green, blue;
  uint16_t lux = tcs.getRGB(&red, &green, &blue);

  Serial.print(red);
  Serial.print(",");
  Serial.print(green);
  Serial.print(",");
  Serial.print(blue);
  Serial.print(" @ ");
  Serial.println(lux);

  delay(100);
}
