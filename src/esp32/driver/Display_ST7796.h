#pragma once

#include <vector>
#include <string>
#include <interface/DisplayInterface.h>
#include <CustomLGFX.h>
#include <TFT_eSPI.h>

class Display_ST7796 : public DisplayInterface
{
    public:
        Display_ST7796(uint16_t screenWidth = 320, uint16_t screenHeight = 480);
        ~Display_ST7796();
        void init();
        void tftDraw(int x, int y, int xOffset, int yOffset, int w, int h, uint16_t *lBuf);
        size_t drawString(const char *string, int poX, int poY);
        void fillScreen(uint16_t color);
        uint16_t getWidht();
        uint16_t getHeight();

    private:
        TFT_eSPI tft;
        uint16_t _screenWidth;
        uint16_t _screenHeight;
};