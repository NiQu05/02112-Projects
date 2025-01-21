#include "modules/config.h"
#include "modules/sensor.h"

void RED_LED(float * soilTemp)
{
    while(1)
    {
        //ESP_LOGI("Soil temp", "%.1f", *soilTemp);

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
        gpio_set_level(MOTOR, 1);
        //ESP_LOGI("Soil moisture2", "%u", *soilMoisture);
        if (*soilMoisture < 700)
        {
            set_color(0, 8191, 8191); // Rød
            gpio_set_level(MOTOR, 0);
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            gpio_set_level(MOTOR, 1);
            vTaskDelay(10000 / portTICK_PERIOD_MS);

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
        //ESP_LOGI("Soil moisture", "%u", *soilMoisture);
        if (*soilMoisture < 700)
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