# About

This is the implementation of the [Free RTOS](./README.old.md) assignment from the Operative Systems II course of the Faculty of Exact, Physical and Natural Sciences from the National University of Cordoba.

# How to use it

## Requirements
    [*] Ubuntu 20.04 or docker for compiling
    [*] Build-essentials
    [*] Qemu

## Compiling the binaries
### Build
Use `docker build -t ubuntu -f Dockerfile .` to build the docker image

Run the docker using the `start_docker.sh <project_path>` in the docker folder and use ´make all´ to compile the binaries without stack calculation
use  `make all_stack` to compile the necessary code to compute the stack. 
## Excecution

### On windows
If you dont have Linux installed you can install QEMU in windows and use the compiled `TP7.axf` in the `gcc` folder and run it using the Powershell:

```powershell
& 'C:\Program Files\qemu\qemu-system-arm.exe' -serial stdio -machine lm3s811evb -kernel 'gcc\TP7.axf'
```
### On linux
```bash
qemu-system-arm -serial stdio -machine lm3s811evb -kernel gcc/TP7.axf
```

# Development

## Tasks
---
Four tasks were created for each specific requirement.

```c
void xTaskTemperatureSensor( void *pvParameters );
void xTaskFilter( void *pvParameters );
void xTaskDisplay( void *pvParameters );
void xTaskTop( void *pvParameters );
```

### xTaskTemperatureSensor
---
This task is responsible to generate a simulated temperature signal every 100ms using the following stochastic function:

`temperatura = (uint8_t)(rand() % 15) + temperatura_base;` 

with

 `temperatura_base = 15`

### xTaskFilter
---
This function is responsible to receive the temperature values and the filter values. 
After receiving those values it enqueues the temperature in a queue and computes the last N values mean, being N the filter value.

### xTaskDisplay
---
This function is responsible to receive the filtered values and generating an output PucImage (16x96 array of values).
After that it will use the OSRAM functions to display the graph with the text on the OLED Display.

### xTaskTop
---
This is a custom implementation of **GetRunTimeStats** for the LMS3811.
Several  `stl` functions were adapted to a lightweight version.

For using this function is necessary:
* Configuring a Timer and their ISR (I used the demo example)
* `define` the following `FreeRTOSConfig.h` macros    
     - `configGENERATE_RUN_TIME_STATS 1`
     - `configUSE_TRACE_FACILITY 1`      
     - `portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (ulHighFrequencyTimerTicks = 0UL)`
     - `portGET_RUN_TIME_COUNTER_VALUE() ulHighFrequencyTimerTicks`

For more information please refer to the FreeRTOS documentation.

## Stack calculation
---
uxTaskGetStackHighWaterMark is a function in FreeRTOS that returns the minimum amount of remaining stack space that was available to the task since the task started executing. This is what is referred to as the stack ‘high water mark’. The value returned is the high water mark in words (for example, on a 32-bit machine, a return value of 1 would indicate that 4 bytes of stack were unused). If the return value is zero then the task has likely overflowed its stack.

This is an example of how to use this function to calculate the stack usage of a task.

```c
UBaseType_t uxHighWaterMark;
/* Inspect our own high water mark on entering the task. */
uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
for( ;; )
{
    /* Run your task code here. */

    /* Calling uxTaskGetStackHighWaterMark() again will return the
    minimum amount of free stack space that has been available
    since the last time uxTaskGetStackHighWaterMark() was called. */
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
}
```

For example in the Temperature Sensor task we used the example to compute the stack. First an arbitraty stack ammount was set and then the returned value from uxTaskGetStackHighWaterMark was substracted. Take into account that the buffer is allocated in the heap but you have return addresses that will enlarge the stack.


```c
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
            print("Temp:");
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
```


# References

[FreeRTOS](https://www.freertos.org/)

[QEMU](www.qemu.org)

[LMS3811 Luminary](https://www.keil.com/dd/docs/datashts/luminary/lm3s811.pdf)