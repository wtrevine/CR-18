#include "main.h"

void clean_event() {
    if (cr18.lora.status == SENDING) {
        switch (cr18.lora.event.type) {
            case EVT_ALERT:
                cr18.lora.event.alert = FALSE;
                break;
            case EVT_ALERT_DISABLE:
                cr18.lora.event.alert_disable = FALSE;
                break;
            case EVT_KEEPALIVE:
                cr18.lora.event.keepalive = FALSE;
                break;
            case EVT_VIOLATION:
                cr18.lora.event.violation = FALSE;
                break;
            case EVT_LOW_BATTERY:
                cr18.lora.event.low_battery = FALSE;
                break;
        }
        cr18.lora.status = READY;
        // Prever trash
    }
}

/* 
 * Carrega comando no buffer TX e encaminha para enviar
 */
static void load_command(uint8_t command) {

    timeout_uart_receive.count_max = 2000;
    memset(&cr18.uart.buffer_tx, 0, SIZE_BUFFER);
    cr18.lora.command = command;

    switch (command) {
        case SYS_RESET:
            sprintf((char*) cr18.uart.buffer_tx, "sys reset\r\n");
            break;

        case MAC_RESET:
            sprintf((char*) cr18.uart.buffer_tx, "mac reset\r\n");
            break;

        case MAC_SAVE:
            sprintf((char*) cr18.uart.buffer_tx, "mac save\r\n");
            break;

        case RADIO_SET_MOD:
            sprintf((char*) cr18.uart.buffer_tx, "radio set mod lora\r\n");
            break;

        case RADIO_SET_FREQ:
            sprintf((char*) cr18.uart.buffer_tx, "radio set freq 923300000\r\n");
            break;

        case RADIO_SET_PWR:
            sprintf((char*) cr18.uart.buffer_tx, "radio set pwr 14\r\n");
            break;

        case RADIO_SET_SF:
            sprintf((char*) cr18.uart.buffer_tx, "radio set sf sf10\r\n");
            break;

        case RADIO_SET_CRC:
            sprintf((char*) cr18.uart.buffer_tx, "radio set crc on\r\n");
            break;

        case RADIO_SET_CR:
            sprintf((char*) cr18.uart.buffer_tx, "radio set cr 4/5\r\n");
            break;

        case RADIO_SET_SYNC:
            sprintf((char*) cr18.uart.buffer_tx, "radio set sync 55\r\n");
            break;

        case RADIO_SET_BW:
            sprintf((char*) cr18.uart.buffer_tx, "radio set bw 125\r\n");
            break;

        case MAC_PAUSE:
            sprintf((char*) cr18.uart.buffer_tx, "mac pause\r\n");
            break;

        case RADIO_TX:
            if (cr18.lora.event.alert) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 10\r\n");
                cr18.lora.event.type = EVT_ALERT;
            } else if (cr18.lora.event.alert_disable) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 20\r\n");
                cr18.lora.event.type = EVT_ALERT_DISABLE;
            } else if (cr18.lora.event.keepalive) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 30\r\n");
                cr18.lora.event.type = EVT_KEEPALIVE;
            } else if (cr18.lora.event.violation) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 40\r\n");
                cr18.lora.event.type = EVT_VIOLATION;
            } else if (cr18.lora.event.low_battery) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 50\r\n");
                cr18.lora.event.type = EVT_LOW_BATTERY;
            }
            break;

        default:
            return;
    }
    cr18.uart.status = SEND;
}

/* 
 * Compara resposta e retorna possivel erro
 */
uint8_t lora_compare_command() {
    uint8_t error = TRUE;
    switch (cr18.lora.command) {
        case SYS_RESET:
            error = FALSE;
            break;

        case MAC_RESET:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_SAVE:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0) {
                error = FALSE;
                cr18.lora.status = READY;
                cr18.lora.config = TRUE;
                if (cr18.lora.instalation == FALSE) {
                    counters_reset(&timeout_instalation, TRUE);
                    cr18.status = START;
                } else
                    cr18.status = ACTIVE;
            }
            break;

        case RADIO_SET_MOD:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_FREQ:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_PWR:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_SF:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_CRC:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_CR:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_SYNC:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case RADIO_SET_BW:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_PAUSE:
            if (strcmp((char *) cr18.uart.buffer_rx, "4294967245\r\n") == 0) {
                cr18.lora.status = READY;
                cr18.lora.pause = TRUE;
                error = FALSE;
            }
            break;

        case RADIO_TX:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0) {
                clean_event();
                cr18.lora.status = READY;
                error = FALSE;
            }
            break;

        default:
            break;
    }
    return error;
}

void lora_proccess() {
    if (cr18.uart.status != IDLE)
        return;

    switch (cr18.lora.status) {
        case DISABLED:
            if (cr18.lora.event.alert || cr18.lora.event.alert_disable ||
                    cr18.lora.event.keepalive || cr18.lora.event.low_battery ||
                    cr18.lora.event.violation) {
                if (timeout_enabling_lora.enable == FALSE) {
                    counters_reset(&timeout_enabling_lora, TRUE);
                    LORA = FALSE;
                    Nop();
                    TRISBbits.TRISB14 = 0;
                    U1STAbits.UTXEN = 1;
                    cr18.lora.pause = FALSE;
                }
            }
            break;

        case CONFIG:
            if (cr18.lora.command > MAC_SAVE)
                cr18.lora.command = COMMAND_NULL;
            load_command(++cr18.lora.command);
            break;

        case READY:
            if (cr18.lora.event.alert || cr18.lora.event.alert_disable ||
                    cr18.lora.event.keepalive || cr18.lora.event.low_battery ||
                    cr18.lora.event.violation) {
                cr18.lora.status = SENDING;
                counters_reset(&timeout_disable_lora, FALSE);
            } else if (timeout_disable_lora.enable == FALSE)
                counters_reset(&timeout_disable_lora, TRUE);
            break;

        case SENDING:
            if (cr18.lora.pause == FALSE) {
                if (cr18.lora.command < RADIO_SET_MOD || cr18.lora.command > MAC_PAUSE)
                    cr18.lora.command = RADIO_SET_MOD - 1;
                load_command(++cr18.lora.command);
            } else
                load_command(RADIO_TX);
            break;

        default:
            break;
    }
}
