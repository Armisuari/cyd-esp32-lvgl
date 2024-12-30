#include "CustomLGFX.h"
#include "LGFXConfig.h"

CustomLGFX::CustomLGFX() {
    configureBus();
    configurePanel();
    configureBacklight();
    configureTouch();
    setPanel(&_panel_instance);
}

void CustomLGFX::configureBus() {
    auto cfg = LGFXConfig::busConfig();
    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
}

void CustomLGFX::configurePanel() {
    auto cfg = LGFXConfig::panelConfig();
    _panel_instance.config(cfg);
}

void CustomLGFX::configureBacklight() {
    auto cfg = LGFXConfig::lightConfig();
    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
}

void CustomLGFX::configureTouch() {
    auto cfg = LGFXConfig::touchConfig();
    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
}
