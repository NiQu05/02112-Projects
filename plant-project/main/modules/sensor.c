#include "modules/config.h"
#include "sensor.h"

int get_lightvalue(){
    return adc1_get_raw(ADC1_CHANNEL_0);
}

float get_temperatur(){
    float temperature;
    esp_err_t result = am2320_get_rht(&i2c_am2320, &temperature, NULL);

    //If result is ok, then return temperatur
    if (result == ESP_OK){
        return temperature;
    }    
    else{
        ESP_LOGE(TAG_AIR_TEMPERATUR, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

float get_humidity(){
    float humidity;
    esp_err_t result = am2320_get_rht(&i2c_am2320, NULL, &humidity);

    //If result is ok, then return humidity
    if (result == ESP_OK){
        return humidity;
    }    
    else{
        ESP_LOGE(TAG_AIR_HUMIDITY, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

float get_soil_temperatur(){
    float temperatur = 0;
    esp_err_t result = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperatur);

    if (result == ESP_OK){
        return temperatur;
    }
    else{
        ESP_LOGE(TAG_SOIL_TEMPERATUR, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

uint16_t get_soil_moisture(){
    uint16_t moisture = 0;
    esp_err_t result = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture);

    if (result == ESP_OK){
        return moisture;
    }
    else{
        ESP_LOGE(TAG_SOIL_MOISTURE, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}