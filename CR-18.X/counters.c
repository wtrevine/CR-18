/*includes*/
#include "main.h"

/* temporizaores*/
volatile timeout_t timeout_uart_receive;
volatile timeout_t timeout_alert;
volatile timeout_t timeout_violation;
volatile timeout_t timeout_debounce_alert;
volatile timeout_t timeout_debounce_violation;
volatile timeout_t timeout_debounce_instalation;
volatile timeout_t timeout_keepalive;
volatile timeout_t timeout_instalation;
volatile timeout_t timeout_blink_led_on;
volatile timeout_t timeout_blink_led_off;
volatile timeout_t timeout_disable_lora;
volatile timeout_t timeout_enabling_lora;
volatile timeout_t timeout_sleep;

/*
 * Reseta temporizador
 */
void counters_reset(volatile timeout_t * time, uint8_t enable) {
    time->count = 0;
    time->enable = enable;
    time->overflow = FALSE;
}

/*
 * Processa temporizador
 */
void counters_proccess(volatile timeout_t * sData, uint8_t ReStart) {
    if (sData->enable == TRUE) {
        if ((++sData->count) >= sData->count_max) {
            sData->count = 0;
            sData->overflow = TRUE;
            sData->enable = ReStart;
        }
    } else
        sData->count = 0;
}

/*
 * Chama processos
 *  */
void counters_interrupt(uint8_t type) {
    switch (type) {
        case SECONDS:
            counters_proccess(&timeout_keepalive, FALSE);
            counters_proccess(&timeout_instalation, FALSE);
            counters_proccess(&timeout_alert, FALSE);
            counters_proccess(&timeout_violation, FALSE);
            break;

        case MILLISECONDS:
            counters_proccess(&timeout_uart_receive, FALSE);
            counters_proccess(&timeout_debounce_alert, FALSE);
            counters_proccess(&timeout_debounce_violation, FALSE);
            counters_proccess(&timeout_debounce_instalation, FALSE);
            counters_proccess(&timeout_blink_led_on, FALSE);
            counters_proccess(&timeout_blink_led_off, FALSE);
            counters_proccess(&timeout_disable_lora, FALSE);
            counters_proccess(&timeout_enabling_lora, FALSE);
            counters_proccess(&timeout_sleep, FALSE);
            break;
    }
}

/*
 * Inicia temporizadores
 */
void counters_init() {
    timeout_uart_receive.count_max = TIMEOUT_UART_RECEIVE;
    timeout_alert.count_max = TIMEOUT_ALERT;
    timeout_violation.count_max = TIMEOUT_VIOLATION;
    timeout_debounce_alert.count_max = TIMEOUT_DEBOUNCE_ALERT;
    timeout_debounce_violation.count_max = TIMEOUT_DEBOUNCE_VIOLATION;
    timeout_debounce_instalation.count_max = TIMEOUT_DEBOUNCE_INSTALATION;
    timeout_keepalive.count_max = TIMEOUT_KEEPALIVE;
    timeout_instalation.count_max = TIMEOUT_INSTALATION;
    timeout_blink_led_on.count_max = TIMEOUT_BLINK_LED_ON;
    timeout_blink_led_off.count_max = TIMEOUT_BLINK_LED_OFF;
    timeout_disable_lora.count_max = TIMEOUT_DISABLE_LORA;
    timeout_enabling_lora.count_max = TIMEOUT_ENABLING_LORA;
    timeout_sleep.count_max = TIMEOUT_SLEEP;

}

/* 
 * Verifica estoutoros dos temporizadores
 */
void counters_overflow_proccess(void) {
    /* Timeout de resposta de comando LoRa */
    if (timeout_uart_receive.overflow == TRUE) {
        timeout_uart_receive.overflow = FALSE;
        cr18.uart.status = SEND;
        uart_error(TIMEOUT);
    }

    /* Botão de alarta acionado */
    if (timeout_debounce_alert.overflow == TRUE) {
        timeout_debounce_alert.overflow = FALSE;
        cr18.status = ALERT;
        cr18.lora.event.alert = TRUE;
        T1CONbits.TON = 1;
        counters_reset(&timeout_alert, TRUE);
    }

    /* Botão violado */
    if (timeout_debounce_violation.overflow == TRUE) {
        timeout_debounce_violation.overflow = FALSE;
        cr18.lora.instalation = FALSE;
        cr18.status = VIOLATION;
        cr18.lora.event.violation = TRUE;
        T1CONbits.TON = 1;
        counters_reset(&timeout_violation, TRUE);
    }

    /* Botão instalado */
    if (timeout_debounce_instalation.overflow == TRUE) {
        timeout_debounce_instalation.overflow = FALSE;
        cr18.lora.instalation = TRUE;
        counters_reset(&timeout_instalation, FALSE);
        cr18.status = ACTIVE;
        T1CONbits.TON = 1;
    }

    /* Tempo de exibição modo alerta */
    if (timeout_alert.overflow == TRUE) {
        timeout_alert.overflow = FALSE;
        if (cr18.lora.instalation == TRUE)
            cr18.status = ACTIVE;
        else
            cr18.status = OFF;
    }

    /* Tempo de exibição modo violação */
    if (timeout_violation.overflow == TRUE) {
        timeout_violation.overflow = FALSE;
        if (cr18.lora.instalation == TRUE)
            cr18.status = ACTIVE;
        else
            cr18.status = OFF;
    }

    /* Envia Keepalive */
    if (timeout_keepalive.overflow == TRUE) {
        timeout_keepalive.overflow = FALSE;
        cr18.lora.event.keepalive = TRUE;
        //Alterar evento de keepalive para violação caso violado
    }

    /* Aguarda instalação para enviar violação */
    if (timeout_instalation.overflow == TRUE) {
        timeout_instalation.overflow = FALSE;
        cr18.status = VIOLATION;
        cr18.lora.event.violation = TRUE;
        counters_reset(&timeout_violation, TRUE);
    }

    /* Tempo led aceso */
    if (timeout_blink_led_on.overflow == TRUE) {
        timeout_blink_led_on.overflow = FALSE;
        if (cr18.led.led_color_active == LED_RED)
            RED = 1;
        else if (cr18.led.led_color_active == LED_GREEN)
            GREEN = 1;
        counters_reset(&timeout_blink_led_off, TRUE);
    }

    /* Tempo led apagado */
    if (timeout_blink_led_off.overflow == TRUE) {
        timeout_blink_led_off.overflow = FALSE;
        RED = 0;
        Nop();
        GREEN = 0;
        if (--cr18.led.number_blink != 0)
            counters_reset(&timeout_blink_led_on, TRUE);
    }

    /* Tempo ocioso para desligar lora */
    if (timeout_disable_lora.overflow == TRUE) {
        timeout_disable_lora.overflow = FALSE;
        LORA = TRUE;
        Nop();
        U1STAbits.UTXEN = 0;
        TRISBbits.TRISB14 = 1;
        cr18.lora.status = DISABLED;
    }

    /* Tempo de inicialização lora */
    if (timeout_enabling_lora.overflow == TRUE) {
        timeout_enabling_lora.overflow = FALSE;
        if (cr18.lora.config == TRUE)
            cr18.lora.status = READY;
        else
            cr18.lora.status = CONFIG;
    }

    /* Tempo para entrar em sleep */
    if (timeout_sleep.overflow == TRUE) {
        timeout_sleep.overflow = FALSE;
        Sleep();
    }
}
