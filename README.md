# Welcome to Arc reactor sample

This Arduino project contains a sample/demo application to drive LEDs in a 3D printed "custom case".

Everything in this fork is a work-in-progress...
 I haven't updated the fritzing file to reflect my changes
 I can't build a circuit board as well as takacs525 can, so I printed a base for mine (which is a bit oversized at this time)

Added in intensity control for the outer ring by wiring the shift register OE pin to a PWM capable pin on the arduino
 - Also split the outer ring into two sets of 5 pins to each shift register. This would enable you to use smaller resistors
Added in two buttons and didn't use the bluetooth module (although the code is still in the project and would work)
 
In motion :-)
-----------------
[![Youtube video](https://www.youtube.com/watch?v=bBRC_kys7CE)](https://www.youtube.com/watch?v=bBRC_kys7CE)

File structure
-----------------
Directories:
* Arc_reactor: the Arduino project files
* Pictures
* Schematic: fritzing file folder

Hardware
-----------------
Required components:
* an Arduino Pro Mini 5V
* 2 SN74HC595 shift registers
* LEDs
* Resistors (470 ohm)
* a Bluetooth module (HC-07) (Optional)
* Two buttons

Software
-----------------
To send mode switch commands (key '1' to '4') you can use the serial monitor in the Arduino IDE or any Bluetooth mobile application (like [Arduino Bluetooth Control](https://play.google.com/store/apps/details?id=com.broxcode.arduinobluetoothfree) in Google Play Store).

3D model
-----------------
Thanks to takacs525 for his initial work
Special thanks to Gyorgy Balassy for his incredible work. :clap:
You can find more informatin on his Thingiverse page: [Tony Stark's Bluetooth Controlled Arc Reactor](http://www.thingiverse.com/thing:2069812)

