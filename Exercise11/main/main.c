#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BLINK
#define HRT

#define GPIO_PIN 8
#define DELAY 5

#define tag "Exercise 11"

/* static TimerHandle_t one_shot_timer = NULL;

void myTimerCallback(TimerHandle_txTimer)
{
    gpio_set_level(GPIO_PIN, 0);
    ESP_LOGI(tag, "One-shot timer of %d seconds finished.", DELAY);
} */

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

    /* one_shot_timer = xTimerCreate("One-shot timer",                    // Name of timer
                                  (DELAY * 1000) / portTICK_PERIOD_MS, // Period of timer (in ticks)
                                  pdFALSE,                             // Auto-reload
                                  (void *)0,                           // Timer ID
                                  myTimerCallback);                    // Callback function

    xTimerStart(one_shot_timer, portMAX_DELAY); */

#elif HRT

#endif
}
