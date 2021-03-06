#pragma once

#include "esp_err.h"
#include "stdbool.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#define CH1_PIN_NUM_MOSI 23
#define CH1_PIN_NUM_CLK  18

#define PIXELS_CONNECTED 361    //Total length of connected LED array. Even if not all pixels will be used, be sure to still specify the entire lenght here.
#define USE_GAMMA_CORR 1        //Uncomment if gamma correction is not needed.

/* ----- Manchester encoding stuff ------ */
#define MANCHESTER_ONE 0x02		//0b10
#define MANCHESTER_ZERO 0x01	//0b01
#define MANCHESTER_VALUE(x)  ((x) ? MANCHESTER_ONE : MANCHESTER_ZERO)

#define RESET_CMD 0x7FFF4
#define RESET_CMD_LEN_MANCH (19)
#define RESET_CMD_LEN_SPI (RESET_CMD_LEN_MANCH * 2)

#define SYNCH_CMD 0x3FFF8800
#define SYNCH_CMD_LEN_MANCH (30)
#define SYNCH_CMD_LEN_SPI (SYNCH_CMD_LEN_MANCH * 2)

#define START_CMD 0x7FFF2
#define START_CMD_LEN_MANCH (19)
#define START_CMD_LEN_SPI (START_CMD_LEN_MANCH * 2)

#define COLOR_DATA_LEN_MANCH (13)
#define COLOR_DATA_LEN_SPI (COLOR_DATA_LEN_MANCH * 2)

#define PIXEL_DATA_LEN_MANCH (39)
#define PIXEL_DATA_LEN_SPI (PIXEL_DATA_LEN_MANCH * 2)

#define RGB_PACKET_LEN_SPI (39*2)

/* ------------------------------ */

#define SYNCH_DELAY_PER_PIXEL (28.34)

typedef struct
{
    void *spi_tx_data_start;
    uint16_t num_pixels;
    spi_host_device_t spi_channel;
    spi_device_handle_t spi_handle;
    uint32_t spi_freq;
    int spi_mosi_pin;
    int spi_clk_pin;
}TLS3001_handle_s;

typedef struct 
{
uint16_t *color_data_p;                     //Pointer to uint16_t array that contains all the pixel data. I.e: {r,g,b,r,g,b,...,r,g,b}
uint16_t pixel_len;                         //The number of pixels that the array contains.
SemaphoreHandle_t data_semaphore_guard;     //Semaphore for making sure that the data is thread-safe
}pixel_message_s;

//extern QueueHandle_t  TLS3001_input_queue;
esp_err_t TLS3001_ch1_init(uint16_t num_pixels);
esp_err_t TLS3001_send_to_queue(pixel_message_s *pixel_message_packet_p, uint16_t *color_array, uint16_t pixel_len);

void TLS3001_show(uint16_t *color_data, uint16_t num_pixels);

