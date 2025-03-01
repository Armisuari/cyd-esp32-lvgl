#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <interface/NetworkInterface.h>

class Network_Wifi : public NetworkInterface
{
    public:
        Network_Wifi(const char *ssid, const char *password);
        ~Network_Wifi();

        bool init(wifi_mode_t mode);
        bool disconnect();
        bool isConnected();
        String getLocalIP();
        String getMACAddress();
        int getSignalStrength();

        void generateSSIDName(char *ssid);

    private:
        char _ssid[32];
        char _password[32];
        bool _connected;
        const char* WIFI_SSID_PREFIX = "esp32-cyd-";

        bool connect(const char *ssid, const char *password);
};