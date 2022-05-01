#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pins
const byte PIN_LED = 18;
const byte PIN_START = 19;
const byte PIN_ESTOP = 17;
const byte PIN_SONAR = 21;
const byte PIN_POT = A0;

Adafruit_NeoPixel pixels(2, PIN_LED, NEO_GRB + NEO_KHZ800);
const byte BRIGHTNESS = 128;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);

enum Color : byte {
  RED,
  BLUE,
  GREEN
};

void setup() {
  // configure pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_START, INPUT_PULLUP);
  pinMode(PIN_ESTOP, INPUT_PULLUP);
  pinMode(PIN_SONAR, OUTPUT);
  pinMode(PIN_POT, INPUT);

  pixels.begin();

}

void loop() {
  // read buttons
  const bool estop_state = digitalRead(PIN_ESTOP) == HIGH;
  const bool start_state = digitalRead(PIN_START) == LOW;

  // read pot
  const int pot_state = analogRead(PIN_POT);
  const byte selected_color = pot_state > 682 ? GREEN : pot_state > 341 ? BLUE : RED;

  // read color sensors
  byte red, green, blue;
  uint16_t lux = tcs.getRGB(&red, &green, &blue);

  if (lux > 1200) {
    if (red > green && red > blue) {
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(BRIGHTNESS, 0, 0));
      pixels.setPixelColor(1, pixels.Color(BRIGHTNESS, 0, 0));  
      pixels.show();
    }
    if (green > red && green > blue) {
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, BRIGHTNESS, 0));
      pixels.setPixelColor(1, pixels.Color(0, BRIGHTNESS, 0));  
      pixels.show();
    }
    if (blue > red && blue > green) {
      pixels.clear();
      pixels.setPixelColor(0, pixels.Color(0, 0, BRIGHTNESS));
      pixels.setPixelColor(1, pixels.Color(0, 0, BRIGHTNESS));  
      pixels.show();
    }
  }
  delay(250);
  
  // read sonar

  // write to LCD

  // write to LEDS
//  pixels.clear();
//  pixels.setPixelColor(0, pixels.Color(BRIGHTNESS*estop_state, BRIGHTNESS*start_state, 0));
//  pixels.setPixelColor(1, pixels.Color(BRIGHTNESS*(selected_color==RED), BRIGHTNESS*(selected_color==GREEN), BRIGHTNESS*(selected_color==BLUE)));
//  pixels.show();
}
