#include "mytasks.h"
#include "rand.h"

void xTaskTemperatureSensor( void *pvParameters )
{
    uint8_t temperatura = 0;
    uint8_t temperatura_base = 15;
    for(;;)
    {
        temperatura = (uint8_t)(rand() % 15) + temperatura_base;

        //Enqueue
        xQueueSend(xTempQueue, &temperatura, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete( NULL );
}

void xTaskFilter( void *pvParameters )
{
    #define N_MAX 20

    uint8_t temp_in_second[N_MAX] = {0};
    uint8_t N_FIL = N_MAX;
    uint8_t N_UART = 0;
    uint8_t last_temp = 0;

    for(;;)
    {
        xQueueReceive(xTempQueue, &last_temp, portMAX_DELAY);
        xQueueReceive(xUARTQueue, &N_UART, 0);                      // Check UART N Value
        //Check N_UART <= N_MAX
        if (N_UART > 0 && N_UART < N_MAX) 
            N_FIL = N_UART;

        //Push into queue
        for(uint8_t i=(N_FIL-1); i>0; i--)
            temp_in_second[i] = temp_in_second[i-1]; //This could be replaced with a bitwise operation.
        temp_in_second[0] = last_temp;

        //Calculate mean
        uint16_t sum = 0;
        for (uint8_t i = 0; i < N_FIL; i++)
            sum += temp_in_second[i];
        uint8_t mean = sum / N_FIL;

        xQueueSend(xDisplayQueue, &mean, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    #undef N_MAX
    vTaskDelete( NULL );
}

void xTaskDisplay( void *pvParameters )
{
    // We are implementing this in the OSRAM96x16 OLED Display
    /**
     * Se utilizara un display de 2 filas, de 16 x 96. Con cada fila de 8x96
    */
    uint8_t graph[192] = {0};
    uint8_t temp = 0; 
    /*Clear the OLED Display*/
    OSRAMClear();
    for (;;)
    {
        xQueueReceive(xDisplayQueue, &temp, portMAX_DELAY);

        plot(graph, temp);
        OSRAMImageDraw(graph, 15, 0, 96, 2);
        OSRAMStringDraw(intToStr(temp,10), 84, 2);
        OSRAMStringDraw("t", 10, 1);
    }
    vTaskDelete( NULL );
}

void xTaskTop( void *pvParameters )
{
    char data[400] = {0};    // Varaible que almacena la data para enviar 
    
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete( NULL );
}

/*------------------------------------------------------------------------------*/
/*                              GRAPH FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/

void plot(uint8_t * image, uint8_t value)
{
    uint8_t new_value = 0;

    if (value < 16) //Primera fila
    {
      image[1] = 0b00000000;
      new_value = ((0b10000000 >> (int)(value / 2)) |0b10000000);
    }
    else //Segunda fila
    {
      image[1] = (0b10000000 >> (int)((value - 16) / 2)) ;
      new_value = 0b10000000;
    }

    //Actualizar los valores
    for (int i = 191; i > 1; i--) 
        if( i == 97)
            image[i] = new_value; //primer pixel de la segunda fila
        else 
            image[i] = (image[i - 1]);

    return;
}

/*------------------------------------------------------------------------------*/
/*                              UTILS FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/

//Similar to itoa from stdlib
char *intToStr(int val, int base)
{

    static char buf[32] = {0};

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];
}

void print(const char *msg)
{
    do{
        UARTCharPut(UART0_BASE, *msg);
    }while (*(msg++) != 0x00);
}
