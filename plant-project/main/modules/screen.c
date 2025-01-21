#include "modules/config.h"
#include "modules/screen.h"

void update_screen_data(uint8_t value, char format[], uint8_t icon[288]){
    static char buffer[24];
    snprintf(buffer, sizeof(buffer), format, value);
    ssd1306_display_text_x3(&screenDevice, 4, buffer, 24, false);
    ssd1306_bitmaps(&screenDevice, ICON_X, ICON_Y, icon, 48, 48, false);
}

void startup_menu(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "    Group 10    ", 16, false);
    ssd1306_bitmaps(&screenDevice, ICON_X + 40, ICON_Y, logo_map, 48, 48, false);
}

void menu_air_temperature(float * temperatur){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Temperature   ", 16, false);
    update_screen_data((uint8_t) *temperatur, "  %dC", thermostat_map);
}

void menu_air_humidity(float * humidity){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "    Humidity    ", 16, false);
    update_screen_data((uint8_t) *humidity, "  %d%%", humidity_map);
}

void menu_soil_temperature(float * soil_temperatur){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "Soil Temperature", 16, false);
    update_screen_data((uint8_t) *soil_temperatur, "  %dC", temp_eco_map);
}

void menu_soil_moisture(uint16_t * soil_moisture){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Soil Moisture ", 16, false);
    uint8_t display_soil_moisture = (*soil_moisture * 100) / 2000;
    update_screen_data(display_soil_moisture, "  %d%%", water_soil_map);
}

void menu_ligt_level(int * light_level){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "   Light Level  ", 16, false);
    
    ssd1306_bitmaps(&screenDevice, ICON_X, ICON_Y, light_level_map, 48, 48, false);

    if(*light_level < 500){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light4_map, 16, 48, false);
    }
    else if(*light_level < 700){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light3_map, 16, 48, false);
    }
    else if(*light_level < 900){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light2_map, 16, 48, false);
    }
    else if(*light_level < 1100){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light1_map, 16, 48, false);
    }
    else{
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light0_map, 16, 48, false);
    }
}