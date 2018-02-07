// Global variables
unsigned long Tick = 0;

#include "./Config.h"

// Matrix of LED Addresses
//     LEDs 0-4 are on shift register #1
//    LEDs 5-10 are on shift register #2
// Both matrixes are use (in series) to push out which lights to power up

// Single LED loop sequence
int LEDAddress1[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
int LEDAddress2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x10};
// Double LED loop sequence
int LEDAddress3[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x01, 0x02, 0x04, 0x08, 0x10};
int LEDAddress4[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x01, 0x02, 0x04, 0x08, 0x10};

// Push button logic
// Two buttons
//   one controls the inside & core
//   the other the outer loop
int Button1LastValue = 0;
int Button1CurrentValue = 0;
int Button1CurrentValueDelayed = 0;
int Button2LastValue = 0;
int Button2CurrentValue = 0;
int Button2CurrentValueDelayed = 0;

// LED Brightness Fade/Pulse
int Inner_Brightness = 255;       // how bright the LEDs are
int Inner_FadeAmount = 5;         // how many points to fade the LED by
int Outer_Brightness = 0;         // how bright the LEDs are
int Outer_FadeAmount = 5;         // how many points to fade the LED by
int Inner_Animation_Brightness = 5;   // Startup Brightness
int Outer_Animation_Brightness = 250;   // Startup Brightness

void setup() {
	Serial.begin(9600);		      // Initialize serial communications with the PC or bluetooth
  if (DEBUG_OUTPUT == 1) { delay(DEBUG_DELAY); }  // Extra delay for any serial monitoring to open up
  
  Serial.println("ARC Reactor");
  Serial.println("Press a key:");
  Serial.println("1) Outer ring mode Cycle Up    (+1)");
  Serial.println("2) Outer ring mode Cycle Down  (-1)");
  Serial.println("3) Inner ring Mode Cycle up    (+1)");
  Serial.println("4) Inner ring Mode Cycle Down  (-1)");
  Serial.println("");
  Serial.println("... Standby! ARC Reactor coming online...");
  
  SetupInnerCircle();
  SetupOuterCircle();

  pinMode(BUTTON1_PIN, INPUT);    // sets the digital pin as input to read switch
  pinMode(BUTTON2_PIN, INPUT);    // sets the digital pin as input to read switch

  
  // Startup Animation
  if (STARTUP_ANIMATION == 1) {
    int InnerPins[] = {ILED0_PIN, ILED1_PIN, ILED2_PIN, ILED3_PIN, ILED4_PIN};

    delay(STARTUP_DELAY);
    Serial.println("Displaying Startup Animation");

    ResetOuterCircle();
    delay(STARTUP_DELAY);
    analogWrite(LEDBAR_OE_PIN, Outer_Animation_Brightness);

    // Loop outer once, one LED
    for (int numloop = 0; numloop < 10; numloop++) {
      digitalWrite(LEDBAR_LATCH_PIN, LOW);

      // Note that the stream occurs backwards
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress2[numloop]);  
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress1[numloop]);  

      if (DEBUG_OUTPUT == 1) {
        Serial.print("LEDBAR 2/1 == ");
        Serial.print(LEDAddress2[numloop]);
        Serial.print(":");
        Serial.println(LEDAddress1[numloop]);
        delay(DEBUG_DELAY);
      }

      digitalWrite(LEDBAR_LATCH_PIN, HIGH);
      Outer_Animation_Brightness -= 5;
      Inner_Animation_Brightness += 5;
      analogWrite(LEDBAR_OE_PIN, Outer_Animation_Brightness);
      delay(STARTUP_DELAY);
    }

    // Bring up the core circle
    analogWrite(InnerPins[0], Inner_Animation_Brightness);
    digitalWrite(InnerPins[0], HIGH);
    if (DEBUG_OUTPUT == 1) {
      Serial.println("InnerPin[0] enabled");
      delay(DEBUG_DELAY);
    }

    // Loop outer once, two LEDs
    for (int numloop = 0; numloop < 10; numloop++) {
      digitalWrite(LEDBAR_LATCH_PIN, LOW);

      // Note that the stream occurs backwards
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[numloop]);  
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[numloop]);  

      digitalWrite(LEDBAR_LATCH_PIN, HIGH);
      Outer_Animation_Brightness -= 5;
      Inner_Animation_Brightness += 5;
      analogWrite(LEDBAR_OE_PIN, Outer_Animation_Brightness);
      analogWrite(InnerPins[0], Inner_Animation_Brightness);
      delay(STARTUP_DELAY);
    }

    // Loop inner & outer once, two LEDs
    for (int numloop = 0; numloop < 10; numloop++) {

      // Bring up an inner circle
      analogWrite(InnerPins[int(numloop / 2)], Inner_Animation_Brightness);
      digitalWrite(InnerPins[int(numloop / 2)], HIGH);
      if (DEBUG_OUTPUT == 1) {
        Serial.print("InnerPin[");
        Serial.print(int(numloop / 2));
        Serial.println("] enabled");
        delay(DEBUG_DELAY);
      }
      digitalWrite(LEDBAR_LATCH_PIN, LOW);

      // Note that the stream occurs backwards
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[numloop]);  
      shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[numloop]);  

      digitalWrite(LEDBAR_LATCH_PIN, HIGH);
      Outer_Animation_Brightness -= 5;
      Inner_Animation_Brightness += 5;
      analogWrite(LEDBAR_OE_PIN, Outer_Animation_Brightness);
      for (int i = 0; i < int(numloop / 2); i++) {
        analogWrite(InnerPins[i], Inner_Animation_Brightness);
      }
      delay((STARTUP_DELAY - 50));
      // Turn off inner 
      digitalWrite(InnerPins[int(numloop / 2)], LOW);
    }

    // Inner circle all on
    for (int i = 1; i < 5; i++) {
      digitalWrite(InnerPins[i], HIGH);
    }
    
    // Accelerate to max, two LEDs
    int subdelay = STARTUP_DELAY - 50;
    // for (int subdelay = (STARTUP_DELAY - 50); subdelay > 10; subdelay -= 30) {
    while (subdelay > 10) {
      for (int numloop = 0; numloop < 10; numloop++) {
        digitalWrite(LEDBAR_LATCH_PIN, LOW);
  
        // Note that the stream occurs backwards
        shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[numloop]);  
        shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[numloop]);  
  
        digitalWrite(LEDBAR_LATCH_PIN, HIGH);
        Outer_Animation_Brightness -= 5;
        Inner_Animation_Brightness += 5;
        if (Outer_Animation_Brightness < 0) { Outer_Animation_Brightness = 0; }
        if (Inner_Animation_Brightness > 255) { Inner_Animation_Brightness = 255; }
        analogWrite(LEDBAR_OE_PIN, Outer_Animation_Brightness);
        for (int i = 0; i < 5; i++) {
          analogWrite(InnerPins[i], Inner_Animation_Brightness);
        }
        delay(subdelay);
      }
      subdelay = subdelay - (subdelay * .1);
    }

    // Maintain max for 1000, two LEDs
    // No pulse/fade lines needed here
    for (int max_speed_iter = 1; max_speed_iter < 10; max_speed_iter++) {
      for (int numloop = 0; numloop < 10; numloop++) {
        digitalWrite(LEDBAR_LATCH_PIN, LOW);
  
        // Note that the stream occurs backwards
        shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress4[numloop]);  
        shiftOut(LEDBAR_DATA_PIN, LEDBAR_CLOCK_PIN, MSBFIRST, LEDAddress3[numloop]);  
  
        digitalWrite(LEDBAR_LATCH_PIN, HIGH);
        delay(10);
      }
    }

    // Finished!
    UpInnerCircle();
    UpOuterCircle();

    Serial.println("Startup animation finished!");
  }
}

void loop() {
  Tick++;

  // Read Physical Push Buttons
  Button1CurrentValue = digitalRead(BUTTON1_PIN);
  Button2CurrentValue = digitalRead(BUTTON2_PIN);
  delay(10);
  Button1CurrentValueDelayed = digitalRead(BUTTON1_PIN);
  Button2CurrentValueDelayed = digitalRead(BUTTON2_PIN);

  // Check Button1
  if (Button1CurrentValue == Button1CurrentValueDelayed) {
    if (Button1CurrentValue == 1 && Button1LastValue == 0) {
      // Serial.println("Inner Core Power Demand Changed");
      ModeChangeInner(1);
      Button1LastValue = Button1CurrentValue;
    } else {
      Button1LastValue = Button1CurrentValue;
    }
  }

  // Check Button2
  if (Button2CurrentValue == Button2CurrentValueDelayed) {
    if (Button2CurrentValue == 1 && Button2LastValue == 0) {
      // Serial.println("Outer Core Power Demand Changed");
      ModeChangeOuter(1);
      Button2LastValue = Button2CurrentValue;
    } else {
      Button2LastValue = Button2CurrentValue;
    }
  }

  // Serial & Bluetooth Code
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte >=49 and inByte <= 52) {
      switch (inByte) {
        case 49:                      //1 -> Outer mode + 1
          ModeChangeOuter(1);
          break;
        case 50:                      //2 -> Outer mode - 1
          ModeChangeOuter(-1);
          break;
        case 51:                      //3 -> Inner mode + 1
          ModeChangeInner(1);
          break;
        case 52:                      //4 -> Inner mode - 1
          ModeChangeInner(-1);
          break;
        case 53:                      //5
          LoopUpOuterCircle();
          break;
        }
    }
  }

  // Every X ticks alter the brightness
  if (Tick % 4 == 0) {
    if (Inner_Brightness <= 0 || Inner_Brightness >= 255) {
      // Switch from fade in to fade out
      Inner_FadeAmount = -Inner_FadeAmount;
    }
    
    Inner_Brightness = Inner_Brightness + Inner_FadeAmount;
    Outer_Brightness = Outer_Brightness + Outer_FadeAmount;
    
    if (Outer_Brightness <= 0 || Outer_Brightness >= 255) {
      // Switch from fade in to fade out
      Outer_FadeAmount = -Outer_FadeAmount;
    }
  }

  LoopInner();
  LoopOuter();

  // delay(10); // already delayed 10 for the button anti-jitter
  
}



