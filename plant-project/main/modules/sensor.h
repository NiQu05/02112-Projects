#include "modules/config.h"

// Debuggin Tags
#define TAG_LIGHT_SENSOR "[Light Sensor]"
#define TAG_AIR_TEMPERATUR "[Air Temperatur]"
#define TAG_AIR_HUMIDITY "[Air Humidity]"
#define TAG_SOIL_TEMPERATUR "[Soil Temperatur]"
#define TAG_SOIL_MOISTURE "[Soil Moisture]"

int get_lightvalue();
float get_temperatur();
float get_humidity();
float get_soil_temperatur();
uint16_t get_soil_moisture();