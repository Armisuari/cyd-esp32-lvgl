#include "Storage_SD.h"

const char *SDTag = "Storage_SD";

Storage_SD::Storage_SD(int chipSelect) : _chipSelect(chipSelect)
{
}

Storage_SD::~Storage_SD()
{
}

bool Storage_SD::init()
{
    while (!SD.begin(_chipSelect))
    {
        ESP_LOGE(SDTag, "SD Card Mount Failed");
        delay(500);
    }

    ESP_LOGI(SDTag, "SD Card Mount Success");
    return true;
}

fs::File Storage_SD::openFile(const String& fileName, const char* mode)
{
    File file = SD.open(fileName, mode);
    if (!file)
    {
        ESP_LOGE(SDTag, "Failed to open file for reading");
    }

    return file;
}

bool Storage_SD::deleteFile(const String& fileName)
{
    if (SD.remove(fileName))
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
    File root = SD.open(dir);
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
            ESP_LOGI(SDTag, "Deleting directory: %s", fileName.c_str());
            deleteDirectory(fileName.c_str());
        }
        else
        {
            // If it's a file, delete it
            ESP_LOGI(SDTag, "Deleting file: %s", fileName.c_str());
            SD.remove(fileName.c_str());
        }

        entry.close();
    }

    return true;
}

bool Storage_SD::deleteDirectory(const char *dirname)
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
            ESP_LOGI(SDTag, "Deleting file: %s", entry.name());
            SD.remove(entry.name());
        }

        entry.close();
    }
    // After directory is empty, remove it
    SD.rmdir(dirname);

    return true;
}