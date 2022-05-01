#pragma once

#include <LiquidCrystal_I2C.h>

class Display {
public:
    Display();
    void init();
    void update(const char* state,
        bool leftColor, bool rightColor,
        int leftDist, int rightDist,
        float leftSpeed, float rightSpeed,
        int faceDist, int frontDist, int rearDist);
private:
    LiquidCrystal_I2C lcd;
    const char *msgStr = "CAUTION: this robot is an asshole   ";
    char buffer[10];
    int msgIndex;
};