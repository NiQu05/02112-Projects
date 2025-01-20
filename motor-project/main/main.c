#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define MOTOR 1

void app_main(void)
{
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = (1ULL << MOTOR);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(MOTOR, 1);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_set_level(MOTOR, 0);

}
