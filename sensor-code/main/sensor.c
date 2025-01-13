#include "sensor.h"

//Public variables
i2c_dev_t device = {0}; //Initialize i2c device descriptor

void i2c_init(){
    //From Demo, i2c gpio setup
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_GPIO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = 0;
    i2c_param_config(I2C_NUM, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
}

void light_sensor_init(){
    //Initializes the analog to digital signal from the light sensor pin
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); //ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)
}

int get_lightvalue(){
    return adc1_get_raw(ADC1_CHANNEL_0);
}

void air_sensor_init(){
    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&device, I2C_NUM));
}

float get_temperatur(){
    float temperature;
    esp_err_t result = am2320_get_rht(&device, &temperature, NULL);

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
    esp_err_t result = am2320_get_rht(&device, NULL, &humidity);

    //If result is ok, then return humidity
    if (result == ESP_OK){
        return humidity;
    }    
    else{
        ESP_LOGE(TAG_AIR_HUMIDITY, "Error reading data: %d (%s)", result, esp_err_to_name(result));
        return 0;
    }
}

void soil_sensor_init(){
    //Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());
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