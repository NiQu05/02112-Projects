#include "modules/config.h"

//Global devices
i2c_dev_t i2c_screen = {0};
i2c_dev_t i2c_am2320 = {0};
SSD1306_t screenDevice;

//Global parameters
float airTemperatur = 0;
float airHumidity = 0;
float soilTemperatur = 21;
uint16_t soilMoisture = 601;
int lightValue = 0;

void config_setup()
{
    wifi_init();
    i2c_setup();
    // vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Alarm and LED setup
    alarm_setup();

    // Sensor setup
    soil_sensor_init();
    light_sensor_init();
    air_sensor_init();

    // Screen setup
    display_init();
}

// Sets the shared u2c gpio config
void i2c_setup()
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
}

void alarm_setup()
{
    // Set gpio config for the Red LED
    gpio_config_t red_led_conf;
    red_led_conf.pin_bit_mask = (1ULL << RED_LED_GPIO);
    red_led_conf.mode = GPIO_MODE_OUTPUT;
    red_led_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&red_led_conf);

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
}

void light_sensor_init()
{
    // Initializes the analog to digital signal from the light sensor pin
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12); // ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)
}

void air_sensor_init()
{
    // Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(am2320_shared_i2c_init(&i2c_am2320, I2C_NUM));
}

void soil_sensor_init()
{
    // Initialize the sensor (shared i2c) only once after boot.
    ESP_ERROR_CHECK(adafruit_stemma_soil_sensor_shared_i2c_init());
}

void display_init()
{
    // Initialize the display (shared i2c) only once after boot.
    i2c_master_shared_i2c_init(&screenDevice);
    vTaskDelay((500) / portTICK_PERIOD_MS);
    ssd1306_init(&screenDevice, 128, 64);
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_contrast(&screenDevice, 0xff);
}