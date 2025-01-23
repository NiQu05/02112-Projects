#include "modules/config.h"
#include "modules/alarm.h"
#include "modules/sensor.h"
#include "modules/screen.h"
#include "modules/button.h"
#include "modules/network/wifi.h"

// Task to update the sensor values
void Value_update(void * arg)
{
    while (1)
    {
        airTemperature = get_temperatur();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        airHumidity = get_humidity();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        soilTemperature = get_soil_temperatur();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        soilMoisture = get_soil_moisture();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        lightValue = get_lightvalue();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    config_setup();
    initializeInterrupt();

    xTaskCreate(Value_update, "Value_update", 4096, NULL, 10, NULL);
    xTaskCreate(Red_LED, "Red_LED", 4096, NULL, 10, NULL);
    xTaskCreate(RGB_LED, "RGB_LED", 4096, NULL, 10, NULL);
    xTaskCreate(Buzz, "Buzz", 4096, NULL, 10, NULL);
    xTaskCreate(Increment_menu, "Increment_menu", 4096, NULL, 1, NULL);
    xTaskCreate(Http_rest_with_url, "Measurement", 4096, NULL, 10, NULL);
}
