#ifndef MAIN_INCLUDE_RTOS_UTILS_H_
#define MAIN_INCLUDE_RTOS_UTILS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef void (*TaskFunction)(void* param);

TaskHandle_t createTask(TaskFunction function, const char* taskName, int prio, void* param);
QueueHandle_t queueCreate(int itemSize, int itemsCount);
long long getTimeMs(void);

#endif
