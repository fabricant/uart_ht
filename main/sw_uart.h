#ifndef SW_UART
#define SW_UART
#include <stdint.h>
void initUARTTx();
void initUARTRx();
void startRX();
void stopRX();
void uart_send(uint8_t data);
void setBaundRate(uint16_t rate);
void setTxPin(uint8_t pin);
void setRxPin(uint8_t pin);
void uart_receive(uint8_t *data);
uint8_t getTxPin();
uint8_t getRxPin();
uint16_t getBaundRate();

#endif