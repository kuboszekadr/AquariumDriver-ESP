#ifndef ESPServer__h
#define ESPServer__h

#include "Config.h"
#include "templates/ConfigIndex.h"

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

namespace ESPServer
{

extern ESP8266WebServer server;
extern bool requires_reconnect;

void launch();

// pages handlers
void handleIndex();
void handleConfig();

void handleConfig_Post();
void handleConfig_Get();

} // namespace ESPServer

#endif