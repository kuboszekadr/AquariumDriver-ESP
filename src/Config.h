#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>

#define CONFIG_SSID_LENGTH 33
#define CONFIG_PWD_LENGTH 33

#define CONFIG_API_HOST_IP_LENGTH 16
#define CONFIG_API_HOST_PORT_LENGTH 6

#define CONFIG_API_DEVICE_ID_LENGTH 3
#define CONFIG_API_DEVICE_NAME_LENGHT 20

namespace Config
{
    enum Params
    {
        SSID = 0,
        PWD,
        
        API_HOST_IP,
        API_HOST_PORT,

        API_DEVICE_ID,
        API_DEVICE_NAME
    };

    // WiFi config
    extern char ssid[CONFIG_SSID_LENGTH]; // wifi name
    extern char pwd[CONFIG_PWD_LENGTH];   // wifi password

    // API config
    extern char api_host_ip[CONFIG_API_HOST_IP_LENGTH];
    extern char api_host_port[CONFIG_API_HOST_PORT_LENGTH];
    
    extern char api_device_id[CONFIG_API_DEVICE_ID_LENGTH];
    extern char api_device_name[CONFIG_API_DEVICE_NAME_LENGHT];

    void load();                              // load whole data
    void load(char *target, uint8_t address); // load EEPROM data into variable

    void loadWiFiConfig();
    void loadApiConfig();

    bool update(Params param, const char *value); // update EEPROM variable
    int getEEPROMAddress(Params param);
} // namespace Config
#endif