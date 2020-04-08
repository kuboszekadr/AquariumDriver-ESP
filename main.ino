#include "src/Config.h"
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

#define DATA_HARVEST_OFFSET 60000L // get data from Arduino once a minute

unsigned long last_data_harvest = 0L;
wl_status_t connection_status;

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
        ESPClient::launchSoftAP("ESP Config WiFi");
    }
    else
    {
        Serial.println("Connecting to target WiFi...");
        connection_status = ESPClient::connect(); 
        if (connection_status == WL_CONNECTED)
        {
            Serial.println("Connection sucessfull");
        }
        else
        {
            Serial.print("Connection failed, error code: ");
            Serial.println(connection_status);
        }
    }
    ESPServer::launch();
}

void loop()
{
    ESPServer::server.handleClient();
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
