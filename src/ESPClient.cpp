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
    char url[50] = {};
    sprintf(url, "http://%s:%s/api/date", Config::api_host_ip, Config::api_host_port);

    client.begin(url);
    int response_code = client.GET(); // download config from the server

    if (response_code != 200)
    {
        Serial.print("Error... \t");
        Serial.println(response_code);

        client.end();
        return false;
    }
 
    client.getString().toCharArray(target, 255);
    client.end(); // close the connection
    return true;
}

uint8_t ESPClient::sendData(const char *data)
{
    // make sure that empty string will not be sent
    if (strlen(data) == 0)
    {
        return ESP_CLIENT_POST_STATUS_NO_DATA;
    }
    HTTPClient client;

    char url[50] = {};
    sprintf(url, "http://%s:%s/api/data_collector", Config::api_host_ip, Config::api_host_port);
    Serial.println(url);

    client.addHeader("Content-Type", "application/x-www-form-urlencoded");
    client.addHeader("Accept", "*/*");
    client.addHeader("Accept-Encoding", "gzip, deflate, br");

    // add missing data to the request form
    char *request = new char[strlen(data) + 50];
    sprintf_P(request, "data={\"device_id\":%d,\"data\":%s}", ESP_CLIENT_DEVICE_ID, data);

    // send request
    int response_code = client.POST(request);
    delete request;

    // close connection and return status
    client.end();
    return response_code == 200 ? ESP_CLIENT_POST_STATUS_SUCCESS : ESP_CLIENT_POST_STATUS_FAILURE;
}

wl_status_t ESPClient::connect()
{
    // return false if SSID is empty
    if (!Config::ssid)
    {
        Serial.println("SSID empty");
        return WL_NO_SSID_AVAIL; // Error
    }

    // Connect to the target WiFi network
    if (strlen(Config::api_device_name) > 0)
    {
        wifi_station_set_hostname(Config::api_device_name);
    }

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