#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define BUTTON1_GPIO 18
#define BUTTON2_GPIO 19

int button1HasBeenPressed = 0;
int button2HasBeenPressed = 0;
int count = 0;

void IRAM_ATTR button1InterruptHandler(void * arg)
{
    button1HasBeenPressed = 1;
}

void IRAM_ATTR button2InterruptHandler(void * arg)
{
    button2HasBeenPressed = 1;
}

void initializeInterrupt()
{
    // From Demo project
    gpio_config_t io_conf;
    
    io_conf.pin_bit_mask = ((1ULL << BUTTON1_GPIO) | (1ULL << BUTTON2_GPIO));
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON1_GPIO, button1InterruptHandler, NULL);
    gpio_isr_handler_add(BUTTON2_GPIO, button2InterruptHandler, NULL);
}

void incrementMenu(){
    while(1)
    {   
        //if(button1HasBeenPressed >= 1){
        //    button1HasBeenPressed = 0;
        //    count++;
        //    ESP_LOGI("[Button]", "Count %d", count);
        //}
        
        //if(button2HasBeenPressed >= 1){
        //    button2HasBeenPressed = 0;
        //    count--;
        //    ESP_LOGI("[Button]", "Count %d", count);
        //}

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}