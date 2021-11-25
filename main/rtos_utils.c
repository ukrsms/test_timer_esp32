#include "rtos_utils.h"

QueueHandle_t queueCreate(int itemSize, int itemsCount) {
	return xQueueCreate(itemsCount, itemSize);
}

TaskHandle_t createTask(TaskFunction function, const char* taskName, int prio, void* param) {
	TaskHandle_t taskHandle;
	BaseType_t result=xTaskCreatePinnedToCore(function, taskName, 4096, param, prio, &taskHandle, PRO_CPU_NUM);
	ESP_ERROR_CHECK(result!=pdPASS);
	return taskHandle;
}
long long getTimeMs(void) {
	return esp_timer_get_time()/1000;
}
