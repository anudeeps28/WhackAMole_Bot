#pragma once

#include <Arduino.h>

class WallFollower {
public:
    // constructor
    WallFollower();
    void calibrate();

    // send pulse to sonars
    // call this on a timer
    void sendPulse();

    // get effort for motors based on control law
    byte getLeftMotorEffort();
    byte getRightMotorEffort();

    // get the range of the sensors
    // units in cm
    byte getFrontRange();
    byte getRearRange();
    byte getFaceRange();

private:
    // most recent measurements
    static volatile unsigned long frontMicros;
    static volatile unsigned long rearMicros;

    // interrupt callbacks
    static void frontCb();
    static void rearCb();

    // nominal measurements for computing error
    int frontNominal;
    int rearNominal;

    // parameters
    const float alpha = 1.5;
    const float beta = 0.25;
    const byte nominalSpeed = 50;
};
