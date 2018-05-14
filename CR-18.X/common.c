#include "main.h"

void init_pic(void) {
    /* Configura oscilador */
    OSCCONbits.NOSC = 0b101;
    OSCCONbits.SOSCDRV = 0;
    OSCCONbits.SOSCEN = 0;
    OSCCONbits.OSWEN = 0;

    /* Configura divis�o de oscilador */
    CLKDIVbits.ROI = 0;
    CLKDIVbits.DOZE = 0;
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.RCDIV = 0;

    /* Configura portas analogicas */
    ANSA = 0;
    ANSB = 0;

    /* Configura TIMER 1 */
    T1CONbits.TON = 1;
    T1CONbits.T1ECS = 0b10;
    T1CONbits.TCKPS = 0b00; //Prescale
    T1CONbits.TCS = 1;

    /* Configura UART*/
    U1MODEbits.UARTEN = 0;
    U1MODEbits.USIDL = 1;
    U1STAbits.UTXEN = 1;

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

    /* Configura pinos de mudan�a de estados */
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;

    /* Configura interrup��es */
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

    IFS4bits.HLVDIF = 0;
    IEC4bits.HLVDIE = 1;

    LORA = TRUE;
    RED = FALSE;
    GREEN = FALSE;

}

void init_variables(void) {

    cr18.status = STARTED;
    cr18.lora.status = DISABLED;
    cr18.lora.config = FALSE;
    cr18.uart.status = IDLE;
    
    //wTimer1ContadorTempo1s = K_TEMPO_1_s;
    stTemporizacao.bFlag1s = FALSE;
    //byStatusLedIO6 = '1';

    stUART2.byBufferOk = FALSE;
    stLoRa.byEstadoTrataComandos = AGUARDANDO_ENVIO_DE_COMANDO;
    cr18.lora.command = SYS_RESET;
    stTemporizacao.wT_TimeoutMaqLoRa = 9999;
    byTemDadosParaEnviar = FALSE;
    stTemporizacao.wT_Tx = 10; //-- Primeira vez = 30 segundos
}

void blink_led(void) {
    cr18.led.period++;

    switch (cr18.status) {
        case STARTED:
            if (cr18.led.period >= LED_STARTED_PERIOD)
                cr18.led.period = 0;
            if (cr18.led.period < LED_STARTED) {
                RED = 1;
                GREEN = 0;
            } else {
                RED = 0;
                GREEN = 1;
            }
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