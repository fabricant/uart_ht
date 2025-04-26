#include "sw_uart.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "esp_check.h"

#define MICRO_SECONDS 1000000

// Default UART settings
static uint16_t baudRate = 9600;
static uint8_t txPin = 5;
static uint8_t rxPin = 7;
static uint32_t bitTimeUs = 0;
uint8_t readByte()
{
    ets_delay_us(bitTimeUs); // Skip start bit
    uint8_t byte = 0;

    // Read 8 bits
    for (uint8_t i = 0; i < 8; ++i)
    {
        byte |= gpio_get_level(rxPin) << i;
        ets_delay_us(bitTimeUs); // Wait for next bit
    }
    return byte;
}

void uart_receive(uint8_t *byte)
{
    // Wait for start bit (0)
    while (gpio_get_level(rxPin))
    {
        taskYIELD();
    }
    *byte = readByte();
}

// Initialize TX pin
void initUARTTx()
{
    bitTimeUs = 1000000 / baudRate;
    esp_rom_gpio_pad_select_gpio(txPin);
    gpio_set_direction(txPin, GPIO_MODE_OUTPUT);
    gpio_set_level(txPin, 1);
}

// Initialize RX pin
void initUARTRx()
{
    bitTimeUs = 1000000 / baudRate;
    esp_rom_gpio_pad_select_gpio(rxPin);
    gpio_set_direction(rxPin, GPIO_MODE_INPUT);
    gpio_pullup_en(rxPin);
}

// Pin configuration functions
void setTxPin(uint8_t pin) { txPin = pin; }
void setRxPin(uint8_t pin) { rxPin = pin; }
uint8_t getTxPin() { return txPin; }
uint8_t getRxPin() { return rxPin; }

// Baud rate configuration functions
void setBaundRate(uint16_t rate)
{
    baudRate = rate;
    bitTimeUs = 1000000 / baudRate;
}
uint16_t getBaundRate() { return baudRate; }

// Function to send a single byte
void uart_send(uint8_t data)
{
    gpio_set_level(txPin, 0); // Start bit 0
    ets_delay_us(bitTimeUs);

    // Send 8 bits
    for (uint8_t i = 0; i < 8; ++i)
    {
        ets_delay_us(bitTimeUs);
        gpio_set_level(txPin, data & 1); // Set pin according to bit value
        data >>= 1;                      // Shift to next bit
    }

    // Stop bit 1
    ets_delay_us(bitTimeUs);
    gpio_set_level(txPin, 1);
    ets_delay_us(bitTimeUs);
}