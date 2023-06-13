#ifndef __MYTASKS_H__
#define __MYTASKS_H__

#include "common.h"

/*------------------------------------------------------------------------------*/
/*                                  TASKS                                       */
/*------------------------------------------------------------------------------*/

/**
 * @brief Emulates temperature sensor readings and enqueue them into a system queue
*/
void xTaskTemperatureSensor( void *pvParameters );  //pvParameters is a value that is passed as the paramater to the created task. 
                                                    //If pvParameters is set to the address of a variable then the variable must still exist when 
                                                    //the created task executes - so it is not valid to pass the address of a stack variable.

/**
 * @brief Reads temperature sensor queue and pass a filter to it
*/
void xTaskFilter( void *pvParameters );

/**
 * @brief Displays the temperature thru time
*/
void xTaskDisplay( void *pvParameters );

/**
 * @brief Displays the process and cpu information
*/
void xTaskTop( void *pvParameters );


/*------------------------------------------------------------------------------*/
/*                              GRAPH FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/

void plot(uint8_t * image, uint8_t value);


/*------------------------------------------------------------------------------*/
/*                              UTILS FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/
/**
 * @brief Funcion para transformar numero a char y que se muestre en la pantalla
 * 
 * @param value 
 * @param ptr 
 * @param base 
 * @return char* 
 */
char *intToStr(int val, int base);

/**
 * @brief Funcion que envia la data por uart
 * 
 * @param msg 
 */
void print(const char *msg);

#endif // __MYTASKS_H__


