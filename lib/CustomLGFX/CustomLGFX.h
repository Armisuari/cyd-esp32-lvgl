#pragma once
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class CustomLGFX : public lgfx::LGFX_Device {
public:
    CustomLGFX();
private:
    void configureBus();
    void configurePanel();
    void configureBacklight();
    void configureTouch();

    lgfx::Panel_ST7796 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
};
