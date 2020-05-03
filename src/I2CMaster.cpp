#include "I2CMaster.h"

char i2c::buffer[I2C_BUFFER_SIZE];
i2c::TransmissionStep i2c::transmission_step;

void i2c::begin()
{
    Wire.begin(0, 2);
}

void i2c::requestData()
{
    int post_length = requestDataLength(); // get amount of the data to be send by the slave
    Serial.println(post_length);

    // process only if there is something to be read
    if (post_length > 0)
    {
        // requestData(post_length);
    }
}

void i2c::requestData(int amount)
{
    clearBuffer();

    int package_size = I2C_SLAVE_BUFFER_SIZE < amount ? I2C_SLAVE_BUFFER_SIZE : amount; // get next package size
    int received_bytes = 0;                                                             // amount of bytes already received from the slave
    int bytes_left = amount;                                                            // amount of bytes to be requested

    while (bytes_left > 0 & received_bytes < I2C_BUFFER_SIZE)
    {
        requestDataPackage(received_bytes, package_size); // get package from the slave

        received_bytes += package_size;       // calculate start for new data
        bytes_left = amount - received_bytes; // shrink the buffer

        package_size = I2C_SLAVE_BUFFER_SIZE < bytes_left ? I2C_SLAVE_BUFFER_SIZE : bytes_left; // calculate next package size
        delay(10);
    }
}

int i2c::requestDataLength()
{
    Wire.requestFrom(I2C_SLAVE_ADDRESS, 3);  // request up to 999 chars from the slave

    char len[4];  // placeholder for incoming data amount
    int i = 0;  // currently read char
    char c;  // read incoming byte

    while (Wire.available())
    {
        delay(1);
        c = Wire.read();
        len[i++] = c;
    }
    return atoi(len);  // convert char to int and return
}

void i2c::requestDataPackage(int received_bytes, int package_size)
{
    Wire.requestFrom(I2C_SLAVE_ADDRESS, package_size);
    char c; // incoming byte
    int i = 0;

    // read package from the slave
    while (Wire.available())
    {
        delay(1);
        c = Wire.read();

        buffer[received_bytes + i] = c; // write to the buffer
        i++;

        // avoid buffer overload
        if (received_bytes + i == I2C_BUFFER_SIZE)
        {
            break;
        }
    }
}

void i2c::sendOrder(char *order_msg, Order order_code)
{
    // check if order is not a rubbish
    if (order_code == NONE || order_code == UNKNOWN)
    {
        return;
    }

    int timeout_counter = 0;
    int package_start = 0;
    int package_size = 32; // inherit from the Wire.h from arduino

    clearBuffer();
    strcpy(buffer, "0");       // convert order into char
    strcat(buffer, ";");       // add separator
    strcat(buffer, order_msg); // paste order message
    Serial.println(buffer);

    uint8_t package_send_status;
    transmission_step = ONGOING;

    // send new data package only if i2c bus was closed previously
    while ((package_start < strlen(buffer)) & (i2c::buffer[package_start] != '\0'))
    {
        package_send_status = sendDataPackage(package_start, package_size);

        // check if package was not sent successfully or timeout occured
        if (package_send_status != 0 && timeout_counter++ >= I2C_TIMEOUT)
        {
            continue;
        }

        package_start += package_size; // increment next package start point
        timeout_counter = 0;           // restart timeout counter
        delay(10);                     // give a slave some time to process the package (obtained experymentally here)
    }

    while (closeTransmission() != FINISHED && timeout_counter++ < I2C_TIMEOUT)
    {
        delay(10); // give a slave some time to process the package
    }
    transmission_step = FINISHED; // finish the trasmission
}

uint8_t i2c::sendDataPackage(int start, int size)
{
    Wire.beginTransmission(I2C_SLAVE_ADDRESS); // start I2C connection with the slave

    // send package to the slave
    for (int i = 0; i < size; i++)
    {
        // if null termination char occurs end transmission
        if (buffer[i + start] == '\0')
            break;

        Wire.write(buffer[i + start]);
        delay(1);                            // wait for Arduino to read the byte
    }                                        // for
    uint8_t status = Wire.endTransmission(); // close I2C tranmission and get status
    return status;
}

bool i2c::closeTransmission()
{
    Wire.beginTransmission(I2C_SLAVE_ADDRESS); // start new connecion
    Wire.write("#");                           // send termination char

    delay(1);                           // wait for Arduino to read the byte
    return Wire.endTransmission() == 0; // check if tranmission ended with success
}

void i2c::clearBuffer()
{
    memset(buffer, 0, I2C_BUFFER_SIZE); // clear the buffer before sending the order
}