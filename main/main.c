/*
   _____ __        __  _         ______          __     _____            __               
  / ___// /_____ _/ /_(_)____   /_  __/__  _____/ /_   / ___/__  _______/ /____  ____ ___ 
  \__ \/ __/ __ `/ __/ / ___/    / / / _ \/ ___/ __/   \__ \/ / / / ___/ __/ _ \/ __ `__ \
 ___/ / /_/ /_/ / /_/ / /__     / / /  __(__  ) /_    ___/ / /_/ (__  ) /_/  __/ / / / / /
/____/\__/\__,_/\__/_/\___/    /_/  \___/____/\__/   /____/\__, /____/\__/\___/_/ /_/ /_/ 
                                                          /____/                                            
*/

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "esp_log.h"

// STS components
#include "sts_common.h"
#include "sts_core.h"
#include "sts_measure.h"
#include "sts_console.h"

sts_common_context_t sts_common_context;

static const char *TAG = "main";

/*
*  Function: catastrophic_failure
*  ------------------------------
*  This function is called when something went horribly wrong during startup.
*  Here would be a good place to blink an red LED or something.
*  After 5 seconds, the ESP32 will be restarted.
*  
*  returns: void
*/
void catastrophic_failure()
{
    ESP_LOGE(TAG, "Catastrophic failure, restarting in 5 seconds...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    esp_restart();
}

void app_main(void)
{
    esp_err_t ret;
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error creating default event loop: %s", esp_err_to_name(ret));
        catastrophic_failure();
    }
    ret = sts_common_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing common components: %s", esp_err_to_name(ret));
        catastrophic_failure();
    }
    ret = sts_core_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing core components: %s", esp_err_to_name(ret));
        catastrophic_failure();
    }
    ret = sts_measure_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing measure components: %s", esp_err_to_name(ret));
        catastrophic_failure();
    }
    ret = sts_console_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing console components: %s", esp_err_to_name(ret));
        catastrophic_failure();
    }
}
