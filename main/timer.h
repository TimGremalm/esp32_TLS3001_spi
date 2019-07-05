#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

void init_timer();
esp_err_t user_timer_start(int timer_period_us);

extern SemaphoreHandle_t xSemaphore_delay;