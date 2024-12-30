#include <Arduino.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "html_file.h"
// #include <SdFat.h>
// #include <SdFatConfig.h>

#define W_STA_MODE

#define WIFI_SSID_PREFIX "esp32-cyd-"

#ifndef W_STA_MODE
char ssid[sizeof(WIFI_SSID_PREFIX) + 6];
const char *password = "123456789"; // You can change this as needed
#else
const char *ssid = "eFisheryFS";
const char *password = "123123123";
#endif

AsyncWebServer server(80);
// SdFat sd;

void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void generateSSIDName(char *ssid);
void deleteAllFiles(); // Function declaration for deleting all files
void deleteDirectory(const char *dirname);

void setup()
{
    Serial.begin(115200);

#ifndef W_STA_MODE
    generateSSIDName(ssid);
    // Set up ESP32 as an Access Point (AP)
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(IP);
#else
    // Initialize WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
#endif

    // Initialize SD card
    if (!SD.begin(5))
    {
        Serial.println("SD Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
    Serial.println("SD Card initialized");

    // Serve the upload form with a delete button
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    // String html = "<form method='POST' action='/upload' enctype='multipart/form-data'>";
    // html += "<input type='file' name='file'><input type='submit' value='Upload'>";
    String html = String(htmlPage);
    html += "</form>";
    html += "<form method='POST' action='/delete' onsubmit=\"return confirm('Are you sure you want to delete all files ?');\">";
    html += "<button type='submit'>delete all files</button>";
    html += "</form>";
    // html += String(htmlPage);
    request->send(200, "text/html", html); });

    // Handle file upload
    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "File Uploaded!"); }, handleFileUpload);

    // Handle deleting all files
    server.on("/delete", HTTP_POST, [](AsyncWebServerRequest *request)
              {
    deleteAllFiles();
    request->send(200, "text/plain", "All files deleted!"); });

    // server.on("/formatSD", HTTP_POST, [](AsyncWebServerRequest *request)
    //           {
    // formatSDCard();
    // request->send(200, "text/plain", "SD Card Formatted"); }); // Format SD card route

    // Start the server
    server.begin();
}

void loop()
{
}

// Function to handle file upload
void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    static File uploadFile;

    if (!index)
    {
        // Open the file for writing on the SD card
        filename = "/uploaded_" + filename;
        Serial.printf("Uploading file: %s\n", filename.c_str());
        uploadFile = SD.open(filename, FILE_WRITE);
    }

    if (uploadFile)
    {
        // Write the received data to the file
        if (uploadFile.write(data, len) != len)
        {
            Serial.println("Failed to write file");
        }
    }

    if (final)
    {
        // Close the file when done
        if (uploadFile)
        {
            uploadFile.close();
            Serial.printf("File uploaded: %s\n", filename.c_str());
        }
    }
}

void generateSSIDName(char *ssid)
{
    const char *ssidPrefix = WIFI_SSID_PREFIX;
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(ssid, "%s%02X%02X%02X", ssidPrefix, mac[3], mac[4], mac[5]);
}

// Function to delete all non-system files from the SD card
void deleteAllFiles()
{
    File root = SD.open("//"); // Open the root directory
    while (true)
    {
        File entry = root.openNextFile();
        String fileName = "/" + String(entry.name());
        if (!entry)
        {
            // No more files
            break;
        }

        if (entry.isDirectory())
        {
            // If it's a directory, recursively delete its contents
            Serial.print("Deleting directory: ");
            Serial.println(fileName.c_str());
            deleteDirectory(fileName.c_str());
        }
        else
        {
            // If it's a file, delete it
            Serial.print("Deleting file: ");
            Serial.println(fileName.c_str());
            SD.remove(fileName.c_str());
        }

        entry.close();
    }
}

void deleteDirectory(const char *dirname)
{
    File dir = SD.open(dirname);
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break; // No more files
        }

        if (entry.isDirectory())
        {
            // Recursively delete subdirectory
            deleteDirectory(entry.name());
        }
        else
        {
            // Delete file
            Serial.print("Deleting file: ");
            Serial.println(entry.name());
            SD.remove(entry.name());
        }

        entry.close();
    }
    // After directory is empty, remove it
    SD.rmdir(dirname);
}