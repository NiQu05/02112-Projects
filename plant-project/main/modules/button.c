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

//#define DEBUG

int buttonPressed = 0;  // To determine if the button is pressed or not
int count = 0;          
int currentCount = 0;

// Button interrupt handler
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

// For clicking through the different menus (task function)
void Increment_menu(void * arg)
{
    while (1)
    {
        // If the button is pressed count up
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

            #ifdef DEBUG
                ESP_LOGI("[Button]", "Count %d", count);
            #endif
        }

        // Determines the menu depending on count
        switch (count)
        {
        case 4:
            if(currentCount == 4){
                break;
            }
            else{
                menu_ligt_level();
                currentCount = 4;
                break;
            }
        case 3:
            if(currentCount == 3){
                break;
            }
            else{
                menu_soil_moisture();
                currentCount = 3;
                break;
            }
        case 2:
            if(currentCount == 2){
                break;
            }
            else{
                menu_soil_temperature();
                currentCount = 2;
                break;
            }
        case 1:
            if(currentCount == 1){
                break;
            }
            else{
                menu_air_humidity();    
                currentCount = 1;
                break;
            }
        default:
            if(currentCount == 0){
                break;
            }
            else{
                menu_air_temperature();
                currentCount = 0;
                break;
            }
        }

        // Delay 
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}