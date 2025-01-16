#include "modules/config.h"
#include "modules/alarm.h"
#include "modules/sensor.h"
#include "modules/screen.h"

float airTemperatur = 0;
float airHumidity = 0;
float soilTemperatur = 0;
uint16_t soilMoisture = 0;
int lightValue = 0;

// Task to update the sensor values
void VALUE_UPDATE()
{
    while (1)
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
    config_setup();

    xTaskCreate(VALUE_UPDATE, "VALUE_UPDATE", 4096, NULL, 1, NULL);
    xTaskCreate(RED_LED, "RED_LED", 4096, &soilTemperatur, 10, NULL);
    xTaskCreate(RGB_LED, "RGB_LED", 4096, &soilMoisture, 10, NULL);
    xTaskCreate(BUZZ, "BUZZ", 4096, &soilMoisture, 10, NULL);
    //xTaskCreate(menu_air_temperature, "menu_air_temperature", 4096, &airTemperatur, 1, NULL);
}
