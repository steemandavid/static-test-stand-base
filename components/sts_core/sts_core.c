#include <stdio.h>
#include "sts_core.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_ota_ops.h"
#include "string.h"

#include "sts_common.h"

extern sts_common_context_t sts_common_context;


const char *TAG_CORE = "STS_CORE";

esp_err_t handle_command_version(sts_command_t *rsp)
{
    esp_app_desc_t app_description;
    esp_ota_get_partition_description(esp_ota_get_running_partition(), &app_description);
    rsp->data_len = strlen(app_description.version);
    memcpy(rsp->data, app_description.version, rsp->data_len);
    return ESP_OK;
}



esp_err_t handle_command(sts_command_t *command)
{
    sts_command_t response;
    switch (command->type)
    {
    case STS_CMD_PING:
        /* code */
        break;
    case STS_CMD_VERSION:
        response.source = STS_CMD_SOURCE_BASE;
        response.type = STS_CMD_VERSION;
        handle_command_version(&response);
        break;
    case STS_CMD_START_MEASUREMENT:
        xEventGroupSetBits(sts_common_context.event_group_core, STS_MEASURING);
        break;
    case STS_CMD_STOP_MEASUREMENT:      
        xEventGroupClearBits(sts_common_context.event_group_core, STS_MEASURING);
        break;
    
    default:
        break;
    }

    if(command->source == STS_CMD_SOURCE_REMOTE)
    {
        xQueueSend(sts_common_context.queue_cmd_tx_remote, &response, portMAX_DELAY);
    }
    else if(command->source == STS_CMD_SOURCE_CONSOLE)
    {
        xQueueSend(sts_common_context.queue_cmd_tx_console, &response, portMAX_DELAY);
    }
    
    return ESP_OK;
}

void vtask_core(void *pvParameters)
{
    ESP_LOGI(TAG_CORE, "Core Task Started");
    while (1)
    {
        sts_command_t command;
        if(xQueueReceive(sts_common_context.queue_cmd_rx, &command, portMAX_DELAY) == pdTRUE)
        {
            handle_command(&command);
        }
    }
}

void vtask_measurement_handle(void *pvParameters)
{
    ESP_LOGI(TAG_CORE, "Measurement Handle Task Started");
    while (1)
    {
        sts_measurement_point_t measurement;
        if(xQueueReceive(sts_common_context.queue_measure, &measurement, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(TAG_CORE, "Measurement received: %f", measurement.value);
        }
    }
}

esp_err_t sts_core_init(void)
{
    ESP_LOGI(TAG_CORE, "STS Core Init");
    xTaskCreate(vtask_core, "vtask_core", 4096, NULL, 5, NULL);
    xTaskCreate(vtask_measurement_handle, "vtask_measurement_handle", 4096, NULL, 5, NULL);
    return ESP_OK;
}
