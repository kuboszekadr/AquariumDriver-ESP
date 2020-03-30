#include "src/I2CMaster.h"
#include "src/Config.h"

#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

#define DATA_HARVEST_OFFSET 60000L  // get data from Arduino once a minute

unsigned long last_data_harvest = 0L;

void setup()
{
    Serial.begin(9600);  // for debugging porpuses
    delay(7500);
    Serial.println("Serial test");

	// i2c::begin();  // join i2c bus

}

void loop()
{
    Serial.println("DUPA");
    delay(500);
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
