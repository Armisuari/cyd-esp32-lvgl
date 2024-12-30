#pragma once

#include <LovyanGFX.hpp>

namespace LGFXConfig {
    lgfx::Bus_SPI::config_t busConfig();
    lgfx::Panel_ST7796::config_t panelConfig();
    lgfx::Light_PWM::config_t lightConfig();
    lgfx::Touch_XPT2046::config_t touchConfig();
}