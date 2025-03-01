#include "Display_ST7796.h"

Display_ST7796::Display_ST7796(uint16_t screenWidth, uint16_t screenHeight) :
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    tft(TFT_eSPI())
{
}

Display_ST7796::~Display_ST7796()
{
}

void Display_ST7796::init()
{
    tft.begin();
    tft.setRotation(0);
    // tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // tft.setTextSize(2);
}

void Display_ST7796::fillScreen(uint16_t color)
{
    tft.fillScreen(color);
}

void Display_ST7796::tftDraw(int x, int y, int xOffset, int yOffset, int w, int h, uint16_t *lBuf)
{
    tft.pushRect(x + xOffset, y + yOffset, w, h, lBuf);
}

uint16_t Display_ST7796::getWidht()
{
    return _screenWidth;
}

uint16_t Display_ST7796::getHeight()
{
    return _screenHeight;
}

size_t Display_ST7796::drawString(const char *string, int poX, int poY)
{
    return tft.drawString(string, poX, poY);
}