#ifndef ESPClient__H
#define ESPClient__H

#include "Config.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

namespace ESPClient
{
    void launchSoftAP(char *ssid);

    bool downloadConfig(char* target);
    bool sendData(const char *data);

    wl_status_t connect();
    extern char IP[15];
}

#endif