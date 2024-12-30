#include "LGFXConfig.h"

namespace LGFXConfig {

lgfx::Bus_SPI::config_t busConfig() {
    lgfx::Bus_SPI::config_t cfg;
    cfg.spi_host = SPI2_HOST;
    cfg.spi_mode = 0;
    cfg.freq_write = 80000000;
    cfg.freq_read = 16000000;
    cfg.spi_3wire = false;
    cfg.use_lock = true;
    cfg.dma_channel = SPI_DMA_CH_AUTO;
    cfg.pin_sclk = 14;
    cfg.pin_mosi = 13;
    cfg.pin_miso = 12;
    cfg.pin_dc = 2;
    return cfg;
}

lgfx::Panel_ST7796::config_t panelConfig() {
    lgfx::Panel_ST7796::config_t cfg;
    cfg.pin_cs = 15;
    cfg.pin_rst = -1;
    cfg.pin_busy = -1;
    cfg.memory_width = 320;
    cfg.memory_height = 480;
    cfg.panel_width = 320;
    cfg.panel_height = 480;
    cfg.offset_x = 0;
    cfg.offset_y = 0;
    cfg.offset_rotation = 0;
    cfg.dummy_read_pixel = 8;
    cfg.dummy_read_bits = 1;
    cfg.readable = false;
    cfg.invert = false;
    cfg.rgb_order = false;
    cfg.dlen_16bit = false;
    cfg.bus_shared = false;
    return cfg;
}

lgfx::Light_PWM::config_t lightConfig() {
    lgfx::Light_PWM::config_t cfg;
    cfg.pin_bl = 27;
    cfg.invert = false;
    cfg.freq = 44100;
    cfg.pwm_channel = 7;
    return cfg;
}

lgfx::Touch_XPT2046::config_t touchConfig() {
    lgfx::Touch_XPT2046::config_t cfg;
    cfg.x_min = 222;
    cfg.x_max = 3367;
    cfg.y_min = 192;
    cfg.y_max = 3732;
    cfg.pin_int = -1;
    cfg.bus_shared = true;
    cfg.offset_rotation = 6;
    cfg.spi_host = SPI2_HOST;
    // cfg.freq = 1000000;
    cfg.freq = 500000;
    cfg.freq = 250000;
    cfg.pin_sclk = 14;
    cfg.pin_mosi = 13;
    cfg.pin_miso = 12;
    cfg.pin_cs = 33;
    return cfg;
}

} // namespace LGFXConfig
