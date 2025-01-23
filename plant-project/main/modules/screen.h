#include "modules/config.h"
#include "modules/bitmaps.h"

// Icon placement coordinates
#define ICON_Y 16
#define ICON_X 0

void update_screen_data(uint8_t value, char format[], uint8_t icon[288]);
void startup_menu();
void menu_air_temperature();
void menu_air_humidity();
void menu_soil_temperature();
void menu_soil_moisture();
void menu_ligt_level();
