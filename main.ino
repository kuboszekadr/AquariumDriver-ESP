#include "src/Config.h"
#include "src/Display.h"
#include "src/ESPClient.h"
#include "src/ESPServer.h"
#include "src/I2CMaster.h"

#include <Arduino.h>
#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include <FS.h>
#include <Wire.h>

#define DATA_HARVEST_OFFSET 5000L

unsigned long last_data_harvest = 0L;
wl_status_t connection_status;

Display display = Display::getInstance();
char config_data[256];

void setup()
{
    Serial.begin(9600); // for debugging purposes
    Config::load();     // load wifi config data

    // short delay to allow Arduino to load config
    // and join I2C bus
    delay(1000);  

    i2c::begin();    // join i2c bus
    display.begin(); // start OLED display

    ESPServer::launch();

    // Check if ESP has WiFi configured
    if (strlen(Config::ssid) == 0)
    {
        display.printMsg("Config mode");
        ESPClient::launchSoftAP("ESP-Config-WiFi");
        configMode();
    }

    display.printMsg("Connecting to target WiFi...");
    connection_status = ESPClient::connect(); // connect to the target WiFi

    if (connection_status == WL_CONNECTED)
    {
        display.setSSID(Config::ssid);
        display.setIP(ESPClient::IP);
        display.setWiFiStatus(WiFi.status());
        display.printMsg("Connected");

        ESPClient::downloadConfig(config_data);
    }
    else
    {
        display.printMsg("Failed");
        display.setSSID("N/A");
        display.setIP("000.000.0.0");
    }

    if (strlen(config_data) > 0)
    {
        Serial.println("Sending order to the salve...");
        Serial.println(config_data);
        i2c::sendOrder(config_data, i2c::Order::UPDATE_RTC);
    }
    i2c::clearBuffer();
}

void loop()
{
    ESPServer::server.handleClient();
    requestDataFromArduino();
}

void requestDataFromArduino()
{
    // check if data can be harvested if not show in s next trial
    if (millis() - last_data_harvest > DATA_HARVEST_OFFSET)
    {
        // walk-around for hanging I2C bus - restart
        i2c::begin();  

        display.printMsg("Getting sensors data...");
        last_data_harvest = millis();
        i2c::requestData(); // request data from arduino

        if (strlen(i2c::buffer) > 0)
        {
            sendData();
            i2c::clearBuffer();
        }

    }
    else
    {
        char msg[30];
        char sec[3];

        ltoa((DATA_HARVEST_OFFSET - (millis() - last_data_harvest)) / 1000L, sec, 10);
        sprintf(msg, "Next request in %s s", sec);
        display.printMsg(msg);
    }
}

void configMode()
{
    while(strlen(Config::ssid) == 0)
    {
        ESPServer::server.handleClient();
    }
    Serial.println("Exiting from config mode");
}

void sendData()
{
    display.printMsg("Sending data out...");
    if (ESPClient::sendData(i2c::buffer) == ESP_CLIENT_POST_STATUS_FAILURE)
    {
        display.printMsg("Error...");
    }

    display.printMsg("OK");
}