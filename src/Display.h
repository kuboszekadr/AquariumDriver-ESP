#ifndef Display__h
#define Display__h

#include "Config.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

// to change Namespace into class
class Display
{
public:
    static Display &getInstance();

    void begin();

    void setSSID(char *ssid);
    void setIP(char *ip);
    void setWiFiStatus(wl_status_t connection_status);
    void printMsg(char *msg);

    void show();

private:
    Display() {};

    char _ssid[33];
    char _ip[15];
    char _msg[4][40];

    wl_status_t _connection_status = WL_IDLE_STATUS;

    Adafruit_SSD1306 _display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void printSSID();
    void printIP();
    void printWiFiStatus();
    // void printMsg(char *msg);
    void update(int row, int col, char *msg);
}
;
#endif