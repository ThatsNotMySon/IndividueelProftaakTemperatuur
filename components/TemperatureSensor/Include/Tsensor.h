//written by: Tom Kaasenbrood
//FULLY WRITTEN
#ifndef TSENSOR_H
#define TSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//generic data from sensor
#define TEMPSENSOR_OK                0
#define TEMPSENSOR_ERROR_CHECKSUM   -1
#define TEMPSENSOR_ERROR_TIMEOUT    -2
#define TEMPSENSOR_INVALID_VALUE    -999

#define TEMPSENSOR_DHT11_WAKEUP     18
#define TEMPSENSOR_DHT_WAKEUP       1

//sensor timeout
#define TEMPSENSOR_TIMEOUT (F_CPU/40000)

//function prototypes
void tempInit();
int readTemp11(uint8_t pin);
int readTemp(uint8_t pin);
inline int readTemp21(uint8_t pin) { return readTemp(pin); };
inline int readTemp22(uint8_t pin) { return readTemp(pin); };
inline int readTemp33(uint8_t pin) { return readTemp(pin); };
inline int readTemp44(uint8_t pin) { return readTemp(pin); };
double humidity;
double temperature;

uint8_t bitsTemp[5];
int _readSensorTemp(uint8_t pin, uint8_t wakeupDelay);

#ifdef __cplusplus
}
#endif

#endif