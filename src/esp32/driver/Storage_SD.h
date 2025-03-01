#pragma once

#include <Arduino.h>
#include <interface/StorageInterface.h>
#include <FS.h>
#include <SPI.h>
#include <SD.h>

class Storage_SD : public StorageInterface
{
    public:
        Storage_SD(int chipSelect);
        ~Storage_SD();
        bool init();
        fs::File openFile(const String& fileName, const char* mode = "r");
        bool deleteFile(const String& fileName);
        bool deleteAllFiles(const char *dir);
        bool deleteDirectory(const char *dirname);

    private:
        int _chipSelect;  
};