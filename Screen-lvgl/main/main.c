#include <stdio.h>

#include "screen.c"

void app_main(void)
{
    setup_display();

    set_screen_temp(0);
    set_screen_humidity(0);

    lv_scr_load(screen_cloud);

    lv_scr_load(screen_rain);
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    lv_scr_load(screen_temp);
    vTaskDelay(4000 / portTICK_PERIOD_MS);
    lv_scr_load(screen_humidity);
}