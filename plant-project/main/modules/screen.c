#include "modules/config.h"
#include "modules/screen.h"

void update_screen_data(uint8_t value, char format[], uint8_t icon[288]){
    static char buffer[24];
    snprintf(buffer, sizeof(buffer), format, value);
    ssd1306_display_text_x3(&screenDevice, 4, buffer, 24, false);
    ssd1306_bitmaps(&screenDevice, ICON_X, ICON_Y, icon, 48, 48, false);
}

void menu_air_temperature(float * temperatur){
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Temperature   ", 16, false);
    update_screen_data((uint8_t) *temperatur, "  %dC", thermostat_map);
}

void menu_air_humidity(float humidity){
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "    Humidity    ", 16, false);
    update_screen_data((uint8_t)humidity, "  %d%%", humidity_map);
}

void menu_soil_temperature(float soil_temperatur){
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "Soil Temperature", 16, false);
    update_screen_data((uint8_t)soil_temperatur, "  %dC", temp_eco_map);
}

void menu_soil_moisture(uint8_t soil_moisture){
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Soil Moisture ", 16, false);
    update_screen_data(soil_moisture, "  %d%%", water_soil_map);
}