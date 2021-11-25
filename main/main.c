#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "rtos_utils.h"

typedef void (*LoggerCb)(const char* logStr);

typedef struct {
	LoggerCb loggerCb;
	QueueHandle_t queueHandle;
} LoggingTaskParams;

void timerTask(void* param) {
	QueueHandle_t queueHandle=param;
	int counter=0;
	int timerPeriodMs=5000;
	int time=xTaskGetTickCount();
	while(1) {
		bool itemIsSent=xQueueSendToBack(queueHandle, &counter, 0);
		if(!itemIsSent)
			logger("Queue sent error!");
		vTaskDelayUntil( &time, timerPeriodMs/portTICK_PERIOD_MS);
		counter++;
	}
}
TaskHandle_t startTimerTask(QueueHandle_t queueHandle) {
	return createTask(timerTask, "timer", 5, queueHandle);
}
void loggingTask(void* params) {
	LoggingTaskParams *loggingTaskParams=params;
	LoggerCb loggerCb=loggingTaskParams->loggerCb;
	QueueHandle_t queueHandle=loggingTaskParams->queueHandle;
	int prevCounterTime=0;
	int counter;
	while(1) {
		xQueueReceive((QueueHandle_t)queueHandle, &counter, portMAX_DELAY );
		long long time=getTimeMs();
		int deltaTime=time-prevCounterTime;
		prevCounterTime=time;
		char logBuffer[60];
		snprintf(logBuffer, sizeof(logBuffer), "Counter received: %d. Delta time: %d", counter, deltaTime);
		loggerCb(logBuffer);
	}
}
TaskHandle_t startLoggingTask(LoggingTaskParams* taskParams) {
	void* taskHandle=NULL;
	return createTask(loggingTask, "logger", 6, taskParams);
}
void app_main() {
	LoggingTaskParams params;
	params.queueHandle=queueCreate(sizeof(int), 10);
	if(!params.queueHandle)
		logger("Queue create error!");
	params.loggerCb=logger;
	startTimerTask(params.queueHandle);
	startLoggingTask(&params);
	vTaskSuspend(NULL);
}
