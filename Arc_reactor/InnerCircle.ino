// Note: 'Core' refers to the LED at the very center
//       'Inner Circle' refers to the 8 LEDs in pairs (4 Pins out) around the 'Core'

// Module variables
int InnerPins[] = {ILED0_PIN, ILED1_PIN, ILED2_PIN, ILED3_PIN, ILED4_PIN};
int InnerMode = 1;               //Start mode: all on

// Mode variables
int IM3_Position = 1;           // Current LED position

// Setup
void SetupInnerCircle() {
  for (int i = 0; i < 5; i++) {
    pinMode(InnerPins[i], OUTPUT);
  }
  ResetInnerCircle();
}

void ResetInnerCircle() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(InnerPins[i], LOW);
  }
}

void UpInnerCircle() {
  for (int i = 1; i < 5; i++) {
    digitalWrite(InnerPins[i], HIGH);
  }
  // Reset Brightness
  for (int i = 0; i < 5; i++) {
    analogWrite(InnerPins[i], 255);
  }
}

void UpCoreCircle() {
  digitalWrite(InnerPins[0], HIGH);
  // Reset Brightness
  for (int i = 0; i < 5; i++) {
    analogWrite(InnerPins[i], 255);
  }
}

//-------------------------------------------------------------------------------

void ModeChangeInner(int change) {
  if (change == 1) {
    InnerMode = InnerMode + 1;
    // OLD Stop code: if (InnerMode > 3) { InnerMode = 3; }
    if (InnerMode > 3) { InnerMode = 0; }
  } else {
    InnerMode = InnerMode - 1;
    // OLD Stop code: if (InnerMode < 0) { InnerMode = 0; }
    if (InnerMode < 0) { InnerMode = 3; }
  }

/*
 * Outer modes:
 *  0: Off
 *  1: On
 *  2: Pulse
 *  3: Circular
 */
  switch (InnerMode) {
    case 0:
      Serial.println("Inner Circle: off");
      ResetInnerCircle();
      break;
    case 1:
      Serial.println("Core Circle: on");
      UpCoreCircle();
      Serial.println("Inner Circle: on");
      UpInnerCircle();
      break;
    case 2:
      {
        Serial.println("Core Circle: Pulse");
        //UpCoreCircle();
        //UpInnerCircle();
      }
      break;
    case 3:
      {
        Serial.println("Core Circle: Circle");
        ResetInnerCircle();
        digitalWrite(ILED0_PIN, HIGH);
        digitalWrite(ILED1_PIN, HIGH);
        IM3_Position = 1;      // Current LED position
      }
      break;
  }
}

void LoopInner() {
  switch (InnerMode) {
    case 2:
      LoopInnerMode2();
      break;
    case 3:
      LoopInnerMode3();
      break;
  }
}

// Pulse Core & Inner circle
void LoopInnerMode2() {
  if (Tick % 3 == 0) {
    for (int i = 0; i < 5; i++) {
      analogWrite(InnerPins[i], Inner_Brightness);
    }
  }  
}

// Circular (inner circle)
void LoopInnerMode3() {
  if (Tick % 20 == 0) {
    digitalWrite(InnerPins[IM3_Position], LOW);
    
    IM3_Position++;
    if (IM3_Position > 4) { IM3_Position = 1; }

    digitalWrite(InnerPins[IM3_Position], HIGH);
  }  
}


