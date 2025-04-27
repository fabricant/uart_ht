# Software UART Implementation

This project implements a software-based Universal Asynchronous Receiver-Transmitter (UART) communication protocol for ESP32 microcontrollers. It allows for serial communication without relying on dedicated hardware UART peripherals.

## Overview

The implementation consists of two main components:

1. **ESP32 Software UART**: A custom implementation that handles both transmitting and receiving data using GPIO pins.
2. **Arduino Interface**: A companion Arduino sketch that demonstrates communication with the ESP32 using Arduino's SoftwareSerial library.

## Features

- Configurable baud rate (default: 9600)
- Customizable TX and RX pins
- Simple API for sending and receiving data

## Files Structure

- `sw_uart.h`: Header file defining the API
- `sw_uart.c`: Implementation of the software UART functions
- `main.c`: Example application demonstrating TX and RX functionality
- `sketch_apr22a.ino`: Arduino sketch for testing communication

## Usage

### ESP32 Side

1. Initialize the UART pins:

```c
initUARTTx();  // Initialize transmitter
initUARTRx();  // Initialize receiver
```

2. Configure pins (optional, defaults are TX=5, RX=7):

```c
setTxPin(GPIO_NUM);
setRxPin(GPIO_NUM);
```

3. Set baud rate (optional, default is 9600):

```c
setBaundRate(19200);
```

4. Send data:

```c
uart_send(byte);
```

5. Receive data:

```c
uint8_t byte;
uart_receive(&byte);
```

### Arduino Side

The Arduino sketch uses the SoftwareSerial library to communicate with the ESP32:

```arduino
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);  // RX, TX

// Initialize in setup()
mySerial.begin(9600);

// Send data
mySerial.println("Hello, world");

// Receive data
if (mySerial.available() > 0) {
    int receivedByte = mySerial.read();
    // Process received byte
}
```
