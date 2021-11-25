#include "esp_log.h"
#include "logger.h"

void logger(const char* logStr) {
	ESP_LOGI("Test", "%s", logStr);
}
