

/******************************************************************************
*
*   Name: "TempLogger.c"
*   Author: njm
*
*   Description:    Worker file to read sensor value and output it to file.
*
*                   Written for UNIX
*
*   Revision Hist:  29-08-2020 V1.0 - Initial methods written.
*
*
*   ToDO: Move Some of the definitions to ArgV values;
******************************************************************************/ 


#include "DS18B20.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

//Eventually convert to argc/argv input
#define MY_SENSOR "28-011927419e6a"
#define WRITE_OUT "/home/njm/scratch/TEMP.OUT"
#define OUTPUT_DIV "----------------------------------------------------\n"
#define COLUMN_HEADER "Temperature, Difference\n"
#define ITERATIONS 0 //Set to 0 for unlimited runtime.
#define INTERVAL 30

float previous_temperature = 0;
float current_temperature = 0;
float difference = 0;
struct tm * timeinfo;

int32_t InitialiseOutFile()
{

    FILE * fp = fopen(WRITE_OUT, "a");

    if(fp == NULL){
        return FILE_NOT_FOUND;
    }
    
    int rtn_code_write = fputs(OUTPUT_DIV, fp);
    rtn_code_write = fputs(COLUMN_HEADER, fp);    
    int rtn_code_close = fclose(fp);

    if(rtn_code_close != 0 || rtn_code_write == EOF)
    {
        printf("File closure failure \n");
        return FILE_IO_ERROR;
    }

    return SUCCESS;
}

int32_t WriteReading(){

    FILE * fp = fopen(WRITE_OUT, "a");

    if(fp == NULL){
        return FILE_NOT_FOUND;
    }
    
    int rtn_code_write = fprintf(fp, "%f, %f, %s",current_temperature, difference, asctime (timeinfo));
    int rtn_code_close = fclose(fp);

    if(rtn_code_close != 0 || rtn_code_write == EOF)
    {
        printf("File closure failure \n");
        return FILE_IO_ERROR;
    }

    return SUCCESS;
}

int32_t QuerySensor()
{
    sleep(INTERVAL);
    current_temperature= GetProcessedTemperatureInCelcius(); 

#ifdef DEBUG
    printf("%f\n", current_temperature);
#endif

    if(current_temperature < (float)ERROR_CODE_START)
    {
        return (int32_t)current_temperature;
    }

    time_t rawtime;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    difference = current_temperature - previous_temperature;
    return WriteReading();

}

int32_t BeginMonitorRoutine(){

    size_t iterations = ITERATIONS;
    do
    {
        int err = QuerySensor();    
        if(err < ERROR_CODE_START){
            return err;
        }
	previous_temperature = current_temperature;
    }
    while(iterations-- != 0 || ITERATIONS == 0);


}

int main(int argc,char* argv[])
{

    int error = Initialise(MY_SENSOR);
    
    if(error == SUCCESS){
        previous_temperature = GetProcessedTemperatureInCelcius();

    } else {
        printf("Failed to initialise sensor. Please check and try again.");
        return -1; 
    }

    error = InitialiseOutFile();
    if(error == SUCCESS){
        error = BeginMonitorRoutine();
    } else {
        printf("Failed to open OUTPUT file. Please check and try again.");
        return -1; 
    }


    return error; 

    
}
