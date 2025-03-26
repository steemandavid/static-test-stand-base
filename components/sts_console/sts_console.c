#include <stdio.h>
#include "sts_console.h"
#include "esp_log.h"
#include "sts_common.h"
#include "esp_console.h"


static const char *TAG_CONSOLE = "STS_CONSOLE";
extern sts_common_context_t sts_common_context;

static void sts_console_wait_for_response(TickType_t timeout, bool print_data_as_string){
    sts_command_t command;
    if(xQueueReceive(sts_common_context.queue_cmd_tx_console, &command, timeout) == pdTRUE){
        //print the command received
        if (print_data_as_string){
            printf("%s\n", command.data);
        }
        else{
            for(int i = 0; i < command.data_len; i++){
                printf("%02x", command.data[i]);
            }
            printf("\n");
        }
    } 
    else {
        printf("Command timed out\n");
    }
}

static int sts_console_cmd_version(int argc, char **argv){
    sts_command_t command = {
        .source = STS_CMD_SOURCE_CONSOLE,
        .type = STS_CMD_VERSION,
        .data_len = 0
    };
    xQueueSend(sts_common_context.queue_cmd_rx, &command, portMAX_DELAY);
    sts_console_wait_for_response(pdMS_TO_TICKS(1000), true);
    return 0;
}

static int sts_console_cmd_start_measurement(int argc, char **argv){
    // Check if there is 2 arg
    if (argc != 2){
        printf("Usage: start_measurement <interval_ms>\n");
        return 1;
    }
    int interval_ms = atoi(argv[1]);
    if (interval_ms < 10){
        printf("Usage: start_measurement <interval_ms>, the interval must be greater than 10ms\n");
        return 1;
    }
    sts_common_context.measurement_interval_ms = interval_ms;
    sts_command_t command = {
        .source = STS_CMD_SOURCE_CONSOLE,
        .type = STS_CMD_START_MEASUREMENT,
        .data_len = 0
    };
    xQueueSend(sts_common_context.queue_cmd_rx, &command, portMAX_DELAY);
    sts_console_wait_for_response(pdMS_TO_TICKS(1000), true);
    return 0;
}

static int sts_console_cmd_stop_measurement(int argc, char **argv){
    sts_command_t command = {
        .source = STS_CMD_SOURCE_CONSOLE,
        .type = STS_CMD_STOP_MEASUREMENT,
        .data_len = 0
    };
    xQueueSend(sts_common_context.queue_cmd_rx, &command, portMAX_DELAY);
    sts_console_wait_for_response(pdMS_TO_TICKS(1000), true);
    return 0;
}

esp_err_t sts_console_init(void){
    ESP_LOGI(TAG_CONSOLE, "STS Console Init");

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = ">";
    repl_config.max_cmdline_length = 1024;

    esp_console_register_help_command();
    esp_console_cmd_t cmd = {
        .command = "version",
        .help = "Prints the version of the firmware",
        .hint = NULL,
        .func = &sts_console_cmd_version
    };
    esp_console_cmd_register(&cmd);
    cmd.command = "start_measurement";
    cmd.help = "Starts the measurement";
    cmd.func = &sts_console_cmd_start_measurement;
    esp_console_cmd_register(&cmd);

    cmd.command = "stop";
    cmd.help = "Stops the measurement";
    cmd.func = &sts_console_cmd_stop_measurement;
    esp_console_cmd_register(&cmd);



    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    esp_console_new_repl_uart(&hw_config, &repl_config, &repl);
    esp_console_start_repl(repl);

    

    return ESP_OK;
}
