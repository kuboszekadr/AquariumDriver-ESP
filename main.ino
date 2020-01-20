#include "src/I2CMaster.h"
#include <Wire.h>

unsigned long lastMillis = 0;

void setup()
{
    Serial.begin(9600);  // for debugging porpuses
	i2c::begin();  // join i2c bus
}

void loop()
{
    unsigned long currentMillis = millis();

	if (abs(currentMillis - lastMillis) > 500L)
    {
        lastMillis = millis();
        Serial.println("Sending order to the slave...");
        memset(i2c::buffer, 0, 512);
        strcpy(i2c::buffer, "1;20200117223344");
        i2c::sendOrder();
        Serial.println("Done.");
    }
}
