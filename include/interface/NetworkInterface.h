#pragma once

# include <Arduino.h>
#include <WiFi.h>
#include <stdlib.h>

class NetworkInterface
{
    public:
        virtual bool init(wifi_mode_t mode) = 0;
        // virtual bool connect(const char *ssid, const char *password) = 0;
        virtual bool disconnect() = 0;
        virtual bool isConnected() = 0;
        virtual String getLocalIP() = 0;
        virtual String getMACAddress() = 0;
        virtual int getSignalStrength() = 0;
};