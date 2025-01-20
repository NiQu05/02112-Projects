#include "modules/config.h"
#include "modules/alarm.h"
#include "modules/sensor.h"
#include "modules/screen.h"
#include "modules/button.h"

// Task to update the sensor values
void VALUE_UPDATE()
{
    while (1)
    {
        airTemperatur = get_temperatur();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        airHumidity = get_humidity();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        soilTemperatur = get_soil_temperatur();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        soilMoisture = get_soil_moisture();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        lightValue = get_lightvalue();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    config_setup();
    initializeInterrupt();
    
    xTaskCreate(VALUE_UPDATE, "VALUE_UPDATE", 4096, NULL, 1, NULL);
    xTaskCreate(RED_LED, "RED_LED", 4096, &soilTemperatur, 10, NULL);
    xTaskCreate(RGB_LED, "RGB_LED", 4096, &soilMoisture, 10, NULL);
    xTaskCreate(BUZZ, "BUZZ", 4096, &soilMoisture, 10, NULL);
    xTaskCreate(incrementMenu, "incrementMenu", 4096, NULL, 10, NULL);

}
