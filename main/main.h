#pragma once

#include "TLS3001.h"
#include "freertos/queue.h"

#define MAX_PIXELS 361
#define USE_GAMMA_CORR 1

extern QueueHandle_t  TLS3001_input_queue;

#define BLINK_GPIO CONFIG_BLINK_GPIO
