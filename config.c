#include "config.h"
#include "mytasks.h"

/* Stores the value of the maximum recorded jitter between interrupts. */
volatile unsigned long ulMaxJitter = 0UL;

/* Counts the total number of times that the high frequency timer has 'ticked'.
This value is used by the run time stats function to work out what percentage
of CPU time each task is taking. */
volatile unsigned long ulHighFrequencyTimerTicks = 0UL;

uint8_t init(void)
{
    //Initialize hardware
    prvSetupHardware();
    vSetupHighFrequencyTimer();

    xTempQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    xUARTQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    xDisplayQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    xTaskCreate(xTaskTemperatureSensor, "GetTemp", 25, NULL, mainTASK_PRIORITY, NULL);
    xTaskCreate(xTaskFilter, "Filter", 47, NULL, mainTASK_PRIORITY, NULL);   
    xTaskCreate(xTaskDisplay, "Display", 90, NULL, mainTASK_PRIORITY, NULL);    
    xTaskCreate(xTaskTop, "Top", 53, NULL, mainTASK_PRIORITY, NULL);    
    return 0;    
}

/**
 * @brief Copied from LM3S811 demo... It just setup the hardware
*/
static void prvSetupHardware(void)
{
    /* Setup the PLL. */
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);

    /* Enable the UART.  */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Set GPIO A0 and A1 as peripheral function.  They are used to output the
    UART signals. */
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_HW);

    /* Configure the UART for 8-N-1 operation. */
    UARTConfigSet(UART0_BASE, mainBAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE);

    /* We don't want to use the fifo.  This is for test purposes to generate
    as many interrupts as possible. */
    HWREG(UART0_BASE + UART_O_LCR_H) &= ~mainFIFO_SET;

    /* Enable Tx interrupts. */
    HWREG(UART0_BASE + UART_O_IM) |= UART_INT_RX;
    IntPrioritySet(INT_UART0, configKERNEL_INTERRUPT_PRIORITY);
    IntEnable(INT_UART0);

    /* Initialise the LCD> */
    OSRAMInit(true);
}

/**
 * @brief This is from FreeRTOS Demo CORTEX_LM3SXXX timertest.c 
*/
void vSetupHighFrequencyTimer(void)
{
    unsigned long ulFrequency;

    //Como dice la docu inicilizo el timer 0 para generar interruciones y el timmer 1 es para el jitter
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    
    /*8.4 Initialization and Configuration from Documentation*/
    TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);

    // Le pongo la prioridad al timer cero como ALTA
    IntPrioritySet(INT_TIMER0A, timerHIGHEST_PRIORITY);

    /* Just used to measure time. */
    TimerLoadSet(TIMER1_BASE, TIMER_A, timerMAX_32BIT_VALUE);

    /* Me aseguro que la interrucion no salte mientras esta ejecutando la tarea*/
    portDISABLE_INTERRUPTS();

    /* la taza con la cual salta la interruocion */
    ulFrequency = configCPU_CLOCK_HZ / timerINTERRUPT_FREQUENCY;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ulFrequency);
    IntEnable(INT_TIMER0A);
    //Habilito la interrupcion por timer0
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* Enable both timers. */
    TimerEnable(TIMER0_BASE, TIMER_A);
}


/*-----------------------------------------------------------*/

void vUART_ISR(void)
{
    
    unsigned long ulStatus;
    uint8_t value;

	/* What caused the interrupt. */
	ulStatus = UARTIntStatus( UART0_BASE, pdTRUE );
	/* Clear the interrupt. */
	UARTIntClear( UART0_BASE, ulStatus );

	/* Was a Tx interrupt pending? */
	if( ulStatus & UART_INT_TX )
	{
        value = (uint8_t)UARTCharGet(UART0_BASE);
        xQueueSendFromISR(xUARTQueue, &value, 0);
	}
    
}

/**
 * @brief Handler del timer
 * From Cortex_LM3SXXX Demo
 */
void Timer0IntHandler(void)
{
    unsigned long ulDifference;
    volatile unsigned long ulCurrentCount;
    static unsigned long ulMaxDifference = 0, ulLastCount = 0;

    /* We use the timer 1 counter value to measure the clock cycles between
    the timer 0 interrupts. */
    ulCurrentCount = timerTIMER_1_COUNT_VALUE;

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if (ulCurrentCount < ulLastCount)
    {
        /* How many times has timer 1 counted since the last interrupt? */
        ulDifference = ulLastCount - ulCurrentCount;

        /* Is this the largest difference we have measured yet? */
        if (ulDifference > ulMaxDifference)
        {
            ulMaxDifference = ulDifference;
            ulMaxJitter = ulMaxDifference - timerEXPECTED_DIFFERENCE_VALUE;
        }
    }

    ulLastCount = ulCurrentCount;

    /* Keep a count of the total number of 20KHz ticks.  This is used by the
    run time stats functionality to calculate how much CPU time is used by
    each task. */
    ulHighFrequencyTimerTicks++;
}
/*-----------------------------------------------------------*/
