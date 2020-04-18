#include "Display.h"

Display &Display::getInstance()
{
    static Display instance;
    return instance;
}

void Display::begin()
{
    _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    _display.clearDisplay();
    _display.setTextColor(WHITE);
    _display.setTextSize(1);
    _display.print("Booting....");
    _display.display();
}

void Display::setSSID(char *ssid)
{
    memset(_ssid, 0, 33);
    strcpy(_ssid, ssid);
}

void Display::setIP(char *ip)
{
    memset(_ip, 0, 15);
    strcpy(_ip, ip);
}

void Display::setWiFiStatus(wl_status_t connection_status)
{
    _connection_status = connection_status;
}

void Display::show()
{
    _display.clearDisplay();

    printSSID();
    printIP();
    // printMsg();

    _display.display();
}

void Display::printSSID()
{
    char msg[30];
    sprintf(msg, "SSID: %s", _ssid);
    update(0, 0, msg);
}

void Display::printIP()
{
    char msg[30];
    sprintf(msg, "IP: %s", _ip);
    update(10, 0, msg);
}

void Display::printWiFiStatus()
{
    char msg[30];
    char status[10];
    switch (_connection_status)
    {
    case WL_IDLE_STATUS:
        strcpy(status, "Idle");
        break;
    case WL_CONNECT_FAILED:
        strcpy(status, "Failed");
        break;
    case WL_DISCONNECTED:
        strcpy(status, "Disconn");
        break;
    case WL_CONNECTION_LOST:
        strcpy(status, "Conn lost");
        break;
    case WL_CONNECTED:
        strcpy(status, "OK");
        break;
    default:
        strcpy(status, "Unknown");
        break;
    }
    sprintf(msg, "WiFi: %s", status);
    update(20, 0, msg);
}

void Display::printMsg(char *msg)
{
    _display.clearDisplay();

    printSSID();
    printIP();
    printWiFiStatus();

    update(50, 0, msg);

    _display.display();
}

void Display::update(int row, int col, char *msg)
{
    _display.setCursor(col, row);
    _display.print(msg);
}
