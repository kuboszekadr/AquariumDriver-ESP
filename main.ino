#include "src/I2CMaster.h"
#include "src/Config.h"
#include "src/templates/Index.h"

#include <Arduino.h>
#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Wire.h>

#define DATA_HARVEST_OFFSET 60000L // get data from Arduino once a minute

ESP8266WebServer server;

IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

unsigned long last_data_harvest = 0L;

void setup()
{
    Serial.begin(9600); // for debugging porpuses
    delay(7500);
    Serial.println("Serial test");

    Config::load();

    // Check if ESP has WiFi configured
    if (!Config::ssid)
    {
        Serial.println("No SSID configured - starting in soft AP mode");
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP("ESP Config WiFi");
    }
    else
    {
        Serial.println("Connecting to target WiFi...");
        if (connectToWiFi())
        {
            WiFi.softAPdisconnect(true);
            Serial.print("Connected to ");
            Serial.println(Config::ssid);
        }
    }

    launchServer();
}

void loop()
{
    server.handleClient();
    // requestDataFromArduino();
}

// void requestDataFromArduino()
// {
//     if (millis() - last_data_harvest > DATA_HARVEST_OFFSET)
//     {
//         last_data_harvest = millis();
//         Serial.println("Requesting data from the slave...");

//         memset(i2c::buffer, 0, 512);
//         i2c::requestData();

//         Serial.println("Done.");
//         Serial.println(i2c::buffer);
//     }
// }

void launchServer()
{
    Serial.println(F("Starting web server..."));
    server.on("/", handleRoot);
    server.on("/config", handleConfig);
    server.begin();
}

void handleRoot()
{
    server.send_P(200, "text/html", index);
}

void handleConfig()
{
    HTTPMethod method = server.method();

    if (method == HTTP_POST)
    {
        // process post request
        handleConfigPost();
    }
    else if (method == HTTP_GET)
    {
        // process get request
        handleConfigGet();
    }
    else
    {
        //TO DO NOT IMPLEMENTED ERROR
    }
}

void handleConfigPost()
{
    char param[30];
    char value[30];
    bool reconnect = false;

    // Loop through send parameters
    for (int i = 0; i < server.args(); i++)
    {
        // convert args and values into arrays
        server.argName(i).toCharArray(param, 30);
        server.arg(i).toCharArray(value, 30);

        // print incoming paramether in the serial
        Serial.print("Param: ");
        Serial.print(param);
        Serial.print("\t");
        Serial.println(value);

        // check parameter and update values
        // WiFI SSID
        if (strcmp(param, "ssid") == 0)
        {
            Config::update(Config::Params::SSID, value);
            reconnect = true;
        }
        // WiFi PWD
        else if (strcmp(param, "wifiPwd") == 0)
        {
            Config::update(Config::Params::PWD, value);
            reconnect = true;
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
    Serial.println("Reloading config variables");
    Config::load(); // reload config variables after update
    Serial.println("Reloaded");
    // check if WiFi-relevant parameters have changed
    if (reconnect)
    {
        // if yes, reconnecto to target WiFi
        if (connectToWiFi())
        {
            // Disable softAPMode if ESP was in that mode
            WiFi.softAPdisconnect(true);
        }
    }
}

void handleConfigGet()
{
    // prepare response
    char response[150];
    sprintf(response, "ssid=%s&dbIp=%s&dbUsername=%s", Config::ssid, Config::db_ip, Config::db_username);

    Serial.println(response);
    server.send(200, "text/plain", response); // send data to the server
}

bool connectToWiFi()
{
    // return false if SSID is empty
    if (!Config::ssid)
    {
        return false;
    }

    // try to connect
    Serial.print("Connecting to ");
    Serial.println(Config::ssid);
    Serial.println(Config::pwd);

    WiFi.begin(Config::ssid, Config::pwd);
    // WiFi.begin("Zdrajcy metalu", "Dz3nt31m3n_m3ta1u");
    int i = 0; // to checkout timeout
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        i++;
        delay(500);

        // break after approx 50s
        if (i > 50)
        {
            Serial.println("Connection timeout.");
            Serial.println(WiFi.status());
            return false;
        }
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to the target");
    }

    return true;
}