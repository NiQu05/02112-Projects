#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_http_client.h"

#include "../config.h"

// WiFi
#define WIFI_SSID "Nikolaj iPhone"
#define WIFI_PASSWORD "1234abcd"
#define MAXIMUM_RETRY 5
#define HTTP_ENDPOINT "http://172.20.10.10:5120/sensor-data"

void wifi_init_sta(void);
void wifi_init();
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void Http_rest_with_url(void * arg);