#include <stdio.h>
#include "sts_common.h"
#include "esp_log.h"

const char *TAG = "STS_COMMON";

esp_err_t sts_common_init(void)
{
    ESP_LOGI(TAG, "STS Common Init");
    return ESP_OK;
}