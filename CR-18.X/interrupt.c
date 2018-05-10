#include "main.h"

void __attribute__((interrupt, auto_psv)) _ISR _T1Interrupt(void) {
    IFS0bits.T1IF = FALSE;
}

void __attribute__((interrupt, auto_psv)) _ISR _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = FALSE;
    //unsigned char byDadoRec1;

    //byDadoRec1 = U1RXREG;			        // Leitura
    //UART_UART1ProcessaByteRecebido(byDadoRec1);

}

void __attribute__((interrupt, auto_psv)) _ISR _CNInterrupt(void) {
    IFS1bits.CNIF = FALSE;

    if (BUTTON_FRONT != cr18.bt_front_previous) {
        cr18.bt_front_previous = BUTTON_FRONT;

    }

    if (BUTTON_BACK != cr18.bt_back_previous) {
        cr18.bt_back_previous = BUTTON_BACK;

    }
}

void __attribute__((interrupt, auto_psv)) _ISR _HLVDInterrupt(void) {
    IFS4bits.HLVDIF = FALSE;


}