#include <Arduino.h>
#include "Dropper.h"
#include "pinout.h"

Dropper::Dropper() {
    // do nothing
}

void Dropper::init() {
    servo.attach(PIN_SERVO);
    angle = SERVO_CENTER;
    servo.write(angle);
    delay(500);
}

void Dropper::deployLeft() {
    while(angle != SERVO_LEFT) {
        angle += (SERVO_LEFT - SERVO_CENTER) / abs(SERVO_CENTER - SERVO_LEFT);
        servo.write(angle);
        delay(SERVO_DELAY);
    }
    while(angle != SERVO_CENTER) {
        angle += (SERVO_CENTER - SERVO_LEFT) / abs(SERVO_LEFT - SERVO_CENTER);
        servo.write(angle);
        delay(SERVO_DELAY);
    }
}

void Dropper::deployRight() {
    while(angle != SERVO_RIGHT) {
        angle += (SERVO_RIGHT - SERVO_CENTER) / abs(SERVO_CENTER - SERVO_RIGHT);
        servo.write(angle);
        delay(SERVO_DELAY);
    }
    while(angle != SERVO_CENTER) {
        angle += (SERVO_CENTER - SERVO_RIGHT) / abs(SERVO_RIGHT - SERVO_CENTER);
        servo.write(angle);
        delay(SERVO_DELAY);
    }
}