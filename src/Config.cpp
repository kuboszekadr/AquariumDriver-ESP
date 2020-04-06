#include "Config.h"

// WiFi configs
char Config::ssid[CONFIG_SSID_LENGTH];
char Config::pwd[CONFIG_PWD_LENGTH];

// DB config
char Config::device_id[CONFIG_DB_DEVICE_ID_LENGTH];
char Config::db_ip[CONFIG_DB_IP_LENGTH];
char Config::db_username[CONFIG_USERNAME_LENGTH];
char Config::db_pwd[CONFIG_DB_PWD_LENGTH];

void Config::load()
{
    loadWiFiConfig();
    loadDBConfig();
}

void Config::loadWiFiConfig()
{
    load(ssid, getEEPROMAddress(SSID));
    load(pwd, getEEPROMAddress(PWD));
}

void Config::loadDBConfig()
{
    load(device_id, getEEPROMAddress(DEVICE_ID));
    load(db_ip, getEEPROMAddress(DB_IP));
    load(db_username, getEEPROMAddress(DB_USERNAME));
    load(db_pwd, getEEPROMAddress(DB_PWD));
}

void Config::load(char *target, uint8_t address)
{
    EEPROM.begin(256);
    uint8_t c = EEPROM.read(address);
    int i = 0;
    while (c != 255)
    {
        c = EEPROM.read(i + address);
        target[i++] = char(c);
    }
    EEPROM.end();
    delay(100);
}

bool Config::update(Params param, const char *value)
{
    int length;

    // get param EEPROM address
    switch (param)
    {
    case SSID:
        length = CONFIG_SSID_LENGTH;
        break;
    case PWD:
        length = CONFIG_PWD_LENGTH;
        break;
    case DEVICE_ID:
        length = CONFIG_DB_DEVICE_ID_LENGTH;
        break;
    case DB_IP:
        length = CONFIG_DB_IP_LENGTH;
        break;
    case DB_USERNAME:
        length = CONFIG_USERNAME_LENGTH;
        break;
    case DB_PWD:
        length = CONFIG_DB_PWD_LENGTH;
        break;
    default:
        break;
    }

    int address = getEEPROMAddress(param);

    EEPROM.begin(512);
    delay(100);

    const char *c = value;
    int i = 0;
    // write value into EEPROM
    while (*c)
    {
        EEPROM.write(address++, *c++);
    }
    // close transaction
    EEPROM.write(address, '\0'); // add empty char at the end
    EEPROM.commit();
    EEPROM.end();

    delay(100);
    return true;
}

int Config::getEEPROMAddress(Params param)
{
    int address = 0;

    if (param >= PWD)
    {
        address += CONFIG_SSID_LENGTH;
    }
    if (param >= DEVICE_ID)
    {
        address += CONFIG_PWD_LENGTH;
    }
    if (param >= DB_IP)
    {
        address += CONFIG_DB_DEVICE_ID_LENGTH;
    }
    if (param >= DB_USERNAME)
    {
        address += CONFIG_DB_IP_LENGTH;
    }
    if (param >= DB_PWD)
    {
        address += CONFIG_USERNAME_LENGTH;
    }

    return address + param;
}