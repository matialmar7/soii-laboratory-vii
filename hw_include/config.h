#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "common.h"

/* UART configuration - note this does not use the FIFO so is not very
efficient. */
#define mainBAUD_RATE				( 19200 )
#define mainFIFO_SET				( 0x10 )


/* The set frequency of the interrupt.  Deviations from this are measured as
the jitter. */
#define timerINTERRUPT_FREQUENCY		( 20000UL )

/* The expected time between each of the timer interrupts - if the jitter was
zero. */
#define timerEXPECTED_DIFFERENCE_VALUE	( configCPU_CLOCK_HZ / timerINTERRUPT_FREQUENCY )

/* The highest available interrupt priority. */
#define timerHIGHEST_PRIORITY			( 0 )

/* Misc defines. */
#define timerMAX_32BIT_VALUE			( 0xffffffffUL )
#define timerTIMER_1_COUNT_VALUE		( * ( ( volatile unsigned long * ) ( ( unsigned long ) TIMER1_BASE + 0x48UL ) ) )


//Task priorities
#define mainTASK_PRIORITY (tskIDLE_PRIORITY + 2)

uint8_t init(void);

//From RTOS Demo
static void prvSetupHardware( void );
void vSetupHighFrequencyTimer(void);
void vGPIO_ISR( void );
void vUART_ISR( void );
void Timer0IntHandler(void);

#endif // __CONFIG_H__