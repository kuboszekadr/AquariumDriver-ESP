#include "Config.h"

// WiFi configs
char Config::ssid[CONFIG_SSID_LENGTH];
char Config::pwd[CONFIG_PWD_LENGTH];

// DB config
char Config::api_host_ip[CONFIG_API_HOST_IP_LENGTH];
char Config::api_host_port[CONFIG_API_HOST_PORT_LENGTH];

char Config::api_device_id[CONFIG_API_DEVICE_ID_LENGTH];
char Config::api_device_name[CONFIG_API_DEVICE_NAME_LENGHT];

void Config::load()
{
    loadWiFiConfig();
    loadApiConfig();

    Serial.println(ssid);
    Serial.println(pwd);

    Serial.println(api_host_ip);
    Serial.println(api_host_port);

    Serial.println(api_device_id);
    Serial.println(api_device_name);
}

void Config::loadWiFiConfig()
{
    load(ssid, getEEPROMAddress(SSID));
    load(pwd, getEEPROMAddress(PWD));
}

void Config::loadApiConfig()
{
    Serial.println(getEEPROMAddress(API_HOST_IP));
    Serial.println(getEEPROMAddress(API_HOST_PORT));
    Serial.println(getEEPROMAddress(API_DEVICE_ID));
    Serial.println(getEEPROMAddress(API_DEVICE_NAME));

    load(api_host_ip, getEEPROMAddress(API_HOST_IP));
    load(api_host_port, getEEPROMAddress(API_HOST_PORT));

    load(api_device_id, getEEPROMAddress(API_DEVICE_ID));
    load(api_device_name, getEEPROMAddress(API_DEVICE_NAME));
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
    char *var;

    // get param EEPROM address
    switch (param)
    {
    case SSID:
        length = CONFIG_SSID_LENGTH;
        var = ssid;
        break;

    case PWD:
        length = CONFIG_PWD_LENGTH;
        var = pwd;
        break;

    case API_HOST_IP:
        length = CONFIG_API_HOST_IP_LENGTH;
        var = api_host_ip;
        break;

    case API_HOST_PORT:
        length = CONFIG_API_HOST_PORT_LENGTH;
        var = api_host_port;
        break;

    case API_DEVICE_ID:
        length = CONFIG_API_DEVICE_ID_LENGTH;
        var = api_device_id;
        break;

    case API_DEVICE_NAME:
        length = CONFIG_API_DEVICE_NAME_LENGHT;
        var = api_device_name;
        break;

    default:
        break;
    }

    if (strlen(value) >= length)
    {
        return false;
    }

    memset(var, 0, length); // clear the variable before update
    strcpy(var, value); // insert new content

    Serial.println("--------");
    Serial.println(value);
    Serial.println(var);
    Serial.println("--------");

    int address = getEEPROMAddress(param);

    EEPROM.begin(512);
    delay(20);

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

    delay(20);
    return true;
}

int Config::getEEPROMAddress(Params param)
{
    int address = 0;

    if (param >= PWD)
    {
        address += CONFIG_SSID_LENGTH;
    }
    if (param >= API_HOST_IP)
    {
        address += CONFIG_PWD_LENGTH;
    }
    if (param >= API_HOST_PORT)
    {
        address += CONFIG_API_HOST_IP_LENGTH;
    }
    if (param >= API_DEVICE_ID)
    {
        address += CONFIG_API_HOST_PORT_LENGTH;
    }
    if (param >= API_DEVICE_NAME)
    {
        address += CONFIG_API_DEVICE_ID_LENGTH;
    }

    Serial.println(address + param);
    return address + param;
}