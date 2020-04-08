#include "ESPServer.h"

ESP8266WebServer ESPServer::server;
bool ESPServer::requires_reconnect;

void ESPServer::launch()
{
    server.on("/", handleIndex);
    server.on("/config", handleConfig);
    server.begin();
}

void ESPServer::handleIndex()
{
    server.send_P(200, "text/html", config_index);
}

void ESPServer::handleConfig()
{
    HTTPMethod method = server.method();

    if (method == HTTP_POST)
    {
        // process post request
        handleConfig_Post();
    }
    else if (method == HTTP_GET)
    {
        // process get request
        handleConfig_Get();
    }
    else
    {
        //TO DO NOT IMPLEMENTED ERROR
    }
}

void ESPServer::handleConfig_Post()
{
    char param[30];
    char value[30];

    // Loop through send parameters
    for (int i = 0; i < server.args(); i++)
    {
        // convert args and values into arrays
        server.argName(i).toCharArray(param, 30);
        server.arg(i).toCharArray(value, 30);

        // check parameter and update values
        // WiFI SSID
        if (strcmp(param, "ssid") == 0)
        {
            Config::update(Config::Params::SSID, value);
            requires_reconnect = true;
        }
        // WiFi PWD
        else if (strcmp(param, "wifiPwd") == 0)
        {
            Config::update(Config::Params::PWD, value);
            requires_reconnect = true;
        }
        else
        {
            Serial.print(F("Parameter unknown:\t"));
            Serial.println(param);
        }

        // clear arrays
        memset(param, 0, 30);
        memset(value, 0, 30);
    }
    server.send(200);
}

void ESPServer::handleConfig_Get()
{
    // prepare response
    char response[150];
    sprintf(response, "ssid=%s&dbIp=%s&dbUsername=%s", Config::ssid, Config::db_ip, Config::db_username);
    server.send(200, "text/plain", response); // send data to the server
}