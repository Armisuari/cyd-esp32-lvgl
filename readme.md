## Firmware Architecture

![alt text](image.png)

## Firmware Design

```C++

// Application
bool init();
int displayGif(char *path);
bool displayAllGif(char *dir);
bool startWebServer();

// Web Server
bool init();
bool serverOn(const char *page);

// Display
void init();
bool tftDraw();

// Network
bool init();
bool connect(const char *ssid, const char *password);
bool disconnect();
bool isConnected();
bool getLocalIP();
bool getMACAddress();
bool getSignalStrength();

// Storage
bool init();
bool openFile(char *fileName);
bool deleteFile(char *fileName);
bool deleteAllFiles(char *dir);
bool isDirectory();

```