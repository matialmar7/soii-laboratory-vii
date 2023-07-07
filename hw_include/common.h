#ifndef __COMMON_H__
#define __COMMON_H__

#include "DriverLib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define QUEUE_SIZE 10

QueueHandle_t xTempQueue;
QueueHandle_t xUARTQueue;
QueueHandle_t xDisplayQueue;

#endif // __COMMON_H__