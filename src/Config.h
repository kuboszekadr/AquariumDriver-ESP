#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>

#define CONFIG_SSID_LENGTH 33
#define CONFIG_PWD_LENGTH 33

#define CONFIG_DB_DEVICE_ID_LENGTH 2
#define CONFIG_DB_IP_LENGTH 16
#define CONFIG_USERNAME_LENGTH 20
#define CONFIG_DB_PWD_LENGTH 20

namespace Config
{
enum Params
{
    SSID = 0,
    PWD,
    DEVICE_ID,
    DB_IP,
    DB_USERNAME,
    DB_PWD
};

// WiFi config
extern char ssid[CONFIG_SSID_LENGTH]; // wifi name
extern char pwd[CONFIG_PWD_LENGTH];   // wifi password

// DB config
extern char device_id[CONFIG_DB_DEVICE_ID_LENGTH]; // device ID visible in database
extern char db_ip[CONFIG_DB_IP_LENGTH];            // IP of database
extern char db_username[CONFIG_USERNAME_LENGTH];   // DB username
extern char db_pwd[CONFIG_DB_PWD_LENGTH];          // DB pasword

void load();                              // load whole data
void load(char *target, uint8_t address); // load EEPROM data into variable

void loadWiFiConfig();
void loadDBConfig();

bool update(Params param, const char *value); // update EEPROM variable
int getEEPROMAddress(Params param);
} // namespace Config
#endif