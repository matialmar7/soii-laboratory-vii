#include "mytasks.h"
#include "rand.h"

void xTaskTemperatureSensor( void *pvParameters )
{
    /* Calculo de stack
    temp = 1
    for  = 4
    xQueSnd = 4
    xTskDel = 4
    xTskDlt = 4
    -----------
    Total = 17
    */
    uint8_t temp = 0;

    for(;;)
    {
        temp = (uint8_t)(rand() % 40) + 1;

        //Enqueue
        xQueueSend(xTempQueue, &temp, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete( NULL );
}

void xTaskFilter( void *pvParameters )
{
    /* Calculo de stack
    temp_in_second   = N_MAX
    N_UART           = 1
    last_temp        = 1
    sum              = 1
    mean             = 1
    i                = 1
    xQueueRcv        = 4
    xQueueRcv        = 4
    xQueueSnd        = 4
    vTaskDelay       = 4
    vTaskDelete      = 4
    for loop         = 4
    -------------------------
    Total:          49
    */


    #define N_MAX 20

    uint8_t temp_in_second[N_MAX] = {0};
    uint8_t N_UART = N_MAX;
    uint8_t last_temp = 0;
    uint8_t sum = 0;
    uint8_t mean = 0;
    uint8_t i = 0;
    for(;;)
    {
        xQueueReceive(xTempQueue, &last_temp, portMAX_DELAY);
        xQueueReceive(xUARTQueue, &N_UART, 0);                      // Check UART N Value
        //Check N_UART <= N_MAX
        if (N_UART < N_MAX) 
            N_UART = N_MAX;

        //Push into queue
        for(int i=(N_UART-1); i>0; i--){temp_in_second[i] = temp_in_second[i-1];} //This could be replaced with a bitwise operation.
        temp_in_second[0] = last_temp;

        //Calculate mean
        for(int j = 0; j < N_UART; j++){ sum = temp_in_second[j]; }
        mean = sum / N_UART;

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
        OSRAMImageDraw(graph, 26, 0, 96, 2);
        OSRAMStringDraw("40", 12, 0);
        OSRAMStringDraw(" 0", 12, 1);
        OSRAMStringDraw("t", 90, 1);
    }
    vTaskDelete( NULL );
}



/*------------------------------------------------------------------------------*/
/*                              GRAPH FUNCTIONS                                 */
/*------------------------------------------------------------------------------*/

void plot(uint8_t * image, uint8_t value)
{
    int i;
    // dibujo la linea del eje Y mediante 255 = 11111111
    image[0] = 255;
    image[96] = 255; // eje y
    /*
    //ESTA PRIMERA PARTE CORRESPONDE A LOS VALORES de 1 a 16 SEGUNDA FILA DEL DiSPLAY
    if (value < 16)
    {
        
        //CON EL SIGUIENTE FOR MUEVO TODOS LOS VALORES de izquierda a la derecha para poner el valor nuevo
        for (i = 191; i > 97; i--)
        {
            image[i] = (image[i - 1]);
        }
        image[97] = ((0b10000000 >> (int)(value / 2)) |0b10000000); //primer pixel de la segunda fila

        for (i = 95; i > 1; i--)
        {
            image[i] = image[i - 1];
        }
        image[1] = 0b00000000;
    }
    //SEGUNDA CORRESPONDE A LOS VALORES de 1 a 16 SEGUNDA FILA DEL DiSPLAY
    else
    {
        //CON EL SIGUIENTE FOR MUEVO TODOS LOS VALORES de izquierda a la derecha para poner el valor nuevo
        for (i = 95; i > 1; i--)
        {
            image[i] = image[i - 1];
        }
        image[1] = (0b10000000 >> (int)((value - 16) / 2)) ;
        
        for (i = 191; i > 97; i--)
        {
            image[i] = (image[i - 1] );
        }
        image[97] = 0b10000000;
        
    }
    */
    return;
}
