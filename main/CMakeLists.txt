idf_component_register(SRCS "main.c" "sw_uart.c"
                    PRIV_REQUIRES spi_flash esp_driver_gpio esp_timer esp_hw_support
                    "")
