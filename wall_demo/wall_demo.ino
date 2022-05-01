// pins
#define PIN_MOTOR_LEFT_EN (13)
#define PIN_MOTOR_LEFT_A (11)
#define PIN_MOTOR_LEFT_B (12)

#define PIN_MOTOR_RIGHT_EN (8)
#define PIN_MOTOR_RIGHT_A (10)
#define PIN_MOTOR_RIGHT_B (9)

#define PIN_IR_FRONT_TRIG (4)
#define PIN_IR_FRONT_ECHO (2)
#define PIN_IR_REAR_TRIG (5)
#define PIN_IR_REAR_ECHO (3)

#define MOTOR_SPEED (127)
#define IR_BUFFER_SIZE (5)

int irFrontDesired;
int irBackDesired;

void setup() {
  pinMode(PIN_MOTOR_LEFT_EN, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_A, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_B, OUTPUT);

  pinMode(PIN_MOTOR_RIGHT_EN, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_A, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_B, OUTPUT);

  pinMode(PIN_IR_FRONT_TRIG, OUTPUT);
  pinMode(PIN_IR_REAR_TRIG, OUTPUT);

  // configure motors fwd, stopped
  analogWrite(PIN_MOTOR_LEFT_EN, 0);
  analogWrite(PIN_MOTOR_RIGHT_EN, 0);

  digitalWrite(PIN_MOTOR_LEFT_A, HIGH);
  digitalWrite(PIN_MOTOR_LEFT_B, LOW);
  digitalWrite(PIN_MOTOR_RIGHT_A, LOW);
  digitalWrite(PIN_MOTOR_RIGHT_B, HIGH);

  // calibrate IR
  irCalibrate();

  // populate buffers
  for (int i = 0; i < IR_BUFFER_SIZE; i++) {
    getFrontError();
    getBackError();
  }
}

void loop() {
  // constants
  float alpha = 1.5;
  float beta = 0.25;
  int nom_speed = 50;

  // calculate direct error
  int frontError = getFrontError();
  int backError = getBackError();

  // calculate indirect error
  int diffError = frontError - backError;
  int commError = frontError + backError;

  // control law
  int leftSpeed = (int) (nom_speed + alpha*diffError + beta*commError);
  int rightSpeed = (int) (nom_speed - alpha*diffError - beta*commError);

  // send to motors
  analogWrite(PIN_MOTOR_LEFT_EN, leftSpeed);
  analogWrite(PIN_MOTOR_RIGHT_EN, rightSpeed);

  // fix frequency
  // delay(50);
}

int getFrontError() {
  static int buffer[IR_BUFFER_SIZE] = {0};
  static int i = 0;

  // read
  buffer[i++] = readFront();
  if (i == IR_BUFFER_SIZE) {
    i = 0;
  }

  // get avg
  int average = 0;
  for (int j = 0; j < IR_BUFFER_SIZE; j++) {
    average += buffer[j];
  }
  average /= IR_BUFFER_SIZE;

  // get error
  return irFrontDesired - average;
}

int getBackError() {
  static int buffer[IR_BUFFER_SIZE] = {0};
  static int i = 0;

  // read
  buffer[i++] = readRear();
  if (i == IR_BUFFER_SIZE) {
    i = 0;
  }

  // get avg
  int average = 0;
  for (int j = 0; j < IR_BUFFER_SIZE; j++) {
    average += buffer[j];
  }
  average /= IR_BUFFER_SIZE;

  // get error
  return irBackDesired - average;
}

void irCalibrate() {
  const int samples = 10;
  long frontBuffer = 0;
  long backBuffer = 0;
  for (int i = 0; i < samples; i++) {
    frontBuffer += readFront();
    backBuffer += readRear();
    delay(100);
  }
  irFrontDesired = (int) (frontBuffer / samples);
  irBackDesired = (int) (backBuffer / samples);
}

int readFront() {
  digitalWrite(PIN_IR_FRONT_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_IR_FRONT_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_IR_FRONT_TRIG, LOW);
  return pulseIn(PIN_IR_FRONT_ECHO, HIGH);
}

int readRear() {
    digitalWrite(PIN_IR_REAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_IR_REAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_IR_REAR_TRIG, LOW);
  return pulseIn(PIN_IR_REAR_ECHO, HIGH);
}