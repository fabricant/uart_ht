#include "sw_uart.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void app_main() {
    printf("baund rate = %d\n", getBaundRate());
    // setBaundRate(123);
    printf("new baund rate = %d\n", getBaundRate());
    // initUARTTx();
    initUARTRx();
    startRX();
    // while (1)   
    // {
    //     uart_send(65);
    //     vTaskDelay(1000);
    // }
    
}

// esp_rom_gpio_pad_select_gpio
// gpio_set_direction
// gpio_set_level
// gpio_config
// gpio_config_t