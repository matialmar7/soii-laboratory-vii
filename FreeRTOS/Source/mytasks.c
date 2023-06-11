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