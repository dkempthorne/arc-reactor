// Serial Debug
#define DEBUG_OUTPUT         0
#define DEBUG_DELAY          1000

// LEDbar with 2x 74HC595
#define LEDBAR_DATA_PIN      2
#define LEDBAR_LATCH_PIN     8   // WAS PIN 3
#define LEDBAR_CLOCK_PIN     4
#define LEDBAR_BARS         16
#define LEDBAR_OE_PIN        3   // Must be PWM capable
                                 // Uno/Mini Pro Pins: 3, 5, 6, 9, 10, 11

// Center circuit leds
#define ILED0_PIN            9

// Inner circuit leds
#define ILED1_PIN            5
#define ILED2_PIN           10
#define ILED3_PIN           11
#define ILED4_PIN            6

// Button PINs
#define BUTTON1_PIN         12
#define BUTTON2_PIN         13

// Startup Delay
#define STARTUP_DELAY       250
#define STARTUP_ANIMATION   1     // 0 = OFF ; 1 = ON


