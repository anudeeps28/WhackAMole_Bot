#include "pinout.h"
#include "WallFollower.h"

volatile unsigned long WallFollower::frontMicros;
volatile unsigned long WallFollower::rearMicros;

WallFollower::WallFollower() {
    pinMode(PIN_SONAR_RIGHT_FRONT_TRIG, OUTPUT);
    pinMode(PIN_SONAR_RIGHT_REAR_TRIG, OUTPUT);
    pinMode(PIN_SONAR_FRONT, OUTPUT);

    digitalWrite(PIN_SONAR_RIGHT_FRONT_ECHO, LOW);
    digitalWrite(PIN_SONAR_RIGHT_REAR_ECHO, LOW);
    digitalWrite(PIN_SONAR_FRONT, LOW);
    delayMicroseconds(2);

    attachInterrupt(digitalPinToInterrupt(PIN_SONAR_RIGHT_FRONT_ECHO), frontCb, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SONAR_RIGHT_REAR_ECHO), rearCb, CHANGE);
}

void WallFollower::calibrate() {
    // speed of sound is about 0.034 cm/µs
    // max range is 450 cm
    // max travel time is 13235 uS
    // max pulse time is double that: 26470 uS => ~ 26 mS
    // max frequency should then be 20 Hz to give good FoS

    unsigned long frontSum = 0;
    unsigned long rearSum = 0;
    const int n_samples = 5;

    for (int i = 0; i < n_samples; i++) {
        sendPulse();
        delay(50);
        if (frontMicros > 30000) {
            Serial.print("Error: front right sonar has invalid distance of ");
            Serial.println(frontMicros);
        }
        if (rearMicros > 30000) {
            Serial.print("Error: rear right sonar has invalid distance of ");
            Serial.println(rearMicros);
        }
        frontSum += frontMicros;
        rearSum += rearMicros;
    }

    frontNominal = frontSum / n_samples;
    rearNominal = rearSum / n_samples;
}

byte WallFollower::getLeftMotorEffort() {
    // check that values are realistic
    if (frontMicros > 30000) {
        Serial.print("Error: front right sonar has invalid distance of ");
        Serial.println(frontMicros);
    }
    if (rearMicros > 30000) {
        Serial.print("Error: rear right sonar has invalid distance of ");
        Serial.println(rearMicros);
    }

    // direct error
    int frontError = frontMicros - frontNominal;
    int rearError = rearMicros - rearNominal;

    // indirect error
    int diffError = frontError - rearError;
    int commError = frontError + rearError;

    // control law
    return nominalSpeed + alpha*diffError + beta*commError;
}

byte WallFollower::getRightMotorEffort() {
    // check that values are realistic
    if (frontMicros > 30000) {
        Serial.print("Error: front right sonar has invalid distance of ");
        Serial.println(frontMicros);
    }
    if (rearMicros > 30000) {
        Serial.print("Error: rear right sonar has invalid distance of ");
        Serial.println(rearMicros);
    }

    // direct error
    int frontError = frontMicros - frontNominal;
    int rearError = rearMicros - rearNominal;

    // indirect error
    int diffError = frontError - rearError;
    int commError = frontError + rearError;

    // control law
    return nominalSpeed - alpha*diffError - beta*commError;
}

byte WallFollower::getFaceRange() {
    pinMode(PIN_SONAR_FRONT, OUTPUT);
    digitalWrite(PIN_SONAR_FRONT, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_FRONT, LOW);
    pinMode(PIN_SONAR_FRONT, INPUT);
    unsigned long faceMicros = pulseIn(PIN_SONAR_FRONT, HIGH, 30000);

    // convert to cm
    return faceMicros / 58;
}

byte WallFollower::getRearRange() {
    if (rearMicros > 30000) {
        Serial.print("Error: front sonar has invalid distance of ");
        Serial.println(rearMicros);
    }

    // convert to cm
    return rearMicros / 58;
}

byte WallFollower::getFrontRange() {
    if (frontMicros > 30000) {
        Serial.print("Error: front sonar has invalid distance of ");
        Serial.println(frontMicros);
    }

    // convert to cm
    return frontMicros / 58;
}

void WallFollower::sendPulse() {
    digitalWrite(PIN_SONAR_RIGHT_REAR_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_RIGHT_REAR_TRIG, LOW);

    delayMicroseconds(10);

    digitalWrite(PIN_SONAR_RIGHT_FRONT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_RIGHT_FRONT_TRIG, LOW);
}

void WallFollower::frontCb() {
    if (digitalRead(PIN_SONAR_RIGHT_FRONT_ECHO) == HIGH) {
        // record start of rising edge
        frontMicros = micros();
    } else {
        // record duration of high signal
        frontMicros = micros() - frontMicros;
    }
}

void WallFollower::rearCb() {
    if (digitalRead(PIN_SONAR_RIGHT_REAR_ECHO) == HIGH) {
        // record start of rising edge
        rearMicros = micros();
    } else {
        // record duration of high signal
        rearMicros = micros() - rearMicros;
    }
}
