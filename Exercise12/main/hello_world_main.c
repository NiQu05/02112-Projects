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

void buttonInterruptHandler() {
    ESP_LOGI("ButtonInterrupt", "Button pressed!");
}

void initializeInterrupt() {
    //Example of initialization of interrupt from ChatGPT
    //Configure the button GPIO as an input with pull-up and enable interrupt
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO), // Select the button GPIO
        .mode = GPIO_MODE_INPUT,              // Set as input mode
        .pull_up_en = GPIO_PULLUP_ENABLE,     // Enable pull-up resistor
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down resistor
        .intr_type = GPIO_INTR_NEGEDGE        // Interrupt on falling edge
    };
    gpio_config(&io_conf);

    // Install the ISR service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // Attach the interrupt handler
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

}

void app_main(void)
{
    printf("Testebeste!\n");
}
