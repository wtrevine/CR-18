#include "main.h"

ST_UART stUART2;
ST_TEMPORIZACAO stTemporizacao;
ST_LORA stLoRa;
unsigned char byTemDadosParaEnviar;
int wRetTrataCmdLoRa;
unsigned char byTemRespostaIntermediaria;
unsigned char byBufferUART2Tx[TAM_BUFF_TX_UART2];
unsigned char byBufferUART2Rx[TAM_BUFF_RX_UART2];

unsigned char byEnviaBuffer;

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
            timeout_uart_receive.count_max = 30000;
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
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 10");
                cr18.lora.event.type = EVT_KEEPALIVE;
            }
            else if (cr18.lora.event.alert) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 20");
                cr18.lora.event.type = EVT_ALERT;
            }
            else if (cr18.lora.event.violation) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 30");
                cr18.lora.event.type = EVT_VIOLATION;
            }
            else if (cr18.lora.event.low_battery) {
                sprintf((char*) cr18.uart.buffer_tx, "mac tx cnf 1 40");
                cr18.lora.event.type = EVT_LOW_BATTERY;
            }


            timeout_uart_receive.count_max = 30000;
            cr18.lora.double_return = TRUE;
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
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\naccepted\r\n") == 0)
                error = FALSE;
            break;

        case MAC_SET_ADRON:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0)
                error = FALSE;
            break;

        case MAC_SAVE:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\n") == 0) {
                error = FALSE;
                cr18.status = START;
                cr18.lora.status = READY;
                cr18.lora.config = TRUE;
            }
            break;

        case MAC_TX_CNF:
            if (strcmp((char *) cr18.uart.buffer_rx, "ok\r\nmac_tx_ok\r\n") == 0) {
                error = FALSE;
                cr18.lora.status = READY;
            }
            break;

        default:
            break;
    }
    return error;
}
/*
static void proccess(void) {
    uint16_t i;
    if (stLoRa.byEstadoTrataComandos == AGUARDANDO_ENVIO_DE_COMANDO) {
        stLoRa.byEstadoTrataComandos = AGUARDANDO_RESPOSTA;
        byEnviaBuffer = TRUE;
        byTemRespostaIntermediaria = FALSE;

        //load_command();

        if (byEnviaBuffer == TRUE) {
            for (i = 0; i < TAM_BUFF_RX_UART2; i++) {
                byBufferUART2Rx[i] = 0x00;
            }
            stUART2.wIndexBuffer = 0;
            uart_send(byBufferUART2Tx);
            wRetTrataCmdLoRa = 1; //-- Aguardando Resposta
        }
    } else if (stLoRa.byEstadoTrataComandos == AGUARDANDO_RESPOSTA) {
        if (stUART2.byBufferOk == TRUE) {
            //-- Trata resposta do Comando AT
            wRetTrataCmdLoRa = -1; //-- Erro na resposta

            compare_command();

            stUART2.byBufferOk = FALSE;

        } else {
            if (stLoRa.wTimeOutResposta == 0) {
                //-- Houve Time-out
                stLoRa.byEstadoTrataComandos = AGUARDANDO_ENVIO_DE_COMANDO;
                wRetTrataCmdLoRa = -2; //-- Time-out na resposta do comando
            }
        }
    }
}*/

/*static void check_answer(uint8_t success_state) {
    if (wRetTrataCmdLoRa != 1) {
        if (wRetTrataCmdLoRa == 0) {
            cr18.lora.command = success_state;
            cr18.lora.error_counter = 0;
            stTemporizacao.wT_TimeoutMaqLoRa = K_TEMPO_TIMEOUT_LORA;
            stLoRa.byEstadoTrataComandos = AGUARDANDO_ENVIO_DE_COMANDO;
        } else if (cr18.lora.error_counter++ >= ERROR_NUMBER) {
            cr18.lora.command = SYS_RESET;
            cr18.lora.error_counter = 0;
        }

    }
}*/

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

        case MAC_JOIN_ABP:
            load_command(MAC_JOIN_ABP);
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

    switch (cr18.lora.status) {
        case DISABLED:
            break;

        case CONFIG:
            if (cr18.uart.status == IDLE && cr18.lora.config == FALSE)
                config();
            break;

        case READY:
            if (cr18.uart.status == IDLE && cr18.lora.config == TRUE)
                if (cr18.lora.event.alert || cr18.lora.event.keepalive ||
                        cr18.lora.event.low_battery || cr18.lora.event.violation)
                    cr18.lora.status = SENDING;
            break;

        case SENDING:
            if (cr18.uart.status == IDLE)
                load_command(MAC_TX_CNF);
            break;

        default:
            break;
    }

}
