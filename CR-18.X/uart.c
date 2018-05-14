#include "main.h"

void uart_init() {

}

/*void uart_receive(uint8_t data) {
    if (stUART2.byBufferOk == FALSE) {
 *(byBufferUART2Rx + stUART2.wIndexBuffer++) = data;
        if (data == 0x0A) {
            if (byTemRespostaIntermediaria == TRUE) {
                byTemRespostaIntermediaria = FALSE;
            } else {
                stUART2.byBufferOk = TRUE;
            }
        }
        if (stUART2.wIndexBuffer >= TAM_BUFF_RX_UART2) {
            stUART2.wIndexBuffer = 0;
            stUART2.byBufferOk = FALSE;
        }
    }
}*/

void uart_error(uint8_t error) {

    switch (error) {
        case TIMEOUT:
            if (++cr18.lora.error_timeout >= ERROR_NUMBER) {
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
            }
            break;

        case BUFFER:
            if (++cr18.lora.error_buffer >= ERROR_NUMBER) {
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
            }
            break;

        case ASWER:
            if (++cr18.lora.error_aswer >= ERROR_NUMBER) {
                cr18.uart.status = IDLE;
                cr18.lora.command = COMMAND_NULL;
            }
            break;

        default:
            break;
    }
}

void uart_receive(uint8_t data) {
    if (cr18.uart.status = RECEIVE) {
        cr18.uart.buffer_rx[cr18.uart.index++] = data;
        if (data == 0x0A) {
            if (cr18.lora.double_return == TRUE) {
                cr18.lora.double_return = FALSE;
            } else {
                counters_reset(&timeout_uart_receive, FALSE);
                cr18.uart.status = PROCCESS;
            }
        }
        if (cr18.uart.index >= SIZE_BUFFER) {
            cr18.uart.index = 0;
            counters_reset(&timeout_uart_receive, FALSE);
            cr18.uart.status = IDLE;
            uart_error(BUFFER);
        }
    }
}

/*void uart_send(unsigned char *byBufferTxASCII) {
    uint8_t i;
    uint8_t byByteToTx;

    for (i = 0; *(byBufferTxASCII + i) != 0x00; i++) {
        byByteToTx = *(byBufferTxASCII + i);
        U1TXREG = byByteToTx;
        while (U1STAbits.TRMT);

    }
}*/

void uart_send() {
    uint8_t i;
    for (i = 0; cr18.uart.buffer_tx[i] != 0; i++) {
        U1TXREG = cr18.uart.buffer_tx[i];
        while (U1STAbits.TRMT);
    }
}

void uart_proccess() {
    switch (cr18.uart.status) {
        case IDLE:
            break;

        case RECEIVE:
            break;

        case SEND:
            uart_send();
            cr18.uart.status = RECEIVE;
            counters_reset(&timeout_uart_receive, TRUE);
            break;

        case PROCCESS:
            if (lora_compare_command()) {
                cr18.uart.status = SEND;
                uart_error(ASWER);
            }
            else {
                cr18.uart.status = IDLE;
            }
            break;

        default:
            break;
    }
}