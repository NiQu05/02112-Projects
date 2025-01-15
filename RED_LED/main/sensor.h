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

//Temperature/humidity sensor library
#include "am2320.h"

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

#define I2C_MASTER_FREQ_HZ 50000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2 // SDA pin
#define I2C_MASTER_SCL_GPIO 3 //SCL pin
#define I2C_NUM 0 

//Tags
#define TAG_LIGHT_SENSOR "[Light Sensor]"
#define TAG_AIR_TEMPERATUR "[Air Temperatur]"
#define TAG_AIR_HUMIDITY "[Air Humidity]"
#define TAG_SOIL_TEMPERATUR "[Soil Temperatur]"
#define TAG_SOIL_MOISTURE "[Soil Moisture]"

//Prototypes
void i2c_init();
void light_sensor_init();
int get_lightvalue();
void air_sensor_init();
float get_temperatur();
float get_humidity();
void soil_sensor_init();
float get_soil_temperatur();
uint16_t get_soil_moisture();