// FSM
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

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  // read inputs from serial
  while (Serial.available() < 10);
  const bool BTN_STOP = Serial.read() == '1';
  const bool BTN_GO = Serial.read() == '1';
  const bool COLOR_LEFT = Serial.read() == '1';
  const bool COLOR_RIGHT = Serial.read() == '1';
  Serial.read();  // eat space
  const bool DIST_PREP = Serial.read() == '1';
  const bool DIST_SQ1 = Serial.read() == '1';
  const bool DIST_SQ2 = Serial.read() == '1';
  const bool DIST_SQ3 = Serial.read() == '1';
  const bool COUNTER = count == 3;
  Serial.read(); // eat newline
  
  // run FSM
  if (BTN_STOP)
    state = HALT;
  switch (state) {
    case WAIT:
      if (BTN_GO)
        state = PREP_REVERSE;
      break;
    case PREP_REVERSE:
      if (DIST_PREP)
        state = PREP_ROTATE;
      break;
    case PREP_ROTATE:
      state = SQ1_MOVE;
      break;
    case SQ1_MOVE:
      if (DIST_SQ1) {
        if (COLOR_LEFT)
          state = SQ1_DEPLOY_LEFT;
        else if (COLOR_RIGHT)
          state = SQ1_DEPLOY_RIGHT;
        else
          state = SQ2_MOVE;
      }
      break;
    case SQ1_DEPLOY_LEFT:
      if (COLOR_RIGHT)
        state = SQ1_DEPLOY_RIGHT;
      else
        state = SQ2_MOVE;
      break;
    case SQ1_DEPLOY_RIGHT:
      state = SQ2_MOVE;
      break;
    case SQ2_MOVE:
      if (DIST_SQ2) {
        if (COLOR_RIGHT)
          state =  SQ2_DEPLOY;
        else
          state = SQ3_MOVE;
      }    
      break;
    case SQ2_DEPLOY:
      state = SQ3_MOVE;
      break;
    case SQ3_MOVE:
      if (DIST_SQ3) {
        if (COUNTER)
          state = FINISH_ROTATE;
        else if (COLOR_RIGHT)
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
      if (DIST_SQ3)
        state = HALT;
      break;
  }

  // get outputs
  const bool MOVE_REVERSE = state == PREP_REVERSE;
  const bool MOVE_FORWARD = state == SQ1_MOVE || state == SQ2_MOVE || state == SQ3_MOVE || state == FINISH_MOVE;
  const bool ROTATE_LEFT = state == PREP_ROTATE;
  const bool ROTATE_RIGHT = state == FINISH_ROTATE;
  const bool DEPLOY_LEFT = state == SQ1_DEPLOY_LEFT;
  const bool DEPLOY_RIGHT = state == SQ1_DEPLOY_RIGHT || state == SQ2_DEPLOY || state == SQ3_DEPLOY;
  
  // write outputs to serial
  Serial.print("Inputs: ");
  Serial.print(BTN_STOP ? "1" : "0");
  Serial.print(BTN_GO ? "1" : "0");
  Serial.print(COLOR_LEFT ? "1" : "0");
  Serial.print(COLOR_RIGHT ? "1" : "0");
  Serial.print(" ");
  Serial.print(DIST_PREP ? "1" : "0");
  Serial.print(DIST_SQ1 ? "1" : "0");
  Serial.print(DIST_SQ2 ? "1" : "0");
  Serial.print(DIST_SQ3 ? "1" : "0");
  Serial.print(" ");
  Serial.println(COUNTER ? "1" : "0");

  Serial.print("Current state: ");
  Serial.println(state);

  Serial.print("Current outputs: ");
  Serial.print(MOVE_REVERSE ? "1" : "0");
  Serial.print(MOVE_FORWARD ? "1" : "0");
  Serial.print(ROTATE_LEFT ? "1" : "0");
  Serial.print(ROTATE_RIGHT ? "1" : "0");
  Serial.print(" ");
  Serial.print(DEPLOY_LEFT ? "1" : "0");
  Serial.println(DEPLOY_RIGHT ? "1" : "0");
  Serial.println();
}
