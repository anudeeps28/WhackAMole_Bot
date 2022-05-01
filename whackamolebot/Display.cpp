#include "Display.h"
#include "I2CMux.h"

Display::Display() :
        lcd{0x27, 20, 4} {
    // do nothing
}

void Display::init() {
    selectLCD();
    lcd.init();
    lcd.backlight();

    lcd.setCursor(18,0);
    lcd.print("cm");

    lcd.setCursor(4,1);
    lcd.print("cm");

    lcd.setCursor(8,1);
    lcd.print("mm/s");

    lcd.setCursor(14,1);
    lcd.print("Fr");

    lcd.setCursor(0,2);
    lcd.print("L");

    lcd.setCursor(14,2);
    lcd.print("S1");

    lcd.setCursor(0,3);
    lcd.print("R");

    lcd.setCursor(14,3);
    lcd.print("S2");
}

void Display::update(const char *state,
        bool leftColor, bool rightColor,
        int leftDist, int rightDist,
        float leftSpeed, float rightSpeed,
        int faceDist, int frontDist, int rearDist) {
    selectLCD();
    lcd.setCursor(0, 0);
    lcd.print(state);
    for (int i = strlen(state); i < 18; i++) {
        lcd.print(" ");
    }

    lcd.setCursor(0, 1);
    lcd.print(msgStr[msgIndex++]);
    msgIndex %= (strlen(msgStr)-1);
    lcd.print(msgStr[msgIndex]);

    lcd.setCursor(1,2);
    lcd.print(leftColor ? "*" : " ");

    lcd.setCursor(1,3);
    lcd.print(rightColor ? "*" : " ");

    lcd.setCursor(3,2);
    sprintf(buffer, "%3d", leftDist);
    lcd.print(buffer);

    lcd.setCursor(3,3);
    sprintf(buffer, "%3d", rightDist);
    lcd.print(buffer);

    lcd.setCursor(7,2);
    dtostrf(leftSpeed, 5, 1, buffer);
    lcd.print(buffer);

    lcd.setCursor(7,3);
    dtostrf(rightSpeed, 5, 1, buffer);
    lcd.print(buffer);

    lcd.setCursor(17,1);
    sprintf(buffer, "%3d", faceDist);
    lcd.print(buffer);

    lcd.setCursor(17,2);
    sprintf(buffer, "%3d", frontDist);
    lcd.print(buffer);

    lcd.setCursor(17,3);
    sprintf(buffer, "%3d", rearDist);
    lcd.print(buffer);
}