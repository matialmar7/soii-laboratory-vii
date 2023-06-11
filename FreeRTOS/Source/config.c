#include "config.h"

__UINT8_TYPE__ init(void)
{
    prvSetupHardware();

    xTempQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
}


static void prvSetupHardware( void )
{
	/* Setup the PLL. */
	SysCtlClockSet( SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ );

	/* Setup the push button. */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //GPIODirModeSet(GPIO_PORTC_BASE, mainPUSH_BUTTON, GPIO_DIR_MODE_IN);
	//GPIOIntTypeSet( GPIO_PORTC_BASE, mainPUSH_BUTTON,GPIO_FALLING_EDGE );
	IntPrioritySet( INT_GPIOC, configKERNEL_INTERRUPT_PRIORITY );
	//GPIOPinIntEnable( GPIO_PORTC_BASE, mainPUSH_BUTTON );
	IntEnable( INT_GPIOC );



	/* Enable the UART.  */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	/* Set GPIO A0 and A1 as peripheral function.  They are used to output the
	UART signals. */
	GPIODirModeSet( GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_HW );

	/* Configure the UART for 8-N-1 operation. */
	//UARTConfigSet( UART0_BASE, mainBAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE );

	/* We don't want to use the fifo.  This is for test purposes to generate
	as many interrupts as possible. */
	//HWREG( UART0_BASE + UART_O_LCR_H ) &= ~mainFIFO_SET;

	/* Enable Tx interrupts. */
	HWREG( UART0_BASE + UART_O_IM ) |= UART_INT_TX;
	IntPrioritySet( INT_UART0, configKERNEL_INTERRUPT_PRIORITY );
	IntEnable( INT_UART0 );

}

/*-----------------------------------------------------------*/

void vUART_ISR(void)
{
    //
    //unsigned long ulStatus;

	///* What caused the interrupt. */
	//ulStatus = UARTIntStatus( UART0_BASE, pdTRUE );

	///* Clear the interrupt. */
	//UARTIntClear( UART0_BASE, ulStatus );

	///* Was a Tx interrupt pending? */
	//if( ulStatus & UART_INT_TX )
	//{
	//	/* Send the next character in the string.  We are not using the FIFO. */
	//	if( *pcNextChar != 0 )
	//	{
	//		if( !( HWREG( UART0_BASE + UART_O_FR ) & UART_FR_TXFF ) )
	//		{
	//			HWREG( UART0_BASE + UART_O_DR ) = *pcNextChar;
	//		}
	//		pcNextChar++;
	//	}
	//}
    //
}
/*-----------------------------------------------------------*/

void vGPIO_ISR( void )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Clear the interrupt. */
	//GPIOPinIntClear(GPIO_PORTC_BASE, mainPUSH_BUTTON);

	/* Wake the button handler task. */
	//xSemaphoreGiveFromISR( xButtonSemaphore, &xHigherPriorityTaskWoken );

	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/
