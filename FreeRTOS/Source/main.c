#include "common.h"
#include "mytasks.h"
#include "config.h"

int main(void)
{
    init();

    //Start RTOS scheduler
    vTaskStartScheduler(); //https://www.freertos.org/a00132.html 
    return 0;
}
