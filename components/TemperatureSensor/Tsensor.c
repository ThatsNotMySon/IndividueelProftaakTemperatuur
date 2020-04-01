//Written by: Tom Kaasenbrood
//WRITTEN FULLY

#include "Tsensor.h"
#include "mcp23017.h"

mcp23017_t* mcp23017;

void tempInit(mcp23017_t *mcp)
{
    mcp23017 = mcp;
}

//return values:
//TEMPSENSOR_OK : no errors
//TEMPSENSOR_ERROR_CHECKSUM : error occurred in checksum
//TEMPSENSOR_ERROR_TIMEOUT : timed out
int readTemp11(uint8_t pin)
{
    int returnvalue = _readSensorTemp(pin, TEMPSENSOR_DHT11_WAKEUP);
    if (returnvalue != TEMPSENSOR_OK)
    {
        humidity = TEMPSENSOR_INVALID_VALUE;
        temperature = TEMPSENSOR_INVALID_VALUE;
        return returnvalue;
    }

    //convert and store recieved data
    humidity = bitsTemp[0];
    temperature = bitsTemp[2];

    //test results
    uint8_t sum = bitsTemp[0] + bitsTemp[2];
    if (bitsTemp[4] != sum)
    {
        return TEMPSENSOR_ERROR_CHECKSUM;
    }
    return TEMPSENSOR_OK;
}

//return values:
//TEMPSENSOR_OK : no errors
//TEMPSENSOR_ERROR_CHECKSUM : error occurred in checksum
//TEMPSENSOR_ERROR_TIMEOUT : timed out
int readTemp(uint8_t pin)
{
    //read values
    int returnvalue = _readSensorTemp(pin, TEMPSENSOR_DHT_WAKEUP);
    if (returnvalue != TEMPSENSOR_OK)
    {
        humidity = TEMPSENSOR_INVALID_VALUE;
        temperature = TEMPSENSOR_INVALID_VALUE;
        return returnvalue;
    }

    //convert and store recieved data
    // humidity = word(bitsTemp[0], bitsTemp[1]) * 0.1;
    // temperature = word(bitsTemp[2] & 0x7F, bitsTemp[3]) * 0.1;
    if (bitsTemp[2] & 0x80)
    {
        temperature = -temperature;
    }

    //test results
    uint8_t sum = bitsTemp[0] + bitsTemp[1] + bitsTemp[2] + bitsTemp[3];
    if (bitsTemp[4] != sum)
    {
        return TEMPSENSOR_ERROR_CHECKSUM;
    }
    return TEMPSENSOR_OK;
}

//return values:
//TEMPSENSOR_OK : no errors
//TEMPSENSOR_ERROR_TIMEOUT : timed out
int _readSensorTemp(uint8_t pin, uint8_t wakeupDelay)
{
    //empty buffer
    for (uint8_t i = 0; i < 5; i++)
    {
        bitsTemp[i] = 0;
    }

    //request
    //Error with GPIO expander, unable to make it work in available time
    mcp23017_write_register(mcp23017,
    MCP23017_IODIR,
    GPIOA,
    0x00);
    mcp23017_set_bit(mcp23017,
    0,
    MCP23017_GPIO,
    GPIOA);
    vTaskDelay(50);
    mcp23017_set_bit(mcp23017,
    1,
    MCP23017_GPIO,
    GPIOA);
    vTaskDelay(50);
    mcp23017_write_register(mcp23017,
    MCP23017_IODIR,
    GPIOA,
    0xFF);

    //get output
    uint8_t data = 0x00;
    mcp23017_read_register(mcp23017,
    MCP23017_GPIO,
    GPIOA,
    &data);
    
    mcp23017_write_register(mcp23017,
    MCP23017_IODIR,
    GPIOA,
    0x00);
    
    mcp23017_set_bit(mcp23017,
    1,
    MCP23017_GPIO,
    GPIOA);

    return TEMPSENSOR_OK;
}