#pragma once

#include <Servo.h>

#define SERVO_LEFT (0)
#define SERVO_CENTER (87)
#define SERVO_RIGHT (180)
#define SERVO_DELAY (30)

class Dropper {
public:
    Dropper();
    void init();

    // BLOCKING functions to deploy left and right
    void deployLeft();
    void deployRight();
private:
    Servo servo;
    int angle;
};