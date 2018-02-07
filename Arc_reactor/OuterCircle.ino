// Module variables
int OuterMode = 1;              //Start mode: all on
int OuterLEDCount = 10;

// Mode variables
int OM2_Position = 1;           // Current LED position
int OM3_Position = 1;           // Current LED position
int OM4_Position = 1;           // Current LED position
int OM5_Position = 1;           // Current LED position

byte OM3_Delay = 15;            // Current speed
byte OM3_SpeedChange = 1;       // Speed up or down


// Setup
void SetupOuterCircle() {
  pinMode(LEDBAR_LATCH_PIN, OUTPUT);
  pinMode(LEDBAR_CLOCK_PIN, OUTPUT);
  pinMode(LEDBAR_DATA_PIN, OUTPUT);
  
  pinMode(LEDBAR_OE_PIN, OUTPUT);
  analogWrite(LEDBAR_OE_PIN, 0);

  ResetOuterCircle();
}

void ResetOuterCircle() {
  LEDint(0);
}

void UpOuterCircle() {
  LEDbar(10);
}

void LoopUpOuterCircle() {
  ResetOuterCircle();
  delay(250);
  analogWrite(LEDBAR_OE_PIN, 0);
  for (int numloop = 0; numloop < 10; numloop++) {
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress2[numloop]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress1[numloop]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
    delay(500);
  }
}

void DoubleLoopUpOuterCircle() {
  ResetOuterCircle();
  delay(250);
  for (int numloop = 0; numloop < 10; numloop++) {
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[numloop]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[numloop]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
    delay(500);
  }
}
//-------------------------------------------------------------------------------

void LEDint(int numberToDisplay) {     
  digitalWrite(LEDBAR_LATCH_PIN, LOW);

  shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, (numberToDisplay >> 8));  
  shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, numberToDisplay);  
  // Serial.println((numberToDisplay >> 8));
  // Serial.println(numberToDisplay);

  digitalWrite(LEDBAR_LATCH_PIN, HIGH);  
}

void LEDbar(int bar) {
  int numberToDisplay = 0xFFFF >> (LEDBAR_BARS - bar);
     
  digitalWrite(LEDBAR_LATCH_PIN, LOW);

  shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, numberToDisplay);  
  shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, numberToDisplay);  
  
  digitalWrite(LEDBAR_LATCH_PIN, HIGH);  
}

void ModeChangeOuter(int change) {
  if (change == 1) {
    OuterMode = OuterMode + 1;
    // OLD Stop code: if (OuterMode > 3) { OuterMode = 3; }
    if (OuterMode > 6) { OuterMode = 0; }
  } else {
    OuterMode = OuterMode - 1;
    // OLD Stop code: if (OuterMode < 0) { OuterMode = 0; }
    if (OuterMode < 0) { OuterMode = 6; }      
  }

/*
 * Outer modes:
 *  0: Off
 *  1: On
 *  2: Circular
 *  3: Circular+ (Accelerating)
 *  4: Double LED Circular
 *  5: Double LED Circular+ (Accelerating)
 *  6: Pulse
 */
  // reset brightness
  analogWrite(LEDBAR_OE_PIN, 0);

  switch (OuterMode) {
    case 0:
      Serial.println("Outer Circle: Off");
      ResetOuterCircle();
      break;
    case 1:
      Serial.println("Outer Circle: On");
      UpOuterCircle();
      break;
    case 2:
      Serial.println("Outer Circle: Circular");
      OM2_Position = 1;         // Current LED position
      break;
    case 3:
      Serial.println("Outer Circle: Circular+");
      {
        OM3_Position = 1;       // Current LED position
        OM3_Delay = 15;         // Current speed
        OM3_SpeedChange = 1;    // Speed change direction
      }     
      break;
    case 4:
      Serial.println("Outer Circle: Double Circular");
      OM4_Position = 1;         // Current LED position
      break;
    case 5:
      Serial.println("Outer Circle: Double Circular+");
      OM5_Position = 1;         // Current LED position
      break;
    case 6:
      {
        Serial.println("Outer Circle: Pulse");
        // Enable all the LEDs first
        UpOuterCircle();
      }
      break;
  } 
}

void LoopOuter() {
  switch (OuterMode) {
    case 2:
      LoopOuterMode2();
      break;
    case 3:
      LoopOuterMode3();
      break;
    case 4:
      LoopOuterMode4();
      break;
    case 5:
      LoopOuterMode5();
      break;
    case 6:
      LoopOuterMode6();
      break;
  }  
}

// Circle Accelerating
void LoopOuterMode2() {
  if (Tick % 12 == 0) {
    OM2_Position++;
    if (OM2_Position == OuterLEDCount) { OM2_Position = 0; }
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress2[OM2_Position]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress1[OM2_Position]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
  }
}

// Circle+ Accelerating
void LoopOuterMode3() {
  if (Tick % OM3_Delay == 0) {
    //LEDint(OM3_Position);
    //OM3_Position = OM3_Position * 2;
    //if (OM3_Position > 512) { OM3_Position = 1; }

    OM3_Position++;
    if (OM3_Position == OuterLEDCount) { OM3_Position = 0; }
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress2[OM3_Position]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress1[OM3_Position]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
  }  

  if (Tick % 200 == 0) {
    if (OM3_SpeedChange == 1) {
      OM3_Delay--;
      if (OM3_Delay == 1 ) { OM3_SpeedChange = -1; }
    } else {
      OM3_Delay++;
      if (OM3_Delay == 15 ) { OM3_SpeedChange = 1; }
    }
  }  
}

// Double Circle
void LoopOuterMode4() {
  if (Tick % 12 == 0) {
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    OM4_Position++;
    if (OM4_Position == OuterLEDCount) { OM4_Position = 0; }

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[OM4_Position]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[OM4_Position]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
  }
}

// Double Circle+ Accelerating
void LoopOuterMode5() {
  if (Tick % OM3_Delay == 0) {

    OM5_Position++;
    if (OM5_Position == OuterLEDCount) { OM5_Position = 0; }
    digitalWrite(LEDBAR_LATCH_PIN, LOW);

    // Note that the stream occurs backwards
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[OM5_Position]);  
    shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[OM5_Position]);  

    digitalWrite(LEDBAR_LATCH_PIN, HIGH);
  }  

  if (Tick % 200 == 0) {
    if (OM3_SpeedChange == 1) {
      OM3_Delay--;
      if (OM3_Delay == 1 ) { OM3_SpeedChange = -1; }
    } else {
      OM3_Delay++;
      if (OM3_Delay == 15 ) { OM3_SpeedChange = 1; }
    }
  }  
}

// Pulse inner circle
void LoopOuterMode6() {
  if (Tick % 3 == 0) {
    analogWrite(LEDBAR_OE_PIN, Outer_Brightness);
  }  
}


