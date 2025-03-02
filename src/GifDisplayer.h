#pragma once

#include <Arduino.h>
#include <vector>
#include <string>
#include <AnimatedGIF.h>
#include <FS.h>
#include <TFT_eSPI.h>
#include <freertos/semphr.h>

#include <interface/StorageInterface.h>
#include <interface/DisplayInterface.h>
#include <interface/NetworkInterface.h>

#include <esp32/webserver/WebServerHandler.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_DISCONNECTED_BIT BIT1

class GifDisplayer
{
    public:
        GifDisplayer(StorageInterface &storage, DisplayInterface &display, NetworkInterface &network);
        ~GifDisplayer();
        bool init();
        int displayGif(char *gifPath);
        void displayAllGif();
        int getTotalGifFiles() { return totalGifFiles; }

    private:
        AnimatedGIF gif;
        std::vector<std::string> gifFiles;

        WebServerHandler webServerHandler;

        StorageInterface &_storage;
        DisplayInterface &_display;
        NetworkInterface &_network;

        static SemaphoreHandle_t xGifMutex;

        static GifDisplayer *instance;

        static EventGroupHandle_t wifi_event_group;
        static TaskHandle_t _taskHandle;
        static TaskHandle_t _taskFileHandle;

        static void _taskFunction(void *pvParameters);
        static void _taskFuncFileHandle(void *pvParameters);

        bool startWebServer();
        int getGifInventory(const char *basePath);
        static void *gifOpenFile(const char *fName, int32_t *pSize);
        static void gifCloseFile(void *pHandle);
        static int32_t gifReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
        static int32_t gifSeekFile(GIFFILE *pFile, int32_t iPosition);
        static void gifDraw(GIFDRAW *pDraw);

        void _uploadFileHandleCallback(String filename, size_t index, uint8_t *data, size_t len, bool final);
        void _deleteFileHandleCallback();

        static File FSGifFile;
        int totalGifFiles;
        int currentGifFile;
        static int lastGifFile;
        static int xOffSet;
        static int yOffSet;
        int gifWidth;
        int gifHeight;
        char gifComment[256];
};