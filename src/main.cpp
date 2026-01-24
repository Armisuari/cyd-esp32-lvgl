#include <Arduino.h>

// #define USE_EX

#ifdef USE_EX
// #include "ex_touch.hpp"
#include "ex_webserver.hpp"
#else

#include <esp32/driver/Storage_SD.h>
#include <esp32/driver/Display_ST7796.h>
#include <esp32/driver/Network_Wifi.h>
#include "GifDisplayer.h"

Storage_SD stg(5);
Display_ST7796 display;
// Network_Wifi network("eFisheryFS", "123123123");
// Network_Wifi network("fatur-fatin", "putri1986");
// Network_Wifi network("AksesPoin_eFishery", "efishery2516!");
Network_Wifi network("Angkasa-Timelapse", "1234567890");

GifDisplayer app(stg, display, network);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    if (!app.init())
    {
        Serial.println("Failed to initialize app");
    }
}

void loop()
{
    // vTaskDelete(NULL);
    // receive command from serial
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "reset")
        {
            esp_restart();
        }
    }
}

#endif