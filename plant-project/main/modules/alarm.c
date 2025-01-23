#include "modules/config.h"
#include "modules/sensor.h"

// Changes blinking of the red LED depending on soil temperature (task function)
void Red_LED(void * arg)
{
    while(1)
    {
        if (soilTemperature < CRITICAL_TEMP)  // If soil temperature is critical low, red LED will blink quickly
        {
            for(int i = 0; i < 3; i++){
                gpio_set_level(RED_LED_GPIO, 1);
                vTaskDelay((200) / portTICK_PERIOD_MS);
                gpio_set_level(RED_LED_GPIO, 0);
                vTaskDelay((200) / portTICK_PERIOD_MS);
            }
            
        }
        else if (soilTemperature < MIN_TEMP)  // If soil temperature is low, red LED will blink slow
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

// Sets the color of the RGB LED
void set_color(unsigned int red, unsigned int green, unsigned int blue)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, red);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, green);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, blue);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);
}

// Changes the RGB LED colors depending on the input soil moisture value (task function)
void RGB_LED(void * arg)
{
    while(1)
    {
        gpio_set_level(MOTOR, 1);   // Turn off motor/pump

        if (soilMoisture < 700)
        {
            set_color(0, 8191, 8191); // Red
            gpio_set_level(MOTOR, 0);               // Turn on motor
            vTaskDelay(5000 / portTICK_PERIOD_MS);  // Wait 5 seconds
            gpio_set_level(MOTOR, 1);               // Turn off motor
            vTaskDelay(10000 / portTICK_PERIOD_MS); // Wait 10 seconds
        }
        else if (soilMoisture < 800)
        {
            set_color(0, 0, 8191); // Yellow
        }
        else if (soilMoisture < 1000)
        {
            set_color(8191, 0, 8191); // Green
        }
        else if (soilMoisture < 1200)
        {
            set_color(8191, 0, 0); // Cyan
        }
        else
        {
            set_color(8191, 8191, 0); // Blue
        }

        // Delay
        vTaskDelay((1000) / portTICK_PERIOD_MS);
    }
}

// Handles buzzer behavior (task function)
void Buzz(void * arg)
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
        if (soilMoisture < 700)
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