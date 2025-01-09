#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define BLINK
#define HRT

#define GPIO_PIN 8
#define DELAY 5

#define tag "Exercise 11"

void app_main(void)
{
    // From Demo project
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = (1ULL << GPIO_PIN);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

#ifdef BLINK
    ESP_LOGI(tag, "One-shot timer of %d seconds started.", DELAY);
    gpio_set_level(GPIO_PIN, 1);
    vTaskDelay((DELAY * 1000) / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_PIN, 0);
    ESP_LOGI(tag, "One-shot timer of %d seconds finished.", DELAY);
#endif

#ifdef HRT
    int time_before, time_after;

    // Get time at start of task
    time_before = esp_timer_get_time();

    // Task is blink LED 5 times
    for (int i = 0; i < 10; i++)
    {
        gpio_set_level(GPIO_PIN, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_PIN, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // Get time after task
    time_after = esp_timer_get_time();

    // Print elapsed time
    ESP_LOGI(tag, "Elapsed time: %d microseconds", (time_after - time_before));
#endif
}