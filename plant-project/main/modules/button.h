#include "modules/config.h"

#define BUTTON_GPIO 4

void IRAM_ATTR button1InterruptHandler(void * arg);
void IRAM_ATTR button2InterruptHandler(void * arg);
void initializeInterrupt();
void incrementMenu();