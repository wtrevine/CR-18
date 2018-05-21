#include "main.h"

void clean_event() {
    if (cr18.lora.status == SENDING) {
        switch (cr18.lora.event.type) {
            case EVT_KEEPALIVE:
                cr18.lora.event.keepalive = FALSE;
                break;
            case EVT_ALERT:
                cr18.lora.event.alert = FALSE;
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

        case MAC_SET_DEVADDR:
            sprintf((char*) cr18.uart.buffer_tx, "mac set devaddr 001AD3EE\r\n"); //-- S9
            break;

        case MAC_SET_NWKSKEY:
            sprintf((char*) cr18.uart.buffer_tx, "mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            break;

        case MAC_SET_APPSKEY:
            sprintf((char*) cr18.uart.buffer_tx, "mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            break;

        case MAC_JOIN_ABP:
            sprintf((char*) cr18.uart.buffer_tx, "mac join abp\r\n");
            timeout_uart_receive.count_max = 5000;
            cr18.lora.double_return = TRUE;
            break;

        case MAC_SET_ADRON:
            sprintf((char*) cr18.uart.buffer_tx, "mac set adr on\r\n");
            break;

        case MAC_SAVE:
            sprintf((char*) cr18.uart.buffer_tx, "mac save\r\n");
            timeout_uart_receive.count_max = 5000;
            break;

        case MAC_TX_CNF:
            if (cr18.lora.event.keepalive) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 10\r\n");
                cr18.lora.event.type = EVT_KEEPALIVE;
            } else if (cr18.lora.event.alert) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 20\r\n");
                cr18.lora.event.type = EVT_ALERT;
            } else if (cr18.lora.event.violation) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 30\r\n");
                cr18.lora.event.type = EVT_VIOLATION;
            } else if (cr18.lora.event.low_battery) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 40\r\n");
                cr18.lora.event.type = EVT_LOW_BATTERY;
            }
            timeout_uart_receive.count_max = 5000;
            cr18.lora.double_return = TRUE;
            break;

        case MAC_PAUSE:
            sprintf((char*) cr18.uart.buffer_tx, "mac pause\r\n");
            timeout_uart_receive.count_max = 10000;
            break;

        case RADIO_TX:
            if (cr18.lora.event.keepalive) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 10\r\n");
                cr18.lora.event.type = EVT_KEEPALIVE;
            } else if (cr18.lora.event.alert) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 20\r\n");
                cr18.lora.event.type = EVT_ALERT;
            } else if (cr18.lora.event.violation) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 30\r\n");
                cr18.lora.event.type = EVT_VIOLATION;
            } else if (cr18.lora.event.low_battery) {
                sprintf((char*) cr18.uart.buffer_tx, "radio tx 40\r\n");
                cr18.lora.event.type = EVT_LOW_BATTERY;
            }
            timeout_uart_receive.count_max = 5000;
            break;

        default:
            break;
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

        case MAC_SET_DEVADDR:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_SET_NWKSKEY:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_SET_APPSKEY:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_JOIN_ABP:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\naccepted\r\n") == 0) {
                error = FALSE;
                cr18.lora.status = READY;
                cr18.lora.join = TRUE;
            }
            break;

        case MAC_SET_ADRON:
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

        case MAC_TX_CNF:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\nmac_tx_ok\r\n") == 0) {
                error = FALSE;
                clean_event();
                cr18.lora.status = READY;
            }
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

/* 
 * Encaminha proximo comando para ser carreado
 */
static void config(void) {

    switch (++cr18.lora.command) {
        case COMMAND_NULL:
            break;

        case SYS_RESET:
            load_command(SYS_RESET);
            break;

        case MAC_RESET:
            load_command(MAC_RESET);
            break;

        case MAC_SET_DEVADDR:
            load_command(MAC_SET_DEVADDR);
            break;

        case MAC_SET_NWKSKEY:
            load_command(MAC_SET_NWKSKEY);
            break;

        case MAC_SET_APPSKEY:
            load_command(MAC_SET_APPSKEY);
            break;

        case MAC_SET_ADRON:
            load_command(MAC_SET_ADRON);
            break;

        case MAC_SAVE:
            load_command(MAC_SAVE);
            break;

        default:
            cr18.lora.command = COMMAND_NULL;
            break;
    }
}

void lora_proccess() {
    if (cr18.uart.status != IDLE)
        return;

    switch (cr18.lora.status) {
        case DISABLED:
            if (cr18.lora.event.alert || cr18.lora.event.keepalive ||
                    cr18.lora.event.low_battery || cr18.lora.event.violation) {
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
            config();
            break;

        case JOIN:
            load_command(MAC_JOIN_ABP);
            break;

        case READY:
            if (cr18.lora.event.alert || cr18.lora.event.keepalive ||
                    cr18.lora.event.low_battery || cr18.lora.event.violation) {
                cr18.lora.status = SENDING;
                counters_reset(&timeout_disable_lora, FALSE);
            } else if (timeout_disable_lora.enable == FALSE)
                counters_reset(&timeout_disable_lora, TRUE);
            break;

        case SENDING:
#ifdef LORAWAN
            if (cr18.lora.join == FALSE)
                load_command(MAC_JOIN_ABP);
            else if (cr18.uart.status == IDLE)
                load_command(MAC_TX_CNF);
#else
            if (cr18.lora.pause == FALSE)
                load_command(MAC_PAUSE);
            else
                load_command(RADIO_TX);
#endif
            break;

        default:
            break;
    }
}
