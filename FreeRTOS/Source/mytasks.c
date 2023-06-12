#include "mytasks.h"
#include "stdlib.h"

void xTaskTemperatureSensor( void *pvParameters )
{
    uint8_t temp = 0;

    for(;;)
    {
        temp = (uint8_t)(rand() % 25) + 1;

        //Enqueue
        xQueueSend(xTempQueue, &temp, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete( NULL );
}

void xTaskFilter( void *pvParameters )
{
    #define N_MAX 100

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
    }

    #undef N_MAX
}