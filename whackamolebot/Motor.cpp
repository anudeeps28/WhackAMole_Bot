#include "Motor.h"

Motor::Motor(byte pinEN, byte pinA, byte pinB, float encRes, float kp, float ki, float kd) :
        pinEN{pinEN},
        pinA{pinA},
        pinB{pinB},
        encRes{encRes},
        kp{kp},
        ki{ki},
        kd{kd} {
    pinMode(pinEN, OUTPUT);
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
}

void Motor::stop() {
    analogWrite(pinEN, 0);
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    integralError = 0.0f;
    prevError = 0.0f;
}

void Motor::clear() {
    ticks = 0;
}

void Motor::setVelocity(float velocity) {
    velocityRef = velocity;
}

void Motor::loop() {
    // calculate change in time
    unsigned long currMicros = micros();
    unsigned long deltaMicros = currMicros - lastMicros;
    lastMicros = currMicros;

    // calculate change in distance
    unsigned long currTicks = ticks;
    unsigned long deltaTicks = currTicks - lastTicks;
    lastTicks = currTicks;

    // calculate velocity & time
    velocity = (velocityRef / abs(velocityRef) *
        encRes * deltaTicks /
        (deltaMicros * 1.0e-6f));
    float deltaT = deltaMicros * 1.0e-6f;

    // PID control
    float error = velocityRef - getVelocity();

    float dedt = (error - prevError) / deltaT;
    prevError = error;

    integralError += error * deltaT;

    float control = kp*error + ki*integralError + kd*dedt;

    digitalWrite(pinA, velocityRef > 0.0f ? HIGH : LOW);
    digitalWrite(pinB, velocityRef > 0.0f ? LOW : HIGH);

    // set PWM
    analogWrite(pinEN, max(0, byte(control)));
}

float Motor::getVelocity() {
    return velocity;
}

float Motor::getDistance() {
    return ticks * encRes;
}

void Motor::encoderCallback() {
    ticks++;
}