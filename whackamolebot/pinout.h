#pragma once

// This is the only file that should have pinouts
// Puts everything in one place so we can change it easily

// motors
#define PIN_MOTOR_LEFT_EN (13)
#define PIN_MOTOR_LEFT_A (12)
#define PIN_MOTOR_LEFT_B (11)

#define PIN_MOTOR_RIGHT_EN (8)
#define PIN_MOTOR_RIGHT_A (10)
#define PIN_MOTOR_RIGHT_B (9)

#define PIN_MOTOR_RIGHT_ENC (18)
#define PIN_MOTOR_LEFT_ENC (19)

#define PIN_SERVO (7)

// sonar
#define PIN_SONAR_RIGHT_FRONT_TRIG (4)
#define PIN_SONAR_RIGHT_FRONT_ECHO (2)
#define PIN_SONAR_RIGHT_REAR_TRIG (5)
#define PIN_SONAR_RIGHT_REAR_ECHO (3)
#define PIN_SONAR_FRONT (17)

// user interface
#define PIN_POTENTIOMETER (A0)
#define PIN_PIXELS (6)
#define PIN_ESTOP (50)
#define PIN_GO (52)

// pixels
#define PIXEL_STATE_INDEX (0)
#define PIXEL_COLOR_INDEX (1)
#define PIXEL_COUNT (2)

// I2C Multiplexer
#define I2C_MUX_LCD (0)
#define I2C_MUX_COLOR_LEFT (2)
#define I2C_MUX_COLOR_RIGHT (1)
