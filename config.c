#include "config.h"
#include "mytasks.h"

__UINT8_TYPE__ init(void)
{
    //Initialize hardware
    prvSetupHardware();

    xTempQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    xUARTQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    xDisplayQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    xTaskCreate(xTaskTemperatureSensor, "GetTemp", configMINIMAL_STACK_SIZE, NULL, mainTASK_PRIORITY, NULL);
    xTaskCreate(xTaskFilter, "Filter", configMINIMAL_STACK_SIZE, NULL, mainTASK_PRIORITY, NULL);   
    xTaskCreate(xTaskDisplay, "Display", 100, NULL, mainTASK_PRIORITY, NULL);    
    
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
/*-----------------------------------------------------------*/