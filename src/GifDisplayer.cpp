#include <esp_task_wdt.h>
#include "GifDisplayer.h"

const char *appTag = "GifDisplayer";

SemaphoreHandle_t GifDisplayer::xGifMutex = NULL;

GifDisplayer *GifDisplayer::instance = NULL;
int GifDisplayer::lastGifFile = -1;
File GifDisplayer::FSGifFile;
int GifDisplayer::xOffSet = 0;
int GifDisplayer::yOffSet = 0;

EventGroupHandle_t GifDisplayer::wifi_event_group = NULL;
TaskHandle_t GifDisplayer::_taskHandle = NULL;

GifDisplayer::GifDisplayer(StorageInterface &storage, DisplayInterface &display, NetworkInterface &network) : _storage(storage), _display(display), _network(network), webServerHandler(storage, &_taskHandle)
{
    instance = this;
    currentGifFile = 0;
}

GifDisplayer::~GifDisplayer()
{
}

bool GifDisplayer::init()
{
    ESP_LOGI(appTag, "Initializing Peripherals");
    _display.init();
    _storage.init();

    _display.fillScreen(TFT_BLACK);

    wifi_event_group = xEventGroupCreate();
    if (wifi_event_group == NULL)
    {
        ESP_LOGE(appTag, "Failed to create event group");
        return false;
    }

    ESP_LOGW(appTag, "Waiting for network connection");
    if (!_network.init(WIFI_STA))
    {
        ESP_LOGE(appTag, "Failed to initialize network");
        return false;
    }

    if (_network.isConnected())
    {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else
    {
        xEventGroupSetBits(wifi_event_group, WIFI_DISCONNECTED_BIT);
    }

    xGifMutex = xSemaphoreCreateMutex();
    if (xGifMutex == NULL)
    {
        ESP_LOGE(appTag, "Failed to create mutex");
        return false;
    }

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI(appTag, "Starting Web Server");
    if (!startWebServer())
    {
        ESP_LOGE(appTag, "Failed to start web server");
        return false;
    }

    totalGifFiles = getGifInventory("/");
    ESP_LOGI(appTag, "Total GIF files: %d", totalGifFiles);

    if (totalGifFiles == 0)
    {
        ESP_LOGE(appTag, "No GIF files found. please upload some GIF files");
        return false;
    }

    // webServerHandler.isGifPlay(true);
    xTaskCreate(_taskFunction, "GifDisplayerTask", 4096, this, 5, &_taskHandle);

    return true;
}

void GifDisplayer::_taskFunction(void *pvParameters)
{
    GifDisplayer *app = static_cast<GifDisplayer *>(pvParameters);
    while (1)
    {
        if (xSemaphoreTake(xGifMutex, portMAX_DELAY) == pdTRUE)
        {
            app->displayAllGif();
            xSemaphoreGive(xGifMutex);
        }
    }

    vTaskDelete(NULL);
}

int GifDisplayer::displayGif(char *gifPath)
{
    gif.begin(BIG_ENDIAN_PIXELS);

    if (!gif.open(gifPath, gifOpenFile, gifCloseFile, gifReadFile, gifSeekFile, gifDraw))
    {
        ESP_LOGE(appTag, "Failed to open gif file %s", gifPath);
        return 3000;
    }

    int frameDelay = 0; // store delay for the last frame
    int then = 0;       // store overall delay
    bool showcomment = false;

    // gifWidth = gif.getCanvasWidth() * scale;
    // gifHeight = gif.getCanvasHeight() * scale;
    gifWidth = gif.getCanvasWidth();
    gifHeight = gif.getCanvasHeight();

    // center the GIF !!

    xOffSet = (_display.getWidht() - gifWidth) / 2;
    yOffSet = (_display.getHeight() - gifHeight) / 2;

    if (lastGifFile != currentGifFile)
    {
        ESP_LOGD(appTag, "Playing %s [%d,%d] with offset [%d,%d]", gifPath, gifWidth, gifHeight, xOffSet, yOffSet);
        lastGifFile = currentGifFile;
        showcomment = true;
    }

    while (gif.playFrame(true, &frameDelay))
    {
        if (showcomment)
            if (gif.getComment(gifComment))
                ESP_LOGD(appTag, "GIF Comment: %s", gifComment);

        then += frameDelay;
        if (then > 30000)
        { // avoid being trapped in infinite GIF's
            // log_w("Broke the GIF loop, max duration exceeded");
            break;
        }
    }

    gif.close();

    return then;
}

void *GifDisplayer::gifOpenFile(const char *fName, int32_t *pSize)
{
    FSGifFile = instance->_storage.openFile(fName);
    if (FSGifFile)
    {
        *pSize = FSGifFile.size();
        return (void *)&FSGifFile;
    }
    return NULL;
}

void GifDisplayer::gifCloseFile(void *pHandle)
{
    File *file = static_cast<File *>(pHandle);
    if (file != NULL)
    {
        file->close();
    }
}

int32_t GifDisplayer::gifReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
        iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
        return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();

    return iBytesRead;
}

int32_t GifDisplayer::gifSeekFile(GIFFILE *pFile, int32_t iPosition)
{
    int i = micros();
    File *f = static_cast<File *>(pFile->fHandle);
    f->seek(iPosition);
    pFile->iPos = (int32_t)f->position();
    i = micros() - i;
    // log_d("Seek time = %d us\n", i);
    return pFile->iPos;
}

void GifDisplayer::gifDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

    iWidth = pDraw->iWidth;
    if (iWidth > instance->_display.getWidht())
        iWidth = instance->_display.getWidht();
    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2)
    { // restore to background color
        for (x = 0; x < iWidth; x++)
        {
            if (s[x] == pDraw->ucTransparent)
                s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency)
    { // if transparency used
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        int x, iCount;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0; // count non-transparent pixels
        while (x < iWidth)
        {
            c = ucTransparent - 1;
            d = usTemp;
            while (c != ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent)
                {        // done, stop
                    s--; // back up to treat it like transparent
                }
                else
                { // opaque
                    *d++ = usPalette[c];
                    iCount++;
                }
            } // while looking for opaque pixels
            if (iCount)
            { // any opaque pixels?
                instance->_display.tftDraw(pDraw->iX + x, y, xOffSet, yOffSet, iCount, 1, (uint16_t *)usTemp);
                x += iCount;
                iCount = 0;
            }
            // no, look for a run of transparent pixels
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent)
                    iCount++;
                else
                    s--;
            }
            if (iCount)
            {
                x += iCount; // skip these
                iCount = 0;
            }
        }
    }
    else
    {
        s = pDraw->pPixels;
        // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
        for (x = 0; x < iWidth; x++)
            usTemp[x] = usPalette[*s++];
        instance->_display.tftDraw(pDraw->iX, y, xOffSet, yOffSet, iWidth, 1, (uint16_t *)usTemp);
    }
}

int GifDisplayer::getGifInventory(const char *basePath)
{
    int amount = 0;
    ESP_LOGD(appTag, "Opening directory %s", basePath);
    File GifRootFolder = _storage.openFile(basePath);
    if (!GifRootFolder)
    {
        ESP_LOGE(appTag, "Failed to open directory");
        return 0;
    }

    if (!GifRootFolder.isDirectory())
    {
        ESP_LOGE(appTag, "Not a directory");
        return 0;
    }

    ESP_LOGD(appTag, "Reading files from directory %s", basePath);
    File file = GifRootFolder.openNextFile();

    // tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // tft.setTextSize(2);

    int textPosX = _display.getWidht() / 2 - 16;
    int textPosY = _display.getHeight() / 2 - 10;

    _display.drawString("GIF Files:", textPosX - 40, textPosY - 20);

    while (file)
    {
        if (!file.isDirectory())
        {

            String fileName = file.name();
            if (fileName.startsWith("._"))
            {
                file = GifRootFolder.openNextFile();
                continue;
            }

            gifFiles.push_back(file.name());
            amount++;
            _display.drawString(String(amount).c_str(), textPosX, textPosY);
            file.close();
        }
        file = GifRootFolder.openNextFile();
    }
    GifRootFolder.close();
    ESP_LOGD(appTag, "Found %d GIF files", amount);
    return amount;
}

void GifDisplayer::displayAllGif()
{
    const char *_fileName = gifFiles[currentGifFile++ % totalGifFiles].c_str();
    std::string fileName = std::string("/") + std::string(_fileName);

    int loops = 5;
    int durationControl = 3000;

    _display.fillScreen(TFT_BLACK);

    while (loops-- > 0 && durationControl > 0)
    {
        durationControl -= displayGif((char *)fileName.c_str());
        gif.reset();
    }
}

bool GifDisplayer::startWebServer()
{
    webServerHandler.setHandleFileUploadCB(std::bind(&GifDisplayer::_uploadFileHandleCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
    // webServerHandler.setHandleFileDeleteCB(std::bind(&GifDisplayer::_deleteFileHandleCallback, this));
    return webServerHandler.init(_network.isConnected());
}

void GifDisplayer::_uploadFileHandleCallback(String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    static bool ever;
    if (!ever)
    {
        ever = true;
        ESP_LOGD(appTag, "First upload file callback");

        if (_taskHandle != NULL)
        {
            vTaskDelete(_taskHandle);
        }
    }

    static File uploadFile;
    ESP_LOGD(appTag, "Received file: %s, index: %d, len: %d, final: %d\n", filename.c_str(), index, len, final);
    
    if (!index)
    {
        // Open the file for writing on the SD card
        esp_task_wdt_reset(); // Reset the watchdog timer
        filename = "/uploaded_" + filename;
        ESP_LOGD(appTag, "Uploading file: %s\n", filename.c_str());
        uploadFile = _storage.openFile(filename, "w");
    }

    if (uploadFile)
    {
        // Write the received data to the file
        if (uploadFile.write(data, len) != len)
        {
            ESP_LOGE(appTag, "Failed to write file");
        }
    }

        // // Add a delay to allow other tasks to run and reset the watchdog timer
        // vTaskDelay(10 / portTICK_PERIOD_MS);

        if (final)
        {
            uploadFile.close();
            ESP_LOGD(appTag, "File uploaded: %s\n", filename.c_str());
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            esp_restart();
        }
    }