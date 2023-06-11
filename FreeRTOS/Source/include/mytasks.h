#ifndef __MYTASKS_H__
#define __MYTASKS_H__

#include "common.h"

/**
 * @brief Emulates temperature sensor readings and enqueue them into a system queue
*/
void xTaskTemperatureSensor( void *pvParameters );  //pvParameters is a value that is passed as the paramater to the created task. 
                                                    //If pvParameters is set to the address of a variable then the variable must still exist when 
                                                    //the created task executes - so it is not valid to pass the address of a stack variable.

/**
 * @brief Reads temperature sensor queue and pass a low pass filter into it
*/
void xTaskFilter( void *pvParameters );

#endif // __MYTASKS_H__