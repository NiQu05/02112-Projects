#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
//#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <string.h>
#include "esp_log.h"

// Driver libraries
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/adc.h"

//Temperature/humidity sensor library
#include "am2320.h"

//Stemma soil sensor library
#include "Adafruit_Stemma_soil_sensor.h"

//Display libraies
#include "ssd1306.h"
#include "font8x8_basic.h"

//I2C setup
#define I2C_MASTER_FREQ_HZ 75000 //Reduce it to 50000 if the temperature/umidity sensor fails
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
#define I2C_MASTER_SDA_GPIO 2 // SDA pin
#define I2C_MASTER_SCL_GPIO 3 //SCL pin
#define I2C_NUM 0 

// PWM library to control LED intensity and/or play tone on buzzer
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO_RED (6)   // Define the output GPIO for red
#define LEDC_OUTPUT_IO_GREEN (4) // Define the output GPIO for green
#define LEDC_OUTPUT_IO_BLUE (5)  // Define the output GPIO for blue
#define LEDC_CHANNEL_RED LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE LEDC_CHANNEL_2
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY (4096)                // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY (1000)           // Frequency in Hertz. Set frequency at 1 kHz

#define BUZZ_TIMER LEDC_TIMER_1
#define BUZZ_MODE LEDC_LOW_SPEED_MODE
#define BUZZ_OUTPUT_IO (9) // Define the output GPIO for red
#define BUZZ_CHANNEL LEDC_CHANNEL_4
#define BUZZ_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_DUTY (4096)                // Set duty to 50%. (2 ** 13) * 50% = 4096
#define BUZZ_FREQUENCY (1000)           // Frequency in Hertz. Set frequency at 1 kHz

#define RED_LED_GPIO 8
#define MIN_TEMP 15
#define TEMP 20

extern i2c_dev_t i2c_screen;
extern i2c_dev_t i2c_am2320;
extern SSD1306_t screenDevice; 

void config_setup();
void i2c_setup();
void alarm_setup();
void light_sensor_init();
void air_sensor_init();
void soil_sensor_init();
void display_init();

#endif