#ifndef __COMMON_H__
#define __COMMON_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define QUEUE_SIZE 10

QueueHandle_t xTempQueue;

#endif // __COMMON_H__