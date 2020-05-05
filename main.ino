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

#define DATA_HARVEST_OFFSET 10000L // get data from Arduino once a minute

unsigned long last_data_harvest = 0L;
wl_status_t connection_status;

Display display = Display::getInstance();
char config_data[256];

void setup()
{
    delay(3000);

    Serial.begin(9600); // for debugging purposes
    Config::load();     // load wifi config data
    i2c::begin();       // join i2c bus
    display.begin();

    // Check if ESP has WiFi configured
    if (!Config::ssid)
    {
        Serial.println("No SSID configured - starting in soft AP mode");
        ESPClient::launchSoftAP("ESP Config WiFi");
    }
    else
    {
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
    }
    ESPServer::launch();

    // if (strlen(config_data) > 0)
    // {
    // Serial.println("Sending order to the salve...");
    // Serial.println(config_data);
    // i2c::sendOrder(config_data, i2c::Order::UPDATE_RTC);
    // }    
    // i2c::clearBuffer();
}

void loop()
{
    ESPServer::server.handleClient();
    requestDataFromArduino();
}

void requestDataFromArduino()
{
    if (millis() - last_data_harvest > DATA_HARVEST_OFFSET)
    {
        display.printMsg("Getting sensors data...");
        last_data_harvest = millis();

        i2c::requestData(); // request data
        Serial.println(i2c::buffer);

        // display.printMsg("Sending data out...");
        // char buf[600];
        // sprintf(buf, "data=[%s]", i2c::buffer); // convert data from arduino into JSON array (add [,])
        // if (ESPClient::sendData(buf))
        // {
        //     display.printMsg("OK");
        // }
        // else
        // {
        //     display.printMsg("Error...");
        // }
        // i2c::clearBuffer();
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
