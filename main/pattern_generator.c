#include "main.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "TLS3001.h"

static const char * TAG = "pattern_gen";

static void pattern_gen_task(void *arg);
static void pattern_equal_color(uint16_t *input_color_array, uint16_t num_pixels, uint16_t *rgb);

uint16_t test_color[3] = {3000, 0, 1000};
uint16_t pattern_color_array[TEST_PIXELS];

pixel_message_s pixel_message_pattern;

void pattern_gen_task(void *arg)
{
    //uint16_t *p_message;
    //p_message = &input_color_array;
   while(1)
    {

        if( xSemaphoreTake(pixel_message_pattern.pixel_data_semaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
            ESP_LOGI(TAG, "Generating pattern data");
            pattern_equal_color(&pattern_color_array,TEST_PIXELS, &test_color);

            xSemaphoreGive(pixel_message_pattern.pixel_data_semaphore);
            
            pixel_message_pattern.message_ready = true;
            pixel_message_pattern.message = (void*)&pattern_color_array;
            pixel_message_pattern.len = TEST_PIXELS;
            if(xQueueSend(TLS3001_input_queue, &pixel_message_pattern,(TickType_t )10))
            {
                ESP_LOGI(TAG, "successfully posted pattern data on queue");
            }
            else
            {
                ESP_LOGW(TAG, "Queue full. Did not post any data!");
            }
            
            /*
            if(xTaskNotify(TLS3001_send_task, &pattern_color_array,eSetValueWithoutOverwrite))
            {
                ESP_LOGI(TAG, "pattern data was sent to TLS3001 task");
            }
            else
            {
                //Notification already pending..
            }*/
            

        }
        else
        {
            /* code */
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }        
   
}

esp_err_t pattern_init(uint16_t num_pixels)
{

    /*
    input_color_array = (uint16_t*) heap_caps_malloc((sizeof(total_pixels)*total_pixels*3), MALLOC_CAP_8BIT);
	if(input_color_array == NULL)
	{
		return ESP_FAIL;
	}
    */
    vSemaphoreCreateBinary(pixel_message_pattern.pixel_data_semaphore);
	
    xTaskCreate(&pattern_gen_task, "pattern_gen_task", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "pattern_gen initiated!");


    return ESP_OK;
}

static void pattern_equal_color(uint16_t *input_color_array, uint16_t num_pixels, uint16_t *rgb)
{
    uint16_t red = *rgb;
    uint16_t green = *(rgb+1);
    uint16_t blue = *(rgb+2);

    for (size_t i = 0; i < num_pixels; i++)
	{
		*(input_color_array+(i*3)+0) = red;
		*(input_color_array+(i*3)+1) = green;
		*(input_color_array+(i*3)+2) = blue;
	}

}


void pattern_shift_pixels_right(void *spi_tx_data_start, uint16_t pixel_index_start, uint16_t pixel_index_end, uint16_t num_pixels)
{
    //Function requires you to have pixel data in *spi_tx_data_start. I.e by first setting pattern_equal_color() etc.

    //uint16_t *pixel_pointer_start = &input_color_array + (pixel_index_start*3);
    
}