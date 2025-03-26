#include <stdio.h>
#include "sts_common.h"
#include "esp_log.h"

const char *TAG = "STS_COMMON";
extern sts_common_context_t sts_common_context;

esp_err_t sts_common_init(void)
{
    ESP_LOGI(TAG, "STS Common Init");
    sts_common_context.queue_measure = xQueueCreate(STS_MEASURE_QUEUE_LENGTH, sizeof(sts_measurement_point_t));
    sts_common_context.queue_cmd_rx = xQueueCreate(10, sizeof(sts_command_t));
    sts_common_context.queue_cmd_tx_remote = xQueueCreate(10, sizeof(sts_command_t));
    sts_common_context.queue_cmd_tx_console = xQueueCreate(10, sizeof(sts_command_t));
    sts_common_context.event_group_core = xEventGroupCreate();
    sts_common_context.measurement_interval_ms = 1000;
    return ESP_OK;
}