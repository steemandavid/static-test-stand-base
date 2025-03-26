#pragma once
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#define STS_MEASURE_QUEUE_LENGTH 100

// Evenbits that define various states of the system
static const EventBits_t STS_INIT_DONE = BIT0;              // Initialization is done, can be used by all tasks to check if the system is ready
static const EventBits_t STS_REMOTE_CONNECTED = BIT1;       // Remote is connected
static const EventBits_t STS_REMOTE_DISCONNECTED = BIT2;    // Remote is disconnected, a seperate event bit is used to allow for freeRTOS waiting on this eventbit.
static const EventBits_t STS_DEVICE_ARMED = BIT3;           // Device is armed
static const EventBits_t STS_MEASURING = BIT4;              // Device is measuring

typedef struct 
{
    QueueHandle_t queue_cmd_rx;                 // Queue for commands received from the remote or console
    QueueHandle_t queue_cmd_tx_remote;          // Queue for commands to be sent to the remote
    QueueHandle_t queue_cmd_tx_console;         // Queue for commands to be sent to the console
    QueueHandle_t queue_measure;                // Queue for measurements taken by the sensors and ready for processing
    EventGroupHandle_t event_group_core;        // Event group for signaling between tasks
    uint32_t measurement_interval_ms;           // Interval between measurements
} sts_common_context_t;

typedef enum {
    PRESSURE,
    TEMPERATURE
} sts_measurement_type_t;
typedef struct {
    int64_t timestamp;
    sts_measurement_type_t type;
    float value;
}sts_measurement_point_t;

typedef enum{
    STS_CMD_SOURCE_REMOTE,
    STS_CMD_SOURCE_BASE,
    STS_CMD_SOURCE_CONSOLE
}sts_command_source_t;
typedef enum {
    STS_CMD_PING,
    STS_CMD_VERSION,
    STS_CMD_START_MEASUREMENT,
    STS_CMD_STOP_MEASUREMENT
} sts_command_type_t;
typedef struct 
{
    sts_command_source_t source;
    sts_command_type_t type;
    uint32_t data_len;
    uint8_t uuid[16];
    uint8_t data[32];
} sts_command_t;

esp_err_t sts_common_init(void);