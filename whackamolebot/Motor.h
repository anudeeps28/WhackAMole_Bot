#pragma once

#include <Arduino.h>

#define LEFT_ENC_RES (0.1408f)
#define RIGHT_ENC_RES (1.5374f)

#define LEFT_KP (3.5f)
#define LEFT_KI (2.0f)
#define LEFT_KD (0.0f)

#define RIGHT_KP (0.1)
#define RIGHT_KI (0.5f)
#define RIGHT_KD (0.0f)

class Motor {
public:
    Motor(byte pinEN, byte pinA, byte pinB, float encRes, float kp, float ki, float kd);

    void setVelocity(float velocity);
    void stop();
    void clear();

    // units: mm and mm/s
    float getDistance();
    float getVelocity();

    void encoderCallback();
    void loop();

private:
    float velocityRef, velocity, prevError, integralError;
    unsigned long lastMicros, lastTicks;

    volatile unsigned long ticks;

    const byte pinEN, pinA, pinB;
    const float encRes, kp, ki, kd;
};