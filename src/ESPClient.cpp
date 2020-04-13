#include "ESPClient.h"

char ESPClient::IP[15];

void ESPClient::launchSoftAP(char *ssid)
{
    IPAddress local_IP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 4, 9);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid);
}

wl_status_t ESPClient::connect()
{
    // return false if SSID is empty
    if (!Config::ssid)
    {
        Serial.println("SSID empty or not initalize in config");
        return WL_NO_SHIELD;  // Error
    }

    // Connect to the target WiFi network
    WiFi.begin(Config::ssid, Config::pwd);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        timeout++;
        delay(500);
        if (timeout > 100)
        {
            return WiFi.status();
        }
    }

    WiFi.localIP().toString().toCharArray(IP,  14);

    // If sucessfully connected, disable softAP mode
    WiFi.softAPdisconnect(true);
    return WL_CONNECTED;
}