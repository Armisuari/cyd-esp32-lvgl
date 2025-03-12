#include "Network_Wifi.h"

const char *wifiTag = "Network_Wifi";

Network_Wifi::Network_Wifi(const char *ssid, const char *password) : _connected(false)
{
    strcpy(_ssid, ssid);
    strcpy(_password, password);
}

Network_Wifi::~Network_Wifi()
{
}

bool Network_Wifi::init(wifi_mode_t mode)
{
    WiFi.mode(mode);
    if (mode == WIFI_AP)
    {
        generateSSIDName(_ssid);
        WiFi.softAP(_ssid, _passwordAP);
        IPAddress IP = WiFi.softAPIP();
        ESP_LOGI(wifiTag, "AP IP Address: %s", IP.toString().c_str());
    }
    else
    {
        ESP_LOGI(wifiTag, "WiFi mode set to STA");
        connect(_ssid, _password);
    }

    return true;
}

bool Network_Wifi::connect(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        ESP_LOGW(wifiTag, "Connecting to WiFi...");
    }
    ESP_LOGI(wifiTag, "Connected to WiFi");
    ESP_LOGI(wifiTag, "IP Address: %s", getLocalIP().c_str());

    _connected = true;
    return _connected;
}

bool Network_Wifi::disconnect()
{
    WiFi.disconnect();
    return true;
}

bool Network_Wifi::isConnected()
{
    return _connected;
}

String Network_Wifi::getLocalIP()
{
    return WiFi.localIP().toString();
}

String Network_Wifi::getMACAddress()
{
    return WiFi.macAddress();
}

int Network_Wifi::getSignalStrength()
{
    return WiFi.RSSI();
}

void Network_Wifi::generateSSIDName(char *ssid)
{
    const char *ssidPrefix = WIFI_SSID_PREFIX;
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(ssid, "%s%02X%02X%02X", ssidPrefix, mac[3], mac[4], mac[5]);
}

