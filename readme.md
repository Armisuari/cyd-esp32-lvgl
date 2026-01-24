## 🏗️ Firmware Architecture

![Architecture Diagram](image.png)

## 🎨 Firmware Design

The firmware is organized into modular components. Below are the primary interfaces for each module:

### 📱 Application
```cpp
bool init();
int displayGif(char *path);
bool displayAllGif(char *dir);
bool startWebServer();
```

### 🌐 Web Server
```cpp
bool init();
bool serverOn(const char *page);
```

### 📺 Display
```cpp
void init();
bool tftDraw();
```

### 📶 Network
```cpp
bool init();
bool connect(const char *ssid, const char *password);
bool disconnect();
bool isConnected();
bool getLocalIP();
bool getMACAddress();
bool getSignalStrength();
```

### 💾 Storage
```cpp
bool init();
bool openFile(char *fileName);
bool deleteFile(char *fileName);
bool deleteAllFiles(char *dir);
bool isDirectory();
```
## 🏗️ Build Pre-requisites

Before building the firmware, you must manually configure the library setup by copying the custom configuration files to the PlatformIO library dependency folders. This ensures the display and UI framework are correctly configured for the CYD hardware.

1. **TFT_eSPI Setup**: 
   Copy `tft_include/User_Setup.h` to:
   `.pio\libdeps\esp32doit-devkit-v1\TFT_eSPI\User_Setup.h`

2. **LVGL Setup**: 
   Copy `lvgl_include/lv_conf.h` to:
   `.pio\libdeps\esp32doit-devkit-v1\lv_conf.h`

> [!IMPORTANT]
> These steps are necessary because PlatformIO downloads clean versions of the libraries. Failing to copy these files will result in compilation errors or a non-functional display.