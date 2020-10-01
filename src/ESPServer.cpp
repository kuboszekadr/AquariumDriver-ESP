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
    char arg[5];
    char param_json[256];

    server.argName(0).toCharArray(arg, 5);
    server.arg(1).toCharArray(param_json, 256);

    StaticJsonDocument<256> doc;
    deserializeJson(doc, param_json);
    
    if (strcmp(arg, "wifi") == 0)
    {
        const char* ssid = doc["ssid"];
        const char* pwd = doc["pwd"];

        Config::update(Config::Params::SSID, ssid);
        Config::update(Config::Params::PWD, pwd);

    }
    else if (strcmp(arg, "api") == 0)
    {
        const char* app_host_ip = doc["appHostIp"];
        const char* app_host_port = doc["appHostPort"];

        Config::update(Config::Params::API_HOST_IP, app_host_ip);
        Config::update(Config::Params::API_HOST_PORT, app_host_port);
        
        Serial.println(Config::Params::API_HOST_IP);
        Serial.println(Config::Params::API_HOST_PORT);

        const char* app_device_name = doc["appDeviceName"];
        const char* app_device_id = doc["appDeviceId"];

        Config::update(Config::Params::API_DEVICE_ID, app_device_id);
        Config::update(Config::Params::API_DEVICE_NAME, app_device_name);

        Serial.println(Config::Params::API_DEVICE_ID);
        Serial.println(Config::Params::API_DEVICE_NAME);
    }
    else
    {
        server.send(400);
    }    
    server.send(200);
}

void ESPServer::handleConfig_Get()
{
    // prepare response
    char response[400];
    StaticJsonDocument<256> doc;

    JsonObject wifi = doc.createNestedObject("wifi");
    wifi["ssid"] = Config::ssid;

    JsonObject api = doc.createNestedObject("api");
    api["apiHostIP"] = Config::api_host_ip;
    api["apiHostPort"] = Config::api_host_port;

    api["apiDeviceId"] = Config::api_device_id;
    api["apiDeviceName"] = Config::api_device_name;

    serializeJson(doc, response);
    Serial.println(response);
    server.send(200, "text/plain", response); // send data to the server
}