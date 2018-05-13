#include "main.h"

void uart_init() {
    
}

void uart_receive(uint8_t data) {
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
}

void uart_send(unsigned char *byBufferTxASCII) {
    uint8_t i;
    uint8_t byByteToTx;

    for (i = 0; *(byBufferTxASCII + i) != 0x00; i++) {
        byByteToTx = *(byBufferTxASCII + i);
        U1TXREG = byByteToTx;
        while (U1STAbits.TRMT);

    }
}
