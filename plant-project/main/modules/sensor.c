#include "modules/config.h"
#include "sensor.h"

//#define DEBUG

// Returns the raw light value (Between 0 and 4000)
int get_lightvalue(){
    int light_value = adc1_get_raw(ADC1_CHANNEL_0);

    #ifdef DEBUG
        ESP_LOGI(TAG_LIGHT_SENSOR, "Light Value: %d", light_value);
    #endif

    return light_value;
}

// Returns the air temperature in celsius
float get_temperatur(){
    air_sensor_init();
    float temperature;
    esp_err_t result = am2320_get_rht(&i2c_am2320, &temperature, NULL);

    //If result is ok, then return temperatur
    if (result == ESP_OK){
        #ifdef DEBUG
            ESP_LOGI(TAG_AIR_TEMPERATUR, "Air Temperature: %.1fC", temperature);
        #endif
        return temperature;
    }    
    else{
        ESP_LOGE(TAG_AIR_TEMPERATUR, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

// Returns the air humidity in %
float get_humidity(){
    air_sensor_init();
    float humidity;
    esp_err_t result = am2320_get_rht(&i2c_am2320, NULL, &humidity);

    //If result is ok, then return humidity
    if (result == ESP_OK){
        #ifdef DEBUG
            ESP_LOGI(TAG_AIR_HUMIDITY, "Air Humidity: %.1fC", humidity);
        #endif
        return humidity;
    }    
    else{
        ESP_LOGE(TAG_AIR_HUMIDITY, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

// Returns the soil temperature in celsius
float get_soil_temperatur(){
    soil_sensor_init();

    float temperatur = 0;
    esp_err_t result = adafruit_stemma_soil_sensor_read_temperature(I2C_NUM, &temperatur);

    if (result == ESP_OK){
        #ifdef DEBUG
            ESP_LOGI(TAG_SOIL_TEMPERATUR, "Soil Temperature: %.1fC", temperatur);
        #endif
        return temperatur;
    }
    else{
        ESP_LOGE(TAG_SOIL_TEMPERATUR, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

// Returns the raw soil moisture value (Between 200 and 2000)
uint16_t get_soil_moisture(){
    soil_sensor_init();
    uint16_t moisture = 0;
    esp_err_t result = adafruit_stemma_soil_sensor_read_moisture(I2C_NUM, &moisture);

    if (result == ESP_OK){
        #ifdef DEBUG
            ESP_LOGI(TAG_SOIL_MOISTURE, "Soil Moisture: %u", moisture);
        #endif
        return moisture;
    }
    else{
        ESP_LOGE(TAG_SOIL_MOISTURE, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}