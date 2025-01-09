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

#define BUTTON_GPIO 4

int hasBeenPressed = 0;
int pressedCount = 0;

void IRAM_ATTR buttonInterruptHandler(void * arg)
{
    hasBeenPressed = 1;
    pressedCount++;
}

void initializeInterrupt()
{
    // From Demo project
    gpio_config_t io_conf;
    
    io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, buttonInterruptHandler, NULL);
}

void checkPressed(){
    while(1)
    {   
        if(hasBeenPressed >= 1){
            hasBeenPressed = 0;
            ESP_LOGI("ButtonInterrupt", "Button pressed %d amount of times.", pressedCount);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    initializeInterrupt();

    xTaskCreate(checkPressed, "CheckingPressed", 2048, NULL, 10, NULL);
}