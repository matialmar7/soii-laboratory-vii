#ifndef __STATS_H__
#define __STATS_H__

#include "common.h"
static const char * header  = "ID\tTask\tState\tPRIO\tMEM\tAbsTime\t  %Time\n";
static const char * divider = "-------------------------------------------------------";
/**
 * @brief Get the Run Time Stats objectFUncu
 * 
 * @param buffer 
 */
void getRunTimeStats(char *buffer);

/**
 * @brief OBTENGO EL ESTADO EN FUNCION DEN NUMERO QUE ME DEVUELVE UNA LETRA CORRESPONDIENTE
 * 
 * X = INDETERMINADO
 * B = BLOCKED
 * S = SUSPEND
 * R = RUNNING
 * 
 * @param state 
 * @param buf 
 */
void getStateString(UBaseType_t state, char *buf);

/**
 * @brief Obtengo Tiempo y el uso del CPU en porcentaje
 * 
 * @param dest 
 * @param runTime 
 * @param statsPorcentage 
 */
void getStatsCPU(char *dest, uint32_t runTime, uint32_t statsPorcentage);

/**
 * @brief Obtengo Memoria NO UTILIZADA 
 * 
 * @param dest 
 * @param usStackHighWaterMark 
 */
void getStatsMEM(char *dest, uint32_t usStackHighWaterMark);

/**
 * @brief Get the States Tasks object OBTENGO ID NAME STATE AND PRIO
 * 
 * @param dest 
 * @param TaskNum 
 * @param task_name 
 * @param state 
 * @param priority 
 */
void getStatesTasks(char *dest, UBaseType_t TaskNum, char *task_name, UBaseType_t state, UBaseType_t priority);
/*------------------------------------------------------------------------------*/
/*                              UTILS FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/
/**
 * @brief Funcion para transformar numero a char y que se muestre en la pantalla
 * Similar to itoa from stdlib
 * 
 * @param value 
 * @param base 
 * @return char* 
 */
char *itoa(int val, int base);

/**
 * @brief Funcion para transformar numero a char y que se muestre en la pantalla
 * 
 * @param value 
 * @param ptr 
 * @param base 
 * @return char* 
 */
char *ltoc(unsigned long value, char *ptr, int base);

/**
 * @brief Funcion que envia la data por uart
 * 
 * @param msg 
 */
void print(const char *msg);

#endif // __STATS_H__