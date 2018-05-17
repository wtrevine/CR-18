/*includes*/
#include "main.h"

/* temporizaores*/
timeout_t timeout_uart_receive;
timeout_t timeout_alert;
timeout_t timeout_violation;
timeout_t timeout_debounce_alert;
timeout_t timeout_debounce_violation;
timeout_t timeout_debounce_instalation;
timeout_t timeout_keepalive;
timeout_t timeout_instalation;
timeout_t timeout_blink_led_on;
timeout_t timeout_blink_led_off;

/*
 * Reseta temporizador
 */
void counters_reset(timeout_t * time, uint8_t enable) {
    time->count = 0;
    time->enable = enable;
    time->overflow = FALSE;
}

/*
 * Processa temporizador
 */
void counters_proccess(timeout_t * sData, uint8_t ReStart) {
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

    /* Bot�o de alarta acionado */
    if (timeout_debounce_alert.overflow == TRUE) {
        timeout_debounce_alert.overflow = FALSE;
        cr18.status = ALERT;
        cr18.lora.event.alert = TRUE;
        T1CONbits.TON = 1;
        counters_reset(&timeout_alert, TRUE);
    }

    /* Bot�o violado */
    if (timeout_debounce_violation.overflow == TRUE) {
        timeout_debounce_violation.overflow = FALSE;
        cr18.lora.instalation = FALSE;
        cr18.status = VIOLATION;
        cr18.lora.event.violation = TRUE;
        T1CONbits.TON = 1;
        counters_reset(&timeout_violation, TRUE);
    }

    /* Bot�o instalado */
    if (timeout_debounce_instalation.overflow == TRUE) {
        timeout_debounce_instalation.overflow = FALSE;
        cr18.lora.instalation = TRUE;
        cr18.status = ACTIVE;
        T1CONbits.TON = 1;
    }

    /* Tempo de exibi��o modo alerta */
    if (timeout_alert.overflow == TRUE) {
        timeout_alert.overflow = FALSE;
        cr18.status = OFF;
        if (cr18.lora.instalation == TRUE)
            cr18.status = ACTIVE;
        else
            cr18.status = OFF;
    }

    /* Tempo de exibi��o modo viola��o */
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
        //Alterar evento de keepalive para viola��o caso violado
    }

    /* Aguarda instala��o para enviar viola��o */
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
}
