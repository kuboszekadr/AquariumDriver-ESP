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

bool ESPClient::downloadConfig(char *target)
{
    HTTPClient client;

// TODO: change to variable
    client.begin("http://192.168.0.179/get_date");
    int response_code = client.GET();       // download config from the server

    if (response_code != 200)
    {
        return false;
    }
    
    client.getString().toCharArray(target, 255);
    client.end();                           // close the connection

    return true;
}

bool ESPClient::sendData(const char *data)
{
    HTTPClient client;

    // TODO: change to variable
    client.begin("http://192.168.0.179/data_collector");
    client.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int response_code = client.POST(data);

    client.end();
    return true;
}

wl_status_t ESPClient::connect()
{
    // return false if SSID is empty
    if (!Config::ssid)
    {
        Serial.println("SSID empty or not initalize in config");
        return WL_NO_SHIELD; // Error
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

    WiFi.localIP().toString().toCharArray(IP, 14);

    // If sucessfully connected, disable softAP mode
    WiFi.softAPdisconnect(true);
    return WL_CONNECTED;
}