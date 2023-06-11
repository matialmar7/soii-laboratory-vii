#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "common.h"

/* Environment includes. */
#include "DriverLib.h"

uint8_t init(void);

//From RTOS Demo
static void prvSetupHardware( void );
void vGPIO_ISR( void );
void vUART_ISR( void );

#endif // __CONFIG_H__