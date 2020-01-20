#ifndef I2CMaster_h
#define I2CMaster_h

#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 8  // only one slave can be attached
#define I2C_TIMEOUT 10  // number of trials 
#define I2C_SLAVE_BUFFER_SIZE 32  // amount of bytes that can be sent to the slave

#define I2C_ORDER_OFFSET 5000L
#define I2C_REQUEST_OFFSET 10000L

namespace i2c
{
    enum Order
    {
        UNKNOWN = -1,
        UPDATE_RTC,
        UPDATE_WIFI_STATUS,  
        WATER_CHANGE
    };

    enum TransmissionStep
    {
        EMPTY = -1,  // to be set separatelly when slave is done with parsing the command
        ONGOING,  // transmission is in progress
        FINISHED  // all data received from the master
    };

    extern char buffer[512];  // maximum amount of the data that can be sent from the slave
    // extern int requestDataLength;
    extern TransmissionStep transmissionStep;

    void begin();  // join I2C bus

    int requestDataLength();  // get amount of the data available by the slave
    void requestData();  // starts data transmission
    void requestData(int amount);  // starts data transmission
    void requestDataPackage(int received_bytes, int package_size); 

    void sendOrder();  // sends order to the slave
    uint8_t sendDataPackage(int start, int size);

    bool closeTransmission();
}


#endif