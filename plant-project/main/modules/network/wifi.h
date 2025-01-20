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
//#include "protocol_examples_common.h"
//#include "protocol_examples_utils.h"
#include "esp_tls.h"

#include "esp_http_client.h"

// WiFi
#define WIFI_SSID "Nikolaj iPhone"
#define WIFI_PASSWORD "1234abcd"
#define MAXIMUM_RETRY 5
#define HTTP_ENDPOINT "https://sheets.googleapis.com/v4/spreadsheets/1XJSG7cA65BfSj6b_S_btKm8aCLrrg7alhj8KVbrzOjA/values/Ark1!A1:append?valueInputOption=RAW"
#define HTTP_BEARER "ya29.a0ARW5m75mqyOYTu-ISSLh9oNzJinPL6MPHWOrJbp1zw7nCoWnIQmDMICIXomu3wcaPLUYSecqPODJmG3q5o4Q4ZsTvIyg3TfOUzI4puAUaW2W_OCIvNtuVjzlQdHF5rxt9kfToz4yEUNidtNOfFdp4eEuShW8BNVgRJo98J6xaCgYKAaISARISFQHGX2Mi85JQNWg_8Ntno9cNbmGlfQ0175"
#define POST_DATA "{\"values\":[[\"12\",\"test\"]]}"

void wifi_init_sta(void);
void wifi_init();
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
static void http_rest_with_url(void);

//#include "../config.h"