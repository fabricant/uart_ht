#include "sw_uart.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void testRX()
{
    char buffer[128] = {0};
    uint8_t i = 0;
    uint8_t byte = 0;

    while (true)
    {
        uart_receive(&byte);
        buffer[i++] = byte;
        // printf("byte %d\n", byte);
        if (i == 128 || byte == '\n')
        {
            printf("Receive %s\n", buffer);
            i = 0;
        }
    }
}

void testTX()
{
    uint8_t i = 0;

    while (true)
    {
        uart_send(i);
        printf("Send %d\n", i++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void app_main()
{
    setBaundRate(19200);
    initUARTTx();
    initUARTRx();
    xTaskCreate(testTX, "testTX", 4096, NULL, 5, NULL);
    xTaskCreate(testRX, "testRX", 4096, NULL, 5, NULL);
}