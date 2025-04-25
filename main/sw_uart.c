#include "sw_uart.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "esp_check.h"
#include <time.h>
#define MICRO_SECONDS 1000000
static uint16_t baudRate = 9600;
static uint8_t txPin = 5;
static uint8_t rxPin = 6;
static bool isRunning = false;
#define bitTimeUs 1000000 / baudRate
static const char *TAG = "SOFT_UART";


uint8_t readByte()
{
    ets_delay_us(bitTimeUs);
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; ++i)
    {
        // byte <<= 1;
        // byte |= gpio_get_level(rxPin);
        byte |= gpio_get_level(rxPin) << i;

        ets_delay_us(bitTimeUs);
    }
    return byte;
}

void rxTask()
{
    esp_task_wdt_config_t config = {
        .timeout_ms = 10000,
        .idle_core_mask = (1 << 0),
        .trigger_panic = false};
    esp_task_wdt_reconfigure(&config);

    esp_task_wdt_add(xTaskGetCurrentTaskHandle());
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));
    char buffer[128];
    uint8_t i = 0;
    time_t startTime = time(NULL);

    while (isRunning)
    {
        if (gpio_get_level(rxPin))
        {
            // vTaskDelay(1);
            taskYIELD();
        }
        else
        {

            uint8_t byte = readByte();
            buffer[i++] = byte;
            // printf("%d byte %d\n", i, byte);

            if (i == 128 || byte == '\n')
            {
                printf("buffer %s\n", buffer);
                i = 0;
            }
        }
        uint8_t seconds = (time(NULL) - startTime);
        if (seconds >= 9)
        {
            startTime = time(NULL);
            int err = esp_task_wdt_reset();
            if (err != ESP_OK)
            {
                printf("error: %d\n", err);
            }
        }
    }
    esp_task_wdt_delete(NULL);
}

void initUARTTx()
{
    esp_rom_gpio_pad_select_gpio(txPin);
    gpio_set_direction(txPin, GPIO_MODE_OUTPUT);
    gpio_set_level(txPin, 1);
}

void initUARTRx()
{
    esp_rom_gpio_pad_select_gpio(rxPin);
    gpio_set_direction(rxPin, GPIO_MODE_INPUT);
    gpio_pullup_en(rxPin);
}

void startRX()
{
    if (isRunning)
    {
        return;
    }

    isRunning = true;
    xTaskCreate(rxTask, "rxTask", 4096, NULL, 5, NULL);
}
void setTxPin(uint8_t pin)
{
    txPin = pin;
}

void setRxPin(uint8_t pin)
{
    rxPin = pin;
}

uint8_t getTxPin()
{
    return txPin;
}

uint8_t getRxPin()
{
    return rxPin;
}
void setBaundRate(uint16_t rate)
{
    baudRate = rate;
}

uint16_t getBaundRate()
{
    return baudRate;
}

void uart_send(uint8_t data)
{
    gpio_set_level(txPin, 0);
    for (uint8_t i = 0; i < 8; ++i)
    {
        ets_delay_us(bitTimeUs);
        gpio_set_level(txPin, data & 1);
        data >>= 1;
    }
    ets_delay_us(bitTimeUs);
    gpio_set_level(txPin, 1);
    ets_delay_us(bitTimeUs);
}
