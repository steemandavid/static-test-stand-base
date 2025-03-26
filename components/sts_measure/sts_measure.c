#include <stdio.h>
#include "sts_measure.h"
#include "esp_log.h"
#include "sts_common.h"
#include "esp_random.h"
#include "esp_timer.h"

const char *TAG_MEASURE = "STS_MEASURE";
extern sts_common_context_t sts_common_context;



void vtask_measure(void *pvParameters)
{
    ESP_LOGI(TAG_MEASURE, "Measure Task Started");
    // Warning this setup will not gather data faster than the tick rate of the system.
    // Default tick rate is 100Hz, but can be changed in the FreeRTOS configuration to 1000Hz.
    while (1)
    {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        if(xEventGroupGetBits(sts_common_context.event_group_core) & STS_MEASURING)
        {
            // Simulate some measurements going on.
            sts_measurement_point_t measurement;
            measurement.timestamp = esp_timer_get_time();
            measurement.type = TEMPERATURE;
            measurement.value = esp_random() % 100;
            xQueueSend(sts_common_context.queue_measure, &measurement, portMAX_DELAY);
            xTaskDelayUntil(&xLastWakeTime, sts_common_context.measurement_interval_ms / portTICK_PERIOD_MS);
        }
        else
        {
            vTaskDelay(100 / portTICK_PERIOD_MS); // Measurement start is only guaranteed to be within 100ms
        }
    }
}

esp_err_t sts_measure_init(void)
{
    ESP_LOGI(TAG_MEASURE, "STS Measure Init");
    xTaskCreate(vtask_measure, "vtask_measure", 4096, NULL, 5, NULL);
    return ESP_OK;
}
