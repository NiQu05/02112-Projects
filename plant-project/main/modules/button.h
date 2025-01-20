#include "modules/config.h"

#define BUTTON_GPIO 18

void IRAM_ATTR interruptHandler(void * arg);
void initializeInterrupt();
void incrementMenu();