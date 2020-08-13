//-------------------------------------------------------------------------------
//  TinyCircuits TinyScreen/TinyScreen+ Basic Button Interaction Example
//  
//  This example shows the basic functionality of the TinyScreen/TinyScreen+
//  buttons and how to interact with them.
//
//  Written by Laveréna Wienclaw August 13 2020
//-------------------------------------------------------------------------------

#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>

/* Library must be initialized with the board type
 * TinyScreenDefault for TinyScreen shields
 * TinyScreenAlternate for alternate address TinyScreen shields
 * TinyScreenPlus for TinyScreen+    */
TinyScreen display = TinyScreen(TinyScreenPlus);

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif


void setup(void) {
  Wire.begin(); //initialize I2C before we can initialize TinyScreen, not needed for TinyScreen+
  SerialMonitorInterface.begin(9600);
  display.begin();
  display.setBrightness(10); //sets main current level, valid levels are 0-15
  display.clearScreen();
  display.setFont(thinPixel7_10ptFontInfo);
}

void loop() {
  buttonLoop();
  delay(100); // make screen look more stable between text printing and clearing
  display.clearScreen();
}

/*
  - getButtons() function can be used to test if any button is pressed
  - getButtons(TSButtonUpperLeft) to test a particular button
  - getButtons(TSButtonUpperLeft|TSButtonUpperRight) to test multiple buttons
  - Results are flipped as you would expect when setFlip(true) is used
*/
void buttonLoop() {
  display.setCursor(0, 0);

  if (display.getButtons(TSButtonUpperLeft)) {
    SerialMonitorInterface.println("upper left pressed");
    display.println("Pressed!");
  } 
  
  display.setCursor(0, 54);
  if (display.getButtons(TSButtonLowerLeft)) {
    SerialMonitorInterface.println("lower left pressed");
    display.println("Pressed!");
  } 
  
  display.setCursor(95 - display.getPrintWidth("Pressed!"), 0);
  if (display.getButtons(TSButtonUpperRight)) {
    SerialMonitorInterface.println("upper right pressed");
    display.println("Pressed!");
  } 
  
  display.setCursor(95 - display.getPrintWidth("Pressed!"), 54);
  if (display.getButtons(TSButtonLowerRight)) {
    SerialMonitorInterface.println("lower right pressed");
    display.println("Pressed!");
  } 
}
