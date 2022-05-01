#include <Arduino.h>
#include "pinout.h"
#include "WallFollower.h"
#include "Motor.h"
#include "ColorSensors.h"
#include "I2CMux.h"
#include "Display.h"
#include "Dropper.h"

// parameters
#define DIST_PREP (10)
#define DIST_SQ1 (30)
#define DIST_SQ2 (18)
#define DIST_SQ3 (6)

#define MAX_TIME (120000)

WallFollower wallFollower;
Motor leftMotor(PIN_MOTOR_LEFT_EN, PIN_MOTOR_LEFT_A, PIN_MOTOR_LEFT_B, LEFT_ENC_RES, LEFT_KP, LEFT_KI, LEFT_KD);
Motor rightMotor(PIN_MOTOR_RIGHT_EN, PIN_MOTOR_RIGHT_A, PIN_MOTOR_RIGHT_B, RIGHT_ENC_RES, RIGHT_KP, RIGHT_KI, RIGHT_KD);
Adafruit_NeoPixel pixels(PIXEL_COUNT, PIN_PIXELS, NEO_GRB + NEO_KHZ800);
ColorSensors colorSensors(&pixels);
Display display;
Dropper dropper;

// FSM states
enum states : byte {
    WAIT,
    PREP_REVERSE,
    PREP_ROTATE,
    SQ1_MOVE,
    SQ1_DEPLOY_LEFT,
    SQ1_DEPLOY_RIGHT,
    SQ2_MOVE,
    SQ2_DEPLOY,
    SQ3_MOVE,
    SQ3_DEPLOY,
    FINISH_ROTATE,
    FINISH_MOVE,
    HALT
};
byte state = WAIT; // initial state
byte count = 0;

unsigned long startTime;
byte lcdWait = 0;

void setup() {
    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_LEFT_ENC), [](){leftMotor.encoderCallback();}, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_RIGHT_ENC), [](){rightMotor.encoderCallback();}, CHANGE);

    pinMode(PIN_ESTOP, INPUT_PULLUP);
    pinMode(PIN_GO, INPUT_PULLUP);

    Serial.begin(115200);
    Wire.begin();
    pixels.begin();
    display.init();
    dropper.init();
    // wallFollower.calibrate();

    startTime = millis();
}

void loop() {
    // run high speed loops
    wallFollower.sendPulse();
    leftMotor.loop();
    rightMotor.loop();
    delay(50);

    // run FSM
   stepFSM(
       digitalRead(PIN_ESTOP) == HIGH || startTime + MAX_TIME < millis(),
       digitalRead(PIN_GO) == LOW,
       colorSensors.checkLeft(),
       colorSensors.checkRight(),
       false, false, false, false);
    //    wallFollower.getFrontRange() > DIST_PREP,
    //    wallFollower.getFrontRange() < DIST_SQ1,
    //    wallFollower.getFrontRange() < DIST_SQ2,
    //    wallFollower.getFrontRange() < DIST_SQ3);

    // perform action
    if (state == WAIT) {
        colorSensors.updateSelected();
        startTime = millis();
    }
    if (state == PREP_REVERSE) {
        // wall follow in reverse
        leftMotor.setVelocity(-250.0);
        rightMotor.setVelocity(-250.0);
    }
    else if (state == SQ1_MOVE || state == SQ2_MOVE || state == SQ3_MOVE || state == FINISH_MOVE) {
        // wall follow in forward
        leftMotor.setVelocity(250.0);
        rightMotor.setVelocity(250.0);
    }
    else if (state == PREP_ROTATE) {
        // rotate 90 degrees left (blocking)
        // leftMotor.setVelocity(-250.0);
        // rightMotor.setVelocity(250.0);
    }
    else if (state == SQ1_DEPLOY_LEFT) {
        // deploy left (blocking)
        dropper.deployLeft();
    }
    else if (state == SQ1_DEPLOY_RIGHT || state == SQ2_DEPLOY || state == SQ3_DEPLOY) {
        // deploy right (blocking)
        dropper.deployRight();
    }

    // write state to pixels
    pixels.setPixelColor(PIXEL_STATE_INDEX, pixels.Color(
        PIXEL_BRIGHTNESS * (state == HALT || state == WAIT),    // red if halted or waiting
        PIXEL_BRIGHTNESS * (state != HALT),                     // green if running or waiting
        0));                                                    // no blue
    pixels.show();

    // write stuff to LCD
    if (++lcdWait >= 10) {
        display.update(stateToString(state),
            colorSensors.checkLeft(), colorSensors.checkRight(),
            int(leftMotor.getDistance())/10,
            int(rightMotor.getDistance())/10,
            leftMotor.getVelocity(),
            rightMotor.getVelocity(),
            wallFollower.getFaceRange(),
            wallFollower.getFrontRange(),
            wallFollower.getRearRange());
        lcdWait = 0;
    }
}

void stepFSM(
        bool buttonStop,
        bool buttonGo,
        bool colorLeft,
        bool colorRight,
        bool distPrep,
        bool distSq1,
        bool distSq2,
        bool distSq3) {
    if (buttonStop)
        state = HALT;
    switch (state) {
        case WAIT:
            if (buttonGo)
                state = PREP_REVERSE;
            break;
        case PREP_REVERSE:
            if (distPrep)
                state = PREP_ROTATE;
            break;
        case PREP_ROTATE:
            state = SQ1_MOVE;
            break;
        case SQ1_MOVE:
            if (distSq1) {
                if (colorLeft)
                    state = SQ1_DEPLOY_LEFT;
                else if (colorRight)
                    state = SQ1_DEPLOY_RIGHT;
                else
                    state = SQ2_MOVE;
            }
            break;
        case SQ1_DEPLOY_LEFT:
            if (colorRight)
                state = SQ1_DEPLOY_RIGHT;
            else
                state = SQ2_MOVE;
            break;
        case SQ1_DEPLOY_RIGHT:
            state = SQ2_MOVE;
            break;
        case SQ2_MOVE:
            if (distSq2) {
                if (colorRight)
                    state =    SQ2_DEPLOY;
                else
                    state = SQ3_MOVE;
            }
            break;
        case SQ2_DEPLOY:
            state = SQ3_MOVE;
            break;
        case SQ3_MOVE:
            if (distSq3) {
                if (count == 3)
                    state = FINISH_ROTATE;
                else if (colorRight)
                    state = SQ3_DEPLOY;
                else {
                    state = PREP_REVERSE;
                    count++;
                }
            }
            break;
        case SQ3_DEPLOY:
            state = PREP_REVERSE;
            count++;
            break;
        case FINISH_ROTATE:
            state = FINISH_MOVE;
            break;
        case FINISH_MOVE:
            if (distSq3)
                state = HALT;
            break;
    }
}

const char* stateToString(byte state) {
    switch (state) {
        case WAIT: return "WAIT";
        case PREP_REVERSE: return "PREP_REVERSE";
        case PREP_ROTATE: return "PREP_ROTATE";
        case SQ1_MOVE: return "SQ1_MOVE";
        case SQ1_DEPLOY_LEFT: return "SQ1_DEPLOY_LEFT";
        case SQ1_DEPLOY_RIGHT: return "SQ1_DEPLOY_RIGHT";
        case SQ2_MOVE: return "SQ2_MOVE";
        case SQ2_DEPLOY: return "SQ2_DEPLOY";
        case SQ3_MOVE: return "SQ3_MOVE";
        case SQ3_DEPLOY: return "SQ3_DEPLOY";
        case FINISH_ROTATE: return "FINISH_ROTATE";
        case FINISH_MOVE: return "FINISH_MOVE";
        case HALT: return "HALT";
    }
}
