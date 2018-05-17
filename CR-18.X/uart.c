#include "main.h"

void uart_init() {

}

void clean_event() {
    if (cr18.lora.status == SENDING) {
        switch (cr18.lora.event.type) {
            case EVT_KEEPALIVE:
                cr18.lora.event.keepalive = FALSE;
                break;
            case EVT_ALERT:
                cr18.lora.event.alert = FALSE;
                break;
            case EVT_VIOLATION:
                cr18.lora.event.violation = FALSE;
                break;
            case EVT_LOW_BATTERY:
                cr18.lora.event.low_battery = FALSE;
                break;
        }
        cr18.lora.status = READY;
        // Prever trash
    }
}

void uart_error(uint8_t error) {

    switch (error) {
        case TIMEOUT:
            if (++cr18.lora.error_timeout >= ERROR_NUMBER) {
                cr18.lora.error_timeout = 0;
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
                clean_event();
            }
            break;

        case BUFFER:
            if (++cr18.lora.error_buffer >= ERROR_NUMBER) {
                cr18.lora.error_buffer = 0;
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
                clean_event();
            }
            break;

        case ASWER:
            if (++cr18.lora.error_aswer >= ERROR_NUMBER) {
                cr18.lora.error_aswer = 0;
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
                clean_event();
            }
            break;

        default:
            break;
    }
}

void uart_receive(uint8_t data) {
    if (cr18.uart.status == RECEIVE) {
        cr18.uart.buffer_rx[cr18.uart.index++] = data;
        if (data == 0x0A) {
            if (cr18.lora.double_return == TRUE) {
                cr18.lora.double_return = FALSE;
            } else {
                counters_reset(&timeout_uart_receive, FALSE);
                cr18.lora.error_timeout = 0;
                cr18.uart.status = PROCCESS;
            }
        }
        if (cr18.uart.index >= SIZE_BUFFER) {
            cr18.uart.index = 0;
            counters_reset(&timeout_uart_receive, FALSE);
            cr18.uart.status = IDLE;
            uart_error(BUFFER);
        } else
            cr18.lora.error_buffer = 0;
    }
}

void uart_send() {
    uint8_t i;
    for (i = 0; cr18.uart.buffer_tx[i] != 0; i++) {
        U1TXREG = cr18.uart.buffer_tx[i];
        while (U1STAbits.TRMT == 0);
    }
}

void uart_proccess() {
    switch (cr18.uart.status) {
        case IDLE:
            break;

        case RECEIVE:
            break;

        case SEND:
            cr18.uart.status = RECEIVE;
            uart_send();
            counters_reset(&timeout_uart_receive, TRUE);
            break;

        case PROCCESS:
            if (lora_compare_command()) {
                cr18.uart.status = SEND;
                uart_error(ASWER);
            } else {
                cr18.uart.status = IDLE;
                cr18.lora.error_aswer = 0;
            }
            cr18.uart.index = 0;
            memset(&cr18.uart.buffer_rx, 0, SIZE_BUFFER);
            break;

        default:
            break;
    }

    if (U1STAbits.OERR == TRUE)
        U1STAbits.OERR = FALSE;
}