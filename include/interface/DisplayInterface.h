#pragma once
#include <stdlib.h>

class DisplayInterface
{
    public:
        virtual void init() = 0;
        virtual void tftDraw(int x, int y, int xOffset, int yOffset, int w, int h, uint16_t *lBuf) = 0;
        virtual size_t drawString(const char *string, int poX, int poY) = 0;
        virtual void fillScreen(uint16_t color) = 0;
        virtual uint16_t getWidht() = 0;
        virtual uint16_t getHeight() = 0;
};