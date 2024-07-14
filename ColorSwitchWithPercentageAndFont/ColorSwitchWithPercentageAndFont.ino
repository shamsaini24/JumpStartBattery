#include <TFT_eSPI.h> // Include the graphics library
#include "CST816S.h"  // Include the CST816S touch library

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display
CST816S touch(6, 7, 13, 5); // sda, scl, rst, irq

int currentColor = 0;   // 0 = green, 1 = yellow, 2 = red
bool isTouched = false; // To keep track of touch status
int percentage = 35;
bool flash = false;
void setup()
{
  Serial.begin(115200);
  touch.begin(); // Initialize the touch screen controller

  tft.init();         // Initialize the display
  tft.setRotation(0); // Set display orientation, adjust if necessary

  // Set text font and size
  tft.setFreeFont(&FreeSansBold24pt7b);
  tft.setTextSize(3);
  TFT_SET_BL(50);

  changeColor();
}

void loop()
{
  if (touch.available() && !isTouched)
  {
    isTouched = true; // Set touch status to true
    // Get the touch data
    int x = touch.data.x;
    int y = touch.data.y;

    if (x < 100)
    {
      Serial.println("Touch detected left \r\n");
      if (percentage > 0)
        percentage--;
    }

    if (x > 140)
    {
      Serial.println("Touch detected right \r\n");
      if (percentage < 100)
        percentage++;
    }

    changeColor();
    delay(200); // Add delay to avoid rapid color changes
  }
  else if (!touch.available() && isTouched)
  {
    isTouched = false; // Reset touch status when touch is released
  }
  if (percentage < 11)
  {
    flashRed();
  }
}

void changeColor()
{

  if (percentage > 60)
  {
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
  }
  else if (percentage > 30 && percentage <= 60)
  {
    tft.fillScreen(0xFA00);
    tft.setTextColor(TFT_BLACK, 0xFA00);
  }
  else if (percentage > 10 && percentage <= 30)
  {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_BLACK, TFT_RED);
  }

  drawText();
}

void drawText()
{
  // Define the text to be displayed
  String text = String(percentage);

  // Set the text datum to the center
  tft.setTextDatum(MC_DATUM);
  tft.setTextPadding(tft.width());
  // Draw the text in the center of the screen
  tft.drawString(text, tft.width() / 2, tft.height() / 2);
}

void flashRed()
{
  Serial.println("SHOULD FLASH HERE \r\n");

  if (flash)
  {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_BLACK, TFT_RED);
    flash = false;
  }
  else
  {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    flash = true;
  }
  drawText();
  delay(500);
}

void TFT_SET_BL(uint8_t Value)
{
  if (Value < 0 || Value > 100)
  {
    printf("TFT_SET_BL Error \r\n");
  }
  else
  {
    analogWrite(TFT_BL, Value * 2.55);
  }
}