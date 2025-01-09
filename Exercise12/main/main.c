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

#define BUTTON_GPIO 18
#define INTERRUPT_ID

void buttonInterruptHandler()
{
    
    ESP_LOGI("ButtonInterrupt", "Button pressed!");
}

void initializeInterrupt()
{
    // From Demo project
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    gpio_isr_handler_add(BUTTON_GPIO, buttonInterruptHandler, NULL);
    gpio_intr_enable(BUTTON_GPIO);
}

void app_main(void)
{
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, buttonInterruptHandler, NULL);

    gpio_intr_enable(BUTTON_GPIO);

    while(1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
