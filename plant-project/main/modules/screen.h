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
#include "ssd1306.h"
#include "font8x8_basic.h"
#include <am2320.h>
#include "Adafruit_Stemma_soil_sensor.h"

#include "modules/bitmaps.h"

#define I2C_MASTER_FREQ_HZ 50000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 3
#define I2C_NUM 0

#define tag "EXAMPLE_ALL"

#define ICON_Y 16
#define ICON_X 0

void display_init();
void update_screen_data(uint8_t value, char format[], uint8_t icon[288]);
void menu_air_temperature(uint8_t temperatur);
void menu_air_humidity(uint8_t humidity);
void menu_soil_temperature(uint8_t soil_temperatur);
void menu_soil_moisture(uint8_t soil_moisture);
