#include "main.h"

void __attribute__((interrupt, auto_psv)) _ISR _T1Interrupt(void) {
    IFS0bits.T1IF = FALSE;
    TMR1 = 0;
    counters_interrupt(SECONDS);
    blink_led();
}

void __attribute__((interrupt, auto_psv)) _ISR _T2Interrupt(void) {
    IFS0bits.T2IF = FALSE;
    TMR2 = TIMER2;
    counters_interrupt(MILLISECONDS);
    if (cr18.status == STARTED) {
        if (cr18.led.period >= LED_STARTED_CYCLE)
            cr18.led.period = 0;
        if (cr18.led.period++ < LED_STARTED_PERIOD_ON) {
            RED = 1;
            Nop();
            GREEN = 0;
        } else {
            RED = 0;
            Nop();
            GREEN = 1;
        }
    }
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
            counters_reset(&timeout_debounce_alert, TRUE);
        } else {
            counters_reset(&timeout_debounce_alert, FALSE);
        }
    }

    if (BUTTON_BACK != cr18.bt_back_previous) {
        cr18.bt_back_previous = BUTTON_BACK;
        if (BUTTON_BACK == TRUE) {
            counters_reset(&timeout_debounce_violation, TRUE);
            counters_reset(&timeout_debounce_instalation, FALSE);
        } else {
            counters_reset(&timeout_debounce_violation, FALSE);
            if (cr18.lora.instalation == FALSE)
                counters_reset(&timeout_debounce_instalation, TRUE);
        }
    }
}

void __attribute__((interrupt, auto_psv)) _ISR _HLVDInterrupt(void) {
    IFS4bits.HLVDIF = FALSE;
    //cr18.lora.event.low_battery = TRUE;
}


void __attribute__((interrupt, auto_psv)) _OscillatorFail(void) {
    INTCON1bits.OSCFAIL = 0;
    while(1);
}

void __attribute__((interrupt, auto_psv)) _AddressError(void) {
    INTCON1bits.ADDRERR = 0;
    while(1);
}

void __attribute__((interrupt, auto_psv)) _StackError(void) {
    INTCON1bits.STKERR = 0;
    while(1);
}

void __attribute__((interrupt, auto_psv)) _MathError(void) {
    INTCON1bits.MATHERR = 0;
    while(1);
}