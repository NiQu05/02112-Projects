#include "modules/network/wifi.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

//extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_pem_start");
#define PRIVATE_KEY "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCfWdbEZAJ8EkGb\n93GoreQmfkTrULFhLVc7RhH69JD2b+evXF6ngS0aESoj2tt+yWgvob9Hu3zB0eXH\n1mYfmAIFGDbUjP4WYGj1uuqfyt7tEuHtIZ1r9wIRwgz405bMFZgpkfwwq6QQ8/2c\nazc/pSuEf0GElfkZ1QHJ5z6uPHqz1ApmZJVVlzUa+lj019uFKIJsNPmk01ekZQZk\n38a2YA8OSA9giCLw89VBRvLujN+IunOJBOAoKR67mOhqoLupxoce5rERctnR2RBR\n91FCCJG32s5PWWZ+yA3GzIdYmrg2jZm3va3z540UhFRSQAdAPxvlCIlTcT2Wl9Sx\nw4tfVWGFAgMBAAECggEACXoEyTovYaSOTVG6ay8oPdfNjncGjYJuSSKiKBwKUPWX\nwDiMkoaeoYm6yVYXLqOYtBh48TGnujka4rcP7LoUCKyHej2GUNtb8p0ynd0pQ80X\nLYyNuaF/eIHjVprywWFZYsOBTX2vrKhuATLpHrpaa69LvLBUl6Iw0IqVkjQbkf0m\npAu9Ruo3ZayAi9a3ueZN32VlPx6TktCw3pdPzIxtzSDyGdQHTd8gawryi2f8qXkq\nrTDDv8ypV2DB1bt8KisR5+sr2GmT0MbCX5goMjLm26Xzy1XvgZgtRT06+b5S6g+8\nz9HfaVjN5bOTgrqw6tUz7AasaLK58hL1tzhSW8BXkwKBgQDbcJYgXahc6uzP/M5j\nVlAyGsurefD6frVzMGDV58ZbBB4w0FQdYHpqsAcPrqt4rR470Q8mVdOjYa8jv2Xf\nwPMulyysVRIPsreaXKusFyLpJODLKbNzHtHYhSuymClUxeDCnMgY1oBh91CBtP2A\nfh50TfVwsXfGFCKpGWJV9VGTzwKBgQC55mD5ii1J5HVMx9+5PAMv234VrdXOQru2\n2jw/JLIuG6bh1FdI7dCGosNQW9wC5AdnFeP+0Unx+OzwnI/C38dakS4QMSryflVf\nmkodipdLdwp4Pb0lr6f9ARgyMA0+gDtp76l5L2LROfDHdgA+FA8y9GdwCFUvEv/o\nKOFRhY5GawKBgBTMwy4mMoM8MV41lEqOL+PP7mY+lLzprcFRYJxl78NIzKPL2209\na90Y+lJeOBqkLi11T35OvFTMOZGl3r4qx+Iuqilblu6t/gE/r3YMa162BCi9tyi+\nTx264b139tL2PquzuLPCAUFH8/WXdj/rtG9JxG3+JGA+IFru8Qc8zZK5AoGAAR7V\nWMuvaDGbT2IpBAnLjx9IjELm9f5K+VgpYswK0uJRyCyqdgMQ7a7PdI2JLu9G6CUm\noaOLSY0RiIW5DBijL/WODITvWFTQmOnqJnFuZgMKqhzdJfwADDNUfQDzI3xMX2AV\namiRtOcff2RDrhwa11jHwtIaCVWY+0I70Wz3pY8CgYEAjppW4sxhp/JZVZ87lyRs\nfZ5u4VyeixDYsLimNg0+nvxfhbeJ2h6xjMYOwMvWXVZYUBFB9CYZw9M4ng7svQ1b\njgK30d/cJ5D9YhZMSk/H3/FP3pDCLqCqLBO3wJtd1+f46H2YaPzW0syEUIwYiIfX\nhPixvOn2Cpp6MDTWkOnOHwo=\n-----END PRIVATE KEY-----\n",


static const char *TAG = "[WIFI]";

static int s_retry_num = 0;

void event_handler(void *arg, esp_event_base_t event_base,
                   int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 WIFI_SSID, WIFI_PASSWORD);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 WIFI_SSID, WIFI_PASSWORD);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

void wifi_init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_sta();
    ESP_LOGI(TAG, "Connected to AP, begin http example");

    http_rest_with_url();
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        esp_http_client_set_header(evt->client, "From", "user@example.com");
        esp_http_client_set_header(evt->client, "Accept", "text/html");
        esp_http_client_set_redirection(evt->client);
        break;
    }
    return ESP_OK;
}

static void http_rest_with_url(void)
{
    // Prepare the full authorization header
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Bearer %s", HTTP_BEARER);

    // Configure HTTP client
    esp_http_client_config_t config = {
        .url = HTTP_ENDPOINT,
        .event_handler = _http_event_handler,
        .method = HTTP_METHOD_POST,
        .cert_pem = PRIVATE_KEY // Disable server verification
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    // Set headers
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Authorization", auth_header);

    // Set POST data
    esp_http_client_set_post_field(client, POST_DATA, strlen(POST_DATA));

    // Perform the POST request
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "POST Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "POST request failed: %s", esp_err_to_name(err));
    }

    // Cleanup
    esp_http_client_cleanup(client);
}