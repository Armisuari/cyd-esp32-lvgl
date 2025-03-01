#include "WebServerHandler.h"
#include <esp_task_wdt.h>

const char *webServerTag = "WebServerHandler";

WebServerHandler::WebServerHandler(StorageInterface &storage, TaskHandle_t *gifTaskHandle) : _storage(storage), _gifTaskHandle(gifTaskHandle)
{
}

WebServerHandler::~WebServerHandler()
{
}

bool WebServerHandler::init(bool netConnected)
{
    if (!netConnected)
    {
        ESP_LOGE(webServerTag, "No network connection");
        return false;
    }

    _storage.init();

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
              {
        String html = String(this->htmlPage);
        html += "</form>";
        html += "<form method='POST' action='/delete' onsubmit=\"return confirm('Are you sure you want to delete all files ?');\">";
        html += "<button type='submit'>delete all files</button>";
        html += "</form>";
        request->send(200, "text/html", html); });

    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "File Uploaded!"); }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
              { this->handleFileUpload(request, filename, index, data, len, final); });
    // handleFileUpload);

    server.on("/delete", HTTP_POST, [this](AsyncWebServerRequest *request)
              { handleFileDelete(); });

    server.begin();

    return true;
}

void WebServerHandler::handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (_handleFileUploadCB)
    {
        _handleFileUploadCB(filename, index, data, len, final);
    }
}

void WebServerHandler::handleFileDelete()
{

    if (_gifTaskHandle != NULL)
    {
        vTaskDelete(*_gifTaskHandle);
    }

    // esp_task_wdt_reset(); // Reset the watchdog timer
    ESP_LOGW(webServerTag, "Deleting all files");
    _storage.deleteAllFiles("//");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_restart();
}