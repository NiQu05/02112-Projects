#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "esp_lcd_panel_vendor.h"

#include "images/rainy.c"
#include "images/temp.c"

static const char *TAG = "[SCREEN]";

#define I2C_BUS_PORT 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (400 * 1000)
#define EXAMPLE_PIN_NUM_SDA 2
#define EXAMPLE_PIN_NUM_SCL 3
#define EXAMPLE_PIN_NUM_RST -1
#define EXAMPLE_I2C_HW_ADDR 0x3C

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES 128
#define EXAMPLE_LCD_V_RES 64

// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS 8
#define EXAMPLE_LCD_PARAM_BITS 8

// Declare screen objects
lv_disp_t *disp;

lv_obj_t *screen_blank;
lv_obj_t *screen_rain;
lv_obj_t *screen_temp;
lv_obj_t *screen_humidity;
lv_obj_t *screen_cloud;

static lv_style_t style_title;
static lv_style_t style_temp;

void set_screen_blank()
{
    screen_blank = lv_obj_create(NULL); // Create a new screen
    lv_disp_set_default(disp);          // Optional: Set display if needed
    lv_obj_clean(screen_blank);
}

void set_screen_rain()
{
    screen_rain = lv_obj_create(NULL); // Create a new screen

    lv_obj_t *title = lv_label_create(screen_rain);
    lv_label_set_text(title, "Rain Forecasted!");
    lv_obj_set_width(title, disp->driver->hor_res);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 4, 0);

    LV_IMG_DECLARE(cloud_raw);
    lv_obj_t *cloud_img = lv_img_create(screen_rain);
    lv_img_set_src(cloud_img, &cloud_raw);
    lv_obj_align(cloud_img, LV_ALIGN_CENTER, 0, 14);
}

void set_screen_temp(int temp)
{
    screen_temp = lv_obj_create(NULL); // Create a new screen

    // Image
    LV_IMG_DECLARE(temp_raw);
    lv_obj_t *temp_img = lv_img_create(screen_temp);
    lv_img_set_src(temp_img, &temp_raw);
    lv_obj_align(temp_img, LV_ALIGN_BOTTOM_LEFT, 0, 1);

    // lv_obj_t *scr = lv_disp_get_scr_act(disp);
    //  Title
    lv_obj_t *title = lv_label_create(screen_temp);
    lv_obj_add_style(title, &style_title, 0);
    lv_label_set_text(title, "Temperature");
    lv_obj_set_width(title, disp->driver->hor_res);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 16, 0);

    // Label
    static char buffer[12]; // Allocate enough space for a 32-bit int
    snprintf(buffer, sizeof(buffer), "%d°C", temp);

    lv_obj_t *label = lv_label_create(screen_temp);
    lv_obj_add_style(label, &style_temp, 0);
    lv_label_set_text(label, buffer);
    lv_obj_set_width(label, disp->driver->hor_res);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 50, 8);
}

void set_screen_humidity(int humidity)
{
    screen_humidity = lv_obj_create(NULL); // Create a new screen

    // Image
    LV_IMG_DECLARE(temp_raw);
    lv_obj_t *temp_img = lv_img_create(screen_humidity);
    lv_img_set_src(temp_img, &temp_raw);
    lv_obj_align(temp_img, LV_ALIGN_BOTTOM_LEFT, 0, 1);

    // lv_obj_t *scr = lv_disp_get_scr_act(disp);
    //  Title
    lv_obj_t *title = lv_label_create(screen_humidity);
    lv_obj_add_style(title, &style_title, 0);
    lv_label_set_text(title, "Humidity");
    lv_obj_set_width(title, disp->driver->hor_res);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 16, 0);

    // Label
    static char buffer[12]; // Allocate enough space for a 32-bit int
    snprintf(buffer, sizeof(buffer), "%d%%", humidity);

    lv_obj_t *label = lv_label_create(screen_humidity);
    lv_obj_add_style(label, &style_temp, 0);
    lv_label_set_text(label, buffer);
    lv_obj_set_width(label, disp->driver->hor_res);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 50, 8);
}

void setup_display()
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_master_bus_handle_t i2c_bus = NULL;
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = EXAMPLE_PIN_NUM_SDA,
        .scl_io_num = EXAMPLE_PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = EXAMPLE_I2C_HW_ADDR,
        .scl_speed_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,               // According to SSD1306 datasheet
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,   // According to SSD1306 datasheet
        .lcd_param_bits = EXAMPLE_LCD_CMD_BITS, // According to SSD1306 datasheet
        .dc_bit_offset = 6,                     // According to SSD1306 datasheet

    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = EXAMPLE_PIN_NUM_RST,
    };
    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = EXAMPLE_LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Initialize LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES,
        .double_buffer = true,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }};
    disp = lvgl_port_add_disp(&disp_cfg);

    /* Rotation of the screen */
    lv_disp_set_rotation(disp, LV_DISP_ROT_180);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_14);
    lv_style_init(&style_temp);
    lv_style_set_text_font(&style_temp, &lv_font_montserrat_30);

    set_screen_blank(disp);
    set_screen_rain();
    lv_scr_load(screen_blank);

    ESP_LOGI(TAG, "Finished setting up display");
}