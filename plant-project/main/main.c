#include <stdio.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <string.h>
#include "esp_log.h"

#include "modules/alarm.h"
#include "modules/sensor.h"

float airTemperatur = 0;
float airHumidity = 0;
float soilTemperatur = 0;
uint16_t soilMoisture = 0;
int lightValue = 0;

// Task to update the sensor values
void VALUE_UPDATE()
{
    while(1)
    {
        airTemperatur = get_temperatur();
        airHumidity = get_humidity();
        soilTemperatur = get_soil_temperatur();
        soilMoisture = get_soil_moisture();
        lightValue = get_lightvalue();
        vTaskDelay((2000) / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    alarm_setup();

    xTaskCreate(VALUE_UPDATE, "VALUE_UPDATE", 4096, NULL, 1, NULL);
    xTaskCreate(RED_LED, "RED_LED", 4096, &soilTemperatur, 1, NULL);
    xTaskCreate(RGB_LED, "RGB_LED", 4096, &soilMoisture, 1, NULL);
    xTaskCreate(BUZZ, "BUZZ", 4096, &soilMoisture, 1, NULL);
}
