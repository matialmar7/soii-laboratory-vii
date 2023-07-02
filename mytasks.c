#include "mytasks.h"
#include "rand.h"
#include "stats.h"

void xTaskTemperatureSensor( void *pvParameters )
{
    #ifdef STACK_CALCULATION
        UBaseType_t uxHighWaterMark;
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        char *buf = pvPortMalloc(10*sizeof(char));
    #endif

    uint8_t temperatura = 0;
    uint8_t temperatura_base = 15;

    for(;;)
    {
        temperatura = (uint8_t)(rand() % 15) + temperatura_base;

        //Enqueue
        xQueueSend(xTempQueue, &temperatura, portMAX_DELAY);

        #ifdef STACK_CALCULATION
            uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
            print("Temp: ");
            print(ltoc(uxHighWaterMark,buf,10));
            print("\n");
        #endif

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    #ifdef STACK_CALCULATION
        vPortFree(buf);
    #endif
    vTaskDelete( NULL );
}

void xTaskFilter( void *pvParameters )
{
    #define N_MAX 20
    #ifdef STACK_CALCULATION
        UBaseType_t uxHighWaterMark;
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        char *buf = pvPortMalloc(10*sizeof(char));
    #endif

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

        #ifdef STACK_CALCULATION
            uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
            print("Filter: ");
            print(ltoc(uxHighWaterMark,buf,10));
            print("\n");
        #endif

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    #ifdef STACK_CALCULATION
        vPortFree(buf);
    #endif
    #undef N_MAX
    vTaskDelete( NULL );
}

void xTaskDisplay( void *pvParameters )
{
    // We are implementing this in the OSRAM96x16 OLED Display
    /**
     * Se utilizara un display de 2 filas, de 16 x 96. Con cada fila de 8x96
    */

    #ifdef STACK_CALCULATION
        UBaseType_t uxHighWaterMark;
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        char *buf = pvPortMalloc(10*sizeof(char));
    #endif

    uint8_t graph[192] = {0};
    uint8_t temp = 0; 
    /*Clear the OLED Display*/
    OSRAMClear();
    for (;;)
    {
        xQueueReceive(xDisplayQueue, &temp, portMAX_DELAY);

        plot(graph, temp);
        OSRAMImageDraw(graph, 15, 0, 96, 2);
        OSRAMStringDraw(itoa(temp,10), 84, 2);
        OSRAMStringDraw("t", 10, 1);

        #ifdef STACK_CALCULATION
            uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
            print("Display: ");
            print(ltoc(uxHighWaterMark,buf,10));
            print("\n");
        #endif
    }

    #ifdef STACK_CALCULATION
        vPortFree(buf);
    #endif

    vTaskDelete( NULL );
}

void xTaskTop( void *pvParameters )
{
    #ifdef STACK_CALCULATION
        UBaseType_t uxHighWaterMark;
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        char *buf = pvPortMalloc(10*sizeof(char));
    #endif

    size_t size = 400;
    char *data = pvPortMalloc(size);  // Variable que almacena la data para enviar 

    for (;;)
    {
        print("\e[1;1H\e[2J");  // clear console
        print(header);
        print(divider);
        print("\n");
        getRunTimeStats(data);
        print(data);

        #ifdef STACK_CALCULATION
            uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
            print("Top: ");
            print(ltoc(uxHighWaterMark,buf,10));
            print("\n");
        #endif

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vPortFree(data);

    #ifdef STACK_CALCULATION
        vPortFree(buf);
    #endif

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
