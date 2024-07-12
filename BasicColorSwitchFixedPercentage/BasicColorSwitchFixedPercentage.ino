#include <TFT_eSPI.h>      // Include the graphics library
#include "CST816S.h"       // Include the CST816S touch library

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display
CST816S touch(6, 7, 13, 5); // sda, scl, rst, irq

int currentColor = 0;       // 0 = green, 1 = yellow, 2 = red
bool isTouched = false;     // To keep track of touch status

void setup() {
  Serial.begin(115200);
  touch.begin();             // Initialize the touch screen controller

  tft.init();                // Initialize the display
  tft.setRotation(0);        // Set display orientation, adjust if necessary
  tft.fillScreen(TFT_GREEN); // Start with green screen

  // Set text color and background color
  tft.setTextColor(TFT_WHITE, TFT_GREEN);

  // Set text font and size
  tft.setFreeFont(&FreeSansBold18pt7b);

  drawText();
}

void loop() {
  if (touch.available() && !isTouched) {
    isTouched = true;          // Set touch status to true
    // Get the touch data
    int x = touch.data.x;
    int y = touch.data.y;
    Serial.print("X = "); Serial.print(x);
    Serial.print("\tY = "); Serial.println(y);
    changeColor();
    delay(500); // Add delay to avoid rapid color changes

  } else if (!touch.available() && isTouched) {
    isTouched = false;         // Reset touch status when touch is released
  }
}

void changeColor() {
  currentColor = (currentColor + 1) % 3; // Cycle through 0, 1, 2

  switch (currentColor) {
    case 0:
      tft.fillScreen(TFT_GREEN);
      tft.setTextColor(TFT_WHITE, TFT_GREEN);
      break;
    case 1:
      tft.fillScreen(TFT_YELLOW);
      tft.setTextColor(TFT_BLACK, TFT_YELLOW);
      break;
    case 2:
      tft.fillScreen(TFT_RED);
      tft.setTextColor(TFT_WHITE, TFT_RED);
      break;
  }

  drawText();
}

void drawText() {
  // Define the text to be displayed
  String text = "87%";

  // Set the text datum to the center
  tft.setTextDatum(MC_DATUM);

  // Draw the text in the center of the screen
  tft.drawString(text, tft.width() / 2, tft.height() / 2);
}