#include "main.h"

void init_pic(void) {
    /* Configura oscilador */
    OSCCONbits.NOSC = 0b111;

    /* Configura divisão de oscilador */
    CLKDIVbits.ROI = 0;
    CLKDIVbits.RCDIV = 0;

    /* Configura portas analogicas */
    ANSA = 0;
    ANSB = 0;

    /* Configura PORTs */
    PORTA = 0;
    PORTB = 0;

    TRISAbits.TRISA3 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB14 = 0;

    /* Configura TIMER 1 - 10s */
    T1CONbits.TON = 1;
    T1CONbits.T1ECS = 0b10;
    T1CONbits.TCKPS = 0b01; //Prescale = 8
    T1CONbits.TCS = 1;
    TMR1 = 0;
    PR1 = TIMER1;

    /* Configura TIMER 2 - 1ms */
    T2CONbits.TMR2ON = 1;
    T2CONbits.T2OUTPS = 0; //1:1
    T2CONbits.T2CKPS = 0b10; //1:16
    TMR2 = TIMER2;

    /* Configura UART*/
    U1MODEbits.UARTEN = 1;
    U1MODEbits.RTSMD = 1;
    U1MODEbits.BRGH = 1; //4
    U1STAbits.UTXEN = 1;
    U1BRG = 16;

    /* Configura PULL UP*/
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;

    /* Configura pinos de mudança de estados */
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;

    /* Configura detector de baixa tensão */
    HLVDCONbits.HLVDEN = 1;
    HLVDCONbits.IRVST = 1;
    HLVDCONbits.HLVDL = 0b0110; //2.23V

    /* Configura interrupções */
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

    IFS4bits.HLVDIF = 0;
    IEC4bits.HLVDIE = 1;

    LORA = FALSE;
    Nop();
    RED = FALSE;
    Nop();
    GREEN = FALSE;
}

void init_variables(void) {

    cr18.status = STARTED;

    cr18.lora.status = DISABLED;
    cr18.lora.config = FALSE;
    cr18.lora.join = FALSE;
    cr18.lora.command = COMMAND_NULL;

    cr18.uart.status = IDLE;

    cr18.button.bt_front_previous = BUTTON_FRONT;
    cr18.button.bt_back_previous = BUTTON_BACK;

    if (BUTTON_BACK == FALSE)
        cr18.lora.instalation = TRUE;

    counters_reset(&timeout_enabling_lora, TRUE);
}

void blink_led(void) {
    switch (cr18.status) {
        case STARTED:
            break;

        case START:
            if (++cr18.led.period >= LED_START_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_START_BLINK;
                cr18.led.led_color_active = LED_GREEN;
                GREEN = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case VIOLATION:
            if (++cr18.led.period >= LED_VIOLATION_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_VIOLATION_BLINK;
                cr18.led.led_color_active = LED_RED;
                RED = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case ACTIVE:
            if (++cr18.led.period >= LED_ACTIVE_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_ACTIVE_BLINK;
                cr18.led.led_color_active = LED_GREEN;
                GREEN = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case ALERT:
            if (++cr18.led.period >= LED_ALERT_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_ALERT_BLINK;
                cr18.led.led_color_active = LED_RED;
                RED = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case ALERT_DISABLE:
            if (++cr18.led.period >= LED_ALERT_DISABLE_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_ALERT_DISABLE_BLINK;
                cr18.led.led_color_active = LED_GREEN;
                GREEN = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case ERROR:
            if (++cr18.led.period >= LED_ERROR_PERIOD) {
                cr18.led.period = 0;
                cr18.led.number_blink = LED_ERROR_BLINK;
                cr18.led.led_color_active = LED_RED;
                RED = 1;
                counters_reset(&timeout_blink_led_off, TRUE);
            }
            break;

        case OFF:
            T1CONbits.TON = 0;
            break;

        default:
            cr18.led.period = 0;
            RED = 0;
            Nop();
            GREEN = 0;
    }
}

void reset_led(void) {
    cr18.led.period = 0;
    RED = 0;
    Nop();
    GREEN = 0;
    IFS0bits.T1IF = TRUE;
}

void cr18_proccess() {
    if (cr18.status != cr18.status_previous) {
        cr18.status_previous = cr18.status;
        reset_led();
    }
    if (LORA == TRUE &&
            cr18.led.number_blink == 0 &&
            timeout_debounce_alert.enable == FALSE &&
            timeout_debounce_alert_disable.enable == FALSE &&
            timeout_debounce_violation.enable == FALSE &&
            timeout_debounce_instalation.enable == FALSE) {
        if (timeout_sleep.enable == FALSE)
            counters_reset(&timeout_sleep, TRUE);
    } else
        counters_reset(&timeout_sleep, FALSE);
}