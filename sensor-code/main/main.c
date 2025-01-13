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

#include "sensor.h"

void app_main(void)
{
    int light_sens_val = 0;
    float air_temperatur = 0;
    float air_humidity = 0;
    uint16_t soil_moisture = 0;
    float soil_temperatur = 0;

    //Set up sensors
    i2c_init();
    light_sensor_init();
    air_sensor_init();
    soil_sensor_init();

    //Testing the sensors (main loop)
    for (int i = 0; i < 20; i++)
    {
        light_sens_val = get_lightvalue();
        ESP_LOGI(TAG_LIGHT_SENSOR, "Light sensor ADC value: %d", light_sens_val);
        vTaskDelay(pdMS_TO_TICKS(500));  //Delay for 1 second

        air_temperatur = get_temperatur();
        ESP_LOGI(TAG_AIR_TEMPERATUR, "Temperature: %.1f°C", air_temperatur);
        vTaskDelay(pdMS_TO_TICKS(500));  //Delay for 1 second

        air_humidity = get_humidity();
        ESP_LOGI(TAG_AIR_HUMIDITY, "Humidity: %.1f%%", air_humidity);
        vTaskDelay(pdMS_TO_TICKS(500));  //Delay for 1 second

        soil_temperatur = get_soil_temperatur();
        ESP_LOGI(TAG_SOIL_TEMPERATUR, "Adafruit Stemma sensor value: %f", soil_temperatur);
        vTaskDelay(pdMS_TO_TICKS(500));  //Delay for 1 second

        soil_moisture = get_soil_moisture();
        ESP_LOGI(TAG_SOIL_MOISTURE, "Adafruit Stemma sensor value: %u", soil_moisture);
        vTaskDelay(pdMS_TO_TICKS(500));  //Delay for 1 second

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
