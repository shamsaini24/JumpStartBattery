#include <TFT_eSPI.h>         // Include the graphics library
#include "CST816S.h"          // Include the CST816S touch library
#include "CyborgPunk50pt7b.h" // Include the custom font

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the display
CST816S touch(6, 7, 13, 5); // sda, scl, rst, irq

// User-friendly constants for easy adjustments
const int STARTING_PERCENTAGE = 1;             // Starting percentage
const int TARGET_PERCENTAGE = 23;                // Target percentage for animation
const int BRIGHTNESS_INITIAL = 50;              // Initial brightness level
const unsigned long ANIMATION_DELAY_MS = 3000;  // Delay before starting the animation in milliseconds
const unsigned long STEP_DELAY_MS = 1000;         // Delay for each step in the animation in milliseconds
const unsigned long COUNTDOWN_STEP_MS = 1000;   // Duration for each countdown step in milliseconds

bool isTouched = false;               // To keep track of touch status
int percentage = STARTING_PERCENTAGE; // Current percentage
int brightness = BRIGHTNESS_INITIAL;  // Current brightness level
bool flash = false;
bool resetToStart = false; // Flag to indicate if we should reset to the starting percentage

void setup()
{
  Serial.begin(115200);
  touch.begin(); // Initialize the touch screen controller

  tft.init();         // Initialize the display
  tft.setRotation(0); // Set display orientation, adjust if necessary

  // Set text font and size
  tft.setFreeFont(&cyborgpunk50pt7b);
  tft.setTextSize(1);
  TFT_SET_BL(brightness); // Set the initial brightness

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

    if (y < 50)
    {
      Serial.println("Touch detected top \r\n");
      if (brightness < 100)
        brightness++;
      TFT_SET_BL(brightness);
    }

    if (y > 190)
    {
      Serial.println("Touch detected bottom \r\n");
      if (brightness > 0)
        brightness--;
      TFT_SET_BL(brightness);
    }

    if (x >= 100 && x <= 140 && y >= 50 && y <= 190)
    { // Center touch detection
      Serial.println("Touch detected center \r\n");

      if (resetToStart)
      {
        resetToStart = false;
        percentage = STARTING_PERCENTAGE; // Reset to starting percentage
        changeColor();
      }
      else
      {
        resetToStart = true;
        showCountdown(); // Show countdown before animation
        percentage = STARTING_PERCENTAGE; // Reset to starting percentage
        changeColor();
        if(percentage <11)
        {
          flashRed();
        }
        delay(3000);     // x-second delay before starting the animation
        animateBatteryChange();
      }
    }

    delay(200); // Add delay to avoid rapid changes
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

void showCountdown()
{
  for (int i = 3; i > 0; i--)
  {
    tft.fillScreen(TFT_BLUE);           // Set background to blue
    tft.setTextColor(TFT_WHITE, TFT_BLUE); // Set text color to white with blue background
    drawText(String(i));               // Draw the countdown number
    delay(COUNTDOWN_STEP_MS);          // Wait for the countdown step duration
  }
  tft.fillScreen(TFT_BLUE);           // Clear the countdown number
}

void animateBatteryChange()
{
  int step = (TARGET_PERCENTAGE > percentage) ? 1 : -1;

  while (percentage != TARGET_PERCENTAGE)
  {
    percentage += step;

    changeColor();

    delay(STEP_DELAY_MS); // Adjust this delay for speed of animation
  }
}

void changeColor()
{
  if (percentage > 50)
  {
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
  }
  else if (percentage > 25 && percentage <= 50)
  {
    tft.fillScreen(TFT_ORANGE);
    tft.setTextColor(TFT_BLACK, TFT_ORANGE);
  }
  else if (percentage > 10 && percentage <= 25)
  {
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_BLACK, TFT_RED);
  }
  drawText(String(percentage));
}

void drawText(String text)
{
  // Set the text datum to the center
  tft.setTextDatum(MC_DATUM);
  tft.setTextPadding(tft.width());

  tft.drawString(text, tft.width() / 2, tft.height() / 2 - 15);
}

void flashRed()
{

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
  drawText(String(percentage));
  delay(250);
}

void TFT_SET_BL(uint8_t Value)
{
  if (Value > 100)
  {
    Serial.println("TFT_SET_BL Error \r\n");
  }
  else
  {
    analogWrite(TFT_BL, Value * 2.55); // Convert 0-100 to 0-255 range
  }
}
