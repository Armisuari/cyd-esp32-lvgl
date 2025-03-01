#pragma once

#include <stdlib.h>
#include <FS.h>

class StorageInterface
{
    public:
        virtual bool init() = 0;
        virtual fs::File openFile(const String& path, const char *mode = "r") = 0;
        virtual bool deleteAllFiles(const char *path) = 0;
        virtual bool deleteDirectory(const char *dirname) = 0;
        virtual bool deleteFile(const String& filename) = 0;
};