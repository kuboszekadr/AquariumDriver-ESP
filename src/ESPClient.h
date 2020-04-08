#ifndef ESPClient__H
#define ESPClient__H

#include "Config.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace ESPClient
{
    void launchSoftAP(char *ssid);
    wl_status_t connect();
}

#endif