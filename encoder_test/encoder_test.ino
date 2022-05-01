#define ENCA_LEFT 20
#define ENCB_LEFT 34
#define PWM_LEFT 8
#define IN1_LEFT 9
#define IN2_LEFT 10

#define ENCA_RIGHT 12
#define ENCB_RIGHT 13
#define PWM_RIGHT 9
#define IN1_RIGHT 7
#define IN2_RIGHT 8

volatile int pos_left = 0;
volatile int pos_right = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA_LEFT,INPUT);
  pinMode(ENCB_LEFT,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_LEFT),readEncoder_LEFT,RISING);
  pinMode(ENCA_RIGHT,INPUT);
  pinMode(ENCB_RIGHT,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA_RIGHT),readEncoder_RIGHT,RISING);
}

void loop() {
  // taget position
  int target_left = 1200;

  //PID constants
  float kp = 1;
  float ki = 0;
  float kd = 0;

  //time difference
  long currT = micros();

  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;

  //error
  int e = pos_left-target_left;

  //derivative
  float dedt = (e-eprev)/(deltaT);

  //integral
  eintegral = eintegral + e*deltaT;

  //control signal
  float u = kp*e+kd*dedt+ki*eintegral;

  //motor power 
  float pwr = fabs(u);
  if (pwr > 255){
    pwr = 255;
  }

  int dir = 1;
  if (u < 0){
    dir = -1;
  }

  setMotor(dir,pwr,PWM_LEFT,IN1_LEFT,IN2_LEFT);
  Serial.println(pos_left);
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if(dir == -1){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void readEncoder_LEFT(){
  int b = digitalRead(ENCB_LEFT);
  if(b>0){
    pos_left++;
  }
  else{
    pos_left--;
  }
}

void readEncoder_RIGHT(){
  int b = digitalRead(ENCB_RIGHT);
  if(b>0){
    pos_right++;
  }
  else{
    pos_right--;
  }
}