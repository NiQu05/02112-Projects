#include "modules/config.h"
#include "modules/bitmaps.h"

#define ICON_Y 16
#define ICON_X 0

void update_screen_data(uint8_t value, char format[], uint8_t icon[288]);
void menu_air_temperature(float * temperatur);
void menu_air_humidity(float * humidity);
void menu_soil_temperature(float * soil_temperatur);
void menu_soil_moisture(uint8_t * soil_moisture);
