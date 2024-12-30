/*
 An example showing rainbow colours on a 3.0 or 3.2" TFT LCD screen
 and to show basic examples of font use.

 This sketch uses the GLCD, 2, 4, 6 fonts only.

 Make sure all the required fonts are loaded by editing the
 User_Setup.h file in the TFT_eSPI library folder.


  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######           TO SELECT THE FONTS AND PINS YOU USE              ######
  #########################################################################

  to-do:
  add more info like forecast from web
  add a clock
  make the data available online
  
 */

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Wire.h"
#include "SHT2x.h"
#include "Adafruit_ADT7410.h"

uint32_t start;
uint32_t stop;

SHT2x sht;

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

unsigned long targetTime = 0;
byte red = 31;
byte green = 0;
byte blue = 0;
byte state = 0;
unsigned int colour = red << 11; // Colour order is RGB 5+6+5 bits each

void setup(void) {
  Wire.begin();
  sht.begin();

  tempsensor.setResolution(ADT7410_16BIT);

  if (!tempsensor.begin()) {
    Serial.println("Couldn't find ADT7410!");
    while (1);
  }
  
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);

  targetTime = millis() + 1000;
}

void loop() {


  if (targetTime < millis()) {
    targetTime = millis() + 10000;

    sht.read();
    float c = tempsensor.readTempC();

    rainbow_fill(); // Fill the screen with rainbow colours
    tft.setTextFont(4);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); // This time we will use green text on a black background

    tft.setCursor (5, 10);
    tft.print("Temp1: ");
    tft.drawCentreString(String(sht.getTemperature()), 160, 50, 8);       // Draw text centre at position 120, 54 using font 6

    tft.setCursor (5, 150);
    tft.print("Temp2: ");
    tft.drawCentreString(String(c), 160, 190, 8);       // Draw text centre at position 120, 54 using font 6

    tft.setCursor (5, 330);
    tft.print("Humidity: ");
    tft.drawCentreString(String(sht.getHumidity()), 160, 370, 8);       // Draw text centre at position 120, 54 using font 6
  }
}

// Fill screen with a rainbow pattern
void rainbow_fill()
{
  // The colours and state are not initialised so the start colour changes each time the function is called
  
  for (int i = 479; i > 0; i--) {
    // Draw a vertical line 1 pixel wide in the selected colour
    tft.drawFastHLine(0, i, tft.width(), colour); // in this example tft.width() returns the pixel width of the display
    // This is a "state machine" that ramps up/down the colour brightnesses in sequence
    switch (state) {
      case 0:
        green ++;
        if (green == 64) {
          green = 63;
          state = 1;
        }
        break;
      case 1:
        red--;
        if (red == 255) {
          red = 0;
          state = 2;
        }
        break;
      case 2:
        blue ++;
        if (blue == 32) {
          blue = 31;
          state = 3;
        }
        break;
      case 3:
        green --;
        if (green == 255) {
          green = 0;
          state = 4;
        }
        break;
      case 4:
        red ++;
        if (red == 32) {
          red = 31;
          state = 5;
        }
        break;
      case 5:
        blue --;
        if (blue == 255) {
          blue = 0;
          state = 0;
        }
        break;
    }
    colour = red << 11 | green << 5 | blue;
  }
}
