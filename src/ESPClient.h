#ifndef ESPClient__H
#define ESPClient__H

#include "Config.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define ESP_CLIENT_DEVICE_ID 1
#define ESP_CLIENT_POST_STATUS_NO_DATA 0
#define ESP_CLIENT_POST_STATUS_SUCCESS 1
#define ESP_CLIENT_POST_STATUS_FAILURE 2

namespace ESPClient
{
    void launchSoftAP(char *ssid);

    bool downloadConfig(char* target);
    uint8_t sendData(const char *data);

    wl_status_t connect();
    extern char IP[15];
}

#endif