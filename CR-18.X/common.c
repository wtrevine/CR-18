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

    /* Configura TIMER 1 - 10s */
    T1CONbits.TON = 1;
    T1CONbits.T1ECS = 0b10;
    T1CONbits.TCKPS = 0b01; //Prescale = 8
    T1CONbits.TCS = 1;
    TMR1 = TIMER1;
    
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

    /* Configura PORTs */
    PORTA = 0;
    PORTB = 0;
    TRISA = 0;
    TRISB = 0;

    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB15 = 1;

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

    LORA = TRUE;
    Nop();
    RED = FALSE;
    Nop();
    GREEN = FALSE;
}

void init_variables(void) {

    cr18.status = STARTED;

    cr18.lora.status = DISABLED;
    cr18.lora.config = FALSE;
    cr18.lora.command = COMMAND_NULL;

    cr18.uart.status = IDLE;
}

void blink_led(void) {
    switch (cr18.status) {
        case STARTED:
            break;

        case START:
            RED = 0;
            if (cr18.led.period >= LED_START_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_START)
                GREEN = 1;
            else
                GREEN = 0;
            break;

        case VIOLATION:
            GREEN = 0;
            if (cr18.led.period >= LED_VIOLATION_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_VIOLATION)
                RED = 1;
            else
                RED = 0;
            break;

        case ACTIVE:
            RED = 0;
            if (cr18.led.period >= LED_ACTIVE_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_ACTIVE)
                GREEN = 1;
            else
                GREEN = 0;
            break;

        case ALERT:
            GREEN = 0;
            if (cr18.led.period >= LED_ALERT_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_ALERT)
                RED = 1;
            else
                RED = 0;
            break;

        case ERROR:
            GREEN = 0;
            if (cr18.led.period >= LED_ERROR_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_ERROR)
                RED = 1;
            else
                RED = 0;
            break;

        default:
            cr18.led.period = 0;
            RED = 0;
            GREEN = 0;
    }
    cr18.led.period++;
}

void cr18_proccess() {
    switch (cr18.status) {
        case STARTED:
            cr18.lora.status = CONFIG;
            break;

        case START:
            break;

        case VIOLATION:
            break;

        case ACTIVE:
            break;

        case ALERT:
            break;

        case ERROR:
            break;

        default:
            break;
    }
}