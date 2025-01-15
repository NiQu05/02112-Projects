#include <stdio.h>
#include "sdkconfig.h"
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

#include "modules/sensor.h"

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

void alarm_setup()
{
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = (1ULL << RED_LED_GPIO);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // Prepare and then apply the LEDC PWM channel configuration

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY, // Set output frequency at 1 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel_red = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_RED,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO_RED,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_red));

    ledc_channel_config_t ledc_channel_green = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_GREEN,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO_GREEN,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_green));

    ledc_channel_config_t ledc_channel_blue = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL_BLUE,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO_BLUE,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_blue));

    // Prepare and then apply the LEDC PWM timer configuration (we use it for the buzzer)
    ledc_timer_config_t ledc_timer_buzz = {
        .speed_mode = BUZZ_MODE,
        .duty_resolution = BUZZ_DUTY_RES,
        .timer_num = BUZZ_TIMER,
        .freq_hz = BUZZ_FREQUENCY, // Set output frequency at 1 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_buzz));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel_buzz = {
        .speed_mode = BUZZ_MODE,
        .channel = BUZZ_CHANNEL,
        .timer_sel = BUZZ_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = BUZZ_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_buzz));

    // Sensor setup
    i2c_init();
    soil_sensor_init();
}

void RED_LED(float * soilTemp)
{
    while(1)
    {
        ESP_LOGI("Soil temp", "%.1f", *soilTemp);

        if (*soilTemp < MIN_TEMP)
        {
            for(int i = 0; i < 3; i++){
                gpio_set_level(RED_LED_GPIO, 1);
                vTaskDelay((200) / portTICK_PERIOD_MS);
                gpio_set_level(RED_LED_GPIO, 0);
                vTaskDelay((200) / portTICK_PERIOD_MS);
            }
            
        }
        else if (*soilTemp < TEMP)
        {
            gpio_set_level(RED_LED_GPIO, 1);
            vTaskDelay((500) / portTICK_PERIOD_MS);
            gpio_set_level(RED_LED_GPIO, 0);
            vTaskDelay((500) / portTICK_PERIOD_MS);
        }
        else 
        {
            vTaskDelay((1000) / portTICK_PERIOD_MS);
        }
    }
    
}

void set_color(unsigned int red, unsigned int green, unsigned int blue)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, red);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, green);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, blue);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);
}

void RGB_LED(uint16_t * soilMoisture)
{
    while(1)
    {
        ESP_LOGI("Soil moisture2", "%u", *soilMoisture);
        if (*soilMoisture < 600)
        {
            set_color(0, 8191, 8191); // Rød
        }
        else if (*soilMoisture < 800)
        {
            set_color(0, 0, 8191); // Gul
        }
        else if (*soilMoisture < 1000)
        {
            set_color(8191, 0, 8191); // Grøn
        }
        else if (*soilMoisture < 1200)
        {
            set_color(8191, 0, 0); // Cyan
        }
        else
        {
            set_color(8191, 8191, 0); // Blå
        }

        vTaskDelay((1000) / portTICK_PERIOD_MS);
    }
    
}

void BUZZ(uint16_t * soilMoisture)
{
    int melody[] = 
    {
        659, 659,   0, 659,   0, 523, 659, 784,   0, 392,
        0, 523, 392, 330,   0, 440, 494, 466, 440, 392,
        659, 784, 880,   0, 698, 784, 659, 523, 587, 494,
    };

    int duration[] = 
    {
        200, 200, 150, 200, 150, 200, 200, 300, 150, 300,
        150, 300, 200, 300, 150, 200, 200, 200, 200, 400,
        200, 200, 300, 150, 200, 200, 200, 200, 200, 400,
    };

    while(1)
    {
        ESP_LOGI("Soil moisture", "%u", *soilMoisture);
        if (*soilMoisture < 600)
        {
            ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 4096));
            ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
            for (int i = 0; i < 30; i++)
            {
                if (melody[i] == 0)
                {
                    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 0));
                    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
                    vTaskDelay((duration[i]) / portTICK_PERIOD_MS);
                    ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 4096));
                    ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
                }
                else
                {
                    ledc_set_freq(BUZZ_MODE, BUZZ_TIMER, melody[i]);
                    vTaskDelay((duration[i]) / portTICK_PERIOD_MS);
                }
            }
        } else 
        {
            vTaskDelay((1000) / portTICK_PERIOD_MS);
        }
        ESP_ERROR_CHECK(ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, 0));
        ESP_ERROR_CHECK(ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL));
    } 
}