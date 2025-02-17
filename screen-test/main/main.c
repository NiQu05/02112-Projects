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

SSD1306_t dev;

void display_init(){
    //Initialize the display (shared i2c) only once after boot.
    i2c_master_shared_i2c_init(&dev);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
}

void update_screen_data(uint8_t value, char format[], uint8_t icon[288]){
    static char buffer[24];
    snprintf(buffer, sizeof(buffer), format, value);
    ssd1306_display_text_x3(&dev, 4, buffer, 24, false);
    ssd1306_bitmaps(&dev, ICON_X, ICON_Y, icon, 48, 48, false);
}

void menu_air_temperature(uint8_t temperatur){
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "  Temperature   ", 16, false);
    update_screen_data(temperatur, "  %dC", thermostat_map);
}

void menu_air_humidity(uint8_t humidity){
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "    Humidity    ", 16, false);
    update_screen_data(humidity, "  %d%%", humidity_map);
}

void menu_soil_temperature(uint8_t soil_temperatur){
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "Soil Temperature", 16, false);
    update_screen_data(soil_temperatur, "  %dC", temp_eco_map);
}

void menu_soil_moisture(uint8_t soil_moisture){
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "  Soil Moisture ", 16, false);
    update_screen_data(soil_moisture, "  %d%%", water_soil_map);
}

void app_main(void)
{
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

    vTaskDelay(100 / portTICK_PERIOD_MS);

    display_init();

    menu_air_temperature(28);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    menu_air_humidity(47);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    update_screen_data(48, "  %d%%", humidity_map);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    update_screen_data(70, "  %d%%", humidity_map);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    menu_soil_temperature(30);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    menu_soil_moisture(10);
}