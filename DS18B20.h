

/******************************************************************************
*
*   Name: "DS18B20.h"
*   Author: njm
*
*   Description:    File containing methods to interpret the output of the 
*                   DS18B20 sensor, and return it as double or raw. Sensor 
*                   ID is set in a init method.
*
*
*                   Written for UNIX
*
*   Revision Hist:  29-08-2020 V1.0 - Initial methods written.
*
******************************************************************************/ 

#ifndef _DS18B20
#define _DS18B20

#define SUCCESS 0
#define ERROR_CODE_START -195
#define FILE_NOT_FOUND -200
#define FILE_IO_ERROR -201


//#define TEST_ENV
#define DEBUG


#include <stdint.h>


/* Method sets "sensor id" to query for other menus*/
int16_t Initialise(const char * sensor_id);

/* Get raw value */
int32_t GetRawValue(); 

/* Get current temperature in C. */
float   GetProcessedTemperatureInCelcius();

#endif //_DS18B20
