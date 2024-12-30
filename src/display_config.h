#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
    public:
        LGFX()
        {
            auto bus_cfg = lgfx::Bus_SPI::config_t();
            bus_cfg.spi_host = VSPI_HOST; // Use VSPI
            bus_cfg.spi_mode = 0;
            bus_cfg.freq_write = 40000000; // Adjust frequency as needed
            bus_cfg.freq_read = 20000000;
            bus_cfg.pin_sclk = 18; // SPI SCLK pin
            bus_cfg.pin_mosi = 23; // SPI MOSI pin
            bus_cfg.pin_miso = -1; // (not used)
            bus_cfg.pin_dc = 2;    // Data/Command pin
            _bus_instance.config(bus_cfg);
            _panel_instance.setBus(&_bus_instance);

            auto panel_cfg = lgfx::Panel_ILI9341::config_t();
            panel_cfg.pin_cs = 5;          // Chip select pin
            panel_cfg.pin_rst = 4;         // Reset pin
            panel_cfg.pin_busy = -1;       // (not used)
            panel_cfg.memory_width = 240;  // Display width
            panel_cfg.memory_height = 320; // Display height
            _panel_instance.config(panel_cfg);

            setPanel(&_panel_instance);
        }

    private:
        lgfx::Bus_SPI _bus_instance;
        lgfx::Panel_ST7796 _panel_instance;
};
