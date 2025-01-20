#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "modules/button.h"
#include "modules/screen.h"

int buttonPressed = 0;
int count = 0;
int currentCount = 0;


void IRAM_ATTR interruptHandler(void *arg)
{
    buttonPressed = 1;
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
    gpio_isr_handler_add(BUTTON_GPIO, interruptHandler, NULL);
}

void incrementMenu()
{
    while (1)
    {
        if (buttonPressed >= 1)
        {
            buttonPressed = 0;

            if (count < NUM_SCREENS)
            {
                count++;
            }
            else
            {
                count = 0;
            }

            ESP_LOGI("[Button]", "Count %d", count);
        }

        switch (count)
        {
        case 3:
            if(currentCount == 3){
                break;
            }
            else{
                menu_soil_moisture(&soilMoisture);
                currentCount = 3;
                break;
            }
        case 2:
            if(currentCount == 2){
                break;
            }
            else{
                menu_soil_temperature(&soilTemperatur);
                currentCount = 2;
                break;
            }
        case 1:
            if(currentCount == 1){
                break;
            }
            else{
                menu_air_humidity(&airHumidity);    
                currentCount = 1;
                break;
            }
        default:
            if(currentCount == 0){
                break;
            }
            else{
                menu_air_temperature(&airTemperatur);
                currentCount = 0;
                break;
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}