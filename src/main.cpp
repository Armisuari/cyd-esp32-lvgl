#include <LovyanGFX.hpp>
#include "display_config.h"

LGFX display;

void setup()
{
  display.init();
  display.fillScreen(TFT_BLACK);
  display.setTextSize(2);
  display.setTextColor(TFT_WHITE);
  display.setCursor(10, 10);
  display.print("Display Initialized!");
}

void loop()
{
  // Empty loop
}
