#include "main.h"

void __attribute__((interrupt, auto_psv)) _ISR _T1Interrupt(void) {
    IFS0bits.T1IF = FALSE;
    blink_led();
}

void __attribute__((interrupt, auto_psv)) _ISR _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = FALSE;
    uart_receive(U1RXREG);
}

void __attribute__((interrupt, auto_psv)) _ISR _CNInterrupt(void) {
    IFS1bits.CNIF = FALSE;

    if (BUTTON_FRONT != cr18.bt_front_previous) {
        cr18.bt_front_previous = BUTTON_FRONT;
        if (BUTTON_FRONT == FALSE) {
            counters_reset(&timeout_alert, TRUE);
        } else {
            counters_reset(&timeout_alert, FALSE);
        }
    }

    if (BUTTON_BACK != cr18.bt_back_previous) {
        cr18.bt_back_previous = BUTTON_BACK;
        if (BUTTON_BACK == TRUE) {
            counters_reset(&timeout_violation, TRUE);
        } else {
            counters_reset(&timeout_violation, FALSE);
        }
    }
}

void __attribute__((interrupt, auto_psv)) _ISR _HLVDInterrupt(void) {
    IFS4bits.HLVDIF = FALSE;
    cr18.lora.event.low_battrey = TRUE;

}