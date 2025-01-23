#include "modules/config.h"
#include "modules/screen.h"

// Updates the text value, and the icon, on the screen.
void update_screen_data(uint8_t value, char format[], uint8_t icon[288]){
    static char buffer[24];
    snprintf(buffer, sizeof(buffer), format, value);
    ssd1306_display_text_x3(&screenDevice, 4, buffer, 24, false);
    ssd1306_bitmaps(&screenDevice, ICON_X, ICON_Y, icon, 48, 48, false);
}

// Statup screen that shows title and a logo
void startup_menu(){
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "    Group 10    ", 16, false);
    ssd1306_bitmaps(&screenDevice, ICON_X + 40, ICON_Y, logo_map, 48, 48, false);
}

// Menu that shows the air temperature
void menu_air_temperature(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Temperature   ", 16, false);
    update_screen_data((uint8_t) airTemperature, "  %dC", thermostat_map);
}

// Menu that shows the air humidity
void menu_air_humidity(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "    Humidity    ", 16, false);
    update_screen_data((uint8_t) airHumidity, "  %d%%", humidity_map);
}

// Menu that shows the soil temperature
void menu_soil_temperature(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "Soil Temperature", 16, false);
    update_screen_data((uint8_t) soilTemperature, "  %dC", temp_eco_map);
}

// Menu that shows the soil moisture in %
void menu_soil_moisture(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "  Soil Moisture ", 16, false);
    uint8_t display_soil_moisture = (soilMoisture * 100) / 2000;
    update_screen_data(display_soil_moisture, "  %d%%", water_soil_map);
}

// Menu that shows the light level as a indicator from empty (no light) to full (plenty light)
void menu_ligt_level(){
    display_init();
    ssd1306_clear_screen(&screenDevice, false);
    ssd1306_display_text(&screenDevice, 0, "   Light Level  ", 16, false);
    
    ssd1306_bitmaps(&screenDevice, ICON_X, ICON_Y, light_level_map, 48, 48, false);

    if(lightValue < 500){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light4_map, 16, 48, false);
    }
    else if(lightValue < 700){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light3_map, 16, 48, false);
    }
    else if(lightValue < 900){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light2_map, 16, 48, false);
    }
    else if(lightValue < 1100){
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light1_map, 16, 48, false);
    }
    else{
        ssd1306_bitmaps(&screenDevice, ICON_X + 50, ICON_Y, light0_map, 16, 48, false);
    }
}