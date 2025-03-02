#include "Storage_SD.h"

#define USE_SD

const char *SDTag = "Storage_SD";

Storage_SD::Storage_SD(int chipSelect) : _chipSelect(chipSelect)
{
}

Storage_SD::~Storage_SD()
{
}

bool Storage_SD::init()
{
    #ifdef USE_SD
    while (!SD.begin(_chipSelect))
    {
        ESP_LOGE(SDTag, "SD Card Mount Failed");
        delay(500);
    }

    ESP_LOGI(SDTag, "SD Card Mount Success");
    #else
    if (!LittleFS.begin())
    {
        ESP_LOGE(SDTag, "LittleFS Mount Failed");
        return false;
    }
    #endif
    return true;
}

fs::File Storage_SD::openFile(const String &fileName, const char *mode)
{
    #ifdef USE_SD
    File file = SD.open(fileName, mode);
    #else
    fs::File file = LittleFS.open(fileName, mode);
    #endif
    if (!file)
    {
        ESP_LOGE(SDTag, "Failed to open file for reading");
    }

    return file;
}

bool Storage_SD::deleteFile(const String &fileName)
{
    #ifdef USE_SD
    if (SD.remove(fileName))
    #else
    if (LittleFS.remove(fileName))
    #endif
    {
        ESP_LOGI(SDTag, "File deleted: %s", fileName);
        return true;
    }
    else
    {
        ESP_LOGE(SDTag, "Failed to delete file: %s", fileName);
        return false;
    }
}

bool Storage_SD::deleteAllFiles(const char *dir)
{
    ESP_LOGI(SDTag, "Deleting all files in %s", dir);
    #ifdef USE_SD
    if (!SD.begin(_chipSelect))
    {
        ESP_LOGE(SDTag, "SD Card not mounted");
        return false;
    }
    fs::File root = SD.open(dir);
    #else
    fs::File root = LittleFS.open(dir);
    #endif
    if (!root)
    {
        ESP_LOGE(SDTag, "Failed to open directory: %s", dir);
        return false;
    }

    while (true)
    {
        ESP_LOGD(SDTag, "Deleting files in %s", dir);
        fs::File entry = root.openNextFile();
        if (!entry)
        {
            ESP_LOGD(SDTag, "No more files");
            break;
        }

        String fileName = "/" + String(entry.name());
        if (entry.isDirectory())
        {
            ESP_LOGI(SDTag, "Deleting directory: %s", fileName.c_str());
            deleteDirectory(fileName.c_str());
        }
        else
        {
            ESP_LOGI(SDTag, "Deleting file: %s", fileName.c_str());
            SD.remove(fileName.c_str());
        }

        entry.close();
    }

    return true;
}

bool Storage_SD::deleteDirectory(const char *dirname)
{
    #ifdef USE_SD
    if (!SD.begin(_chipSelect))
    {
        ESP_LOGE(SDTag, "SD Card not mounted");
        return false;
    }
    File dir = SD.open(dirname);
    #else
    fs::File dir = LittleFS.open(dirname);
    #endif
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
            ESP_LOGI(SDTag, "Deleting file: %s", entry.name());
            #ifdef USE_SD
            SD.remove(entry.name());
            #else
            LittleFS.remove(entry.name());
            #endif
        }

        entry.close();
    }
    // After directory is empty, remove it
    #ifdef USE_SD
    SD.rmdir(dirname);
    #else
    LittleFS.rmdir(dirname);
    #endif

    return true;
}