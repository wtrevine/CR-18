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

static void load_command() {
    switch (cr18.lora.command) {
        case SYS_RESET:
        {
            sprintf((char *) byBufferUART2Tx, "sys reset\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_RESET:
        {
            sprintf((char *) byBufferUART2Tx, "mac reset\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_SET_DEVADDR:
        {
            sprintf((char *) byBufferUART2Tx, "mac set devaddr 001AD3EE\r\n"); //-- S9
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_SET_NWKSKEY:
        {
            sprintf((char *) byBufferUART2Tx, "mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_SET_APPSKEY:
        {
            sprintf((char *) byBufferUART2Tx, "mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_JOIN_ABP:
        {
            sprintf((char *) byBufferUART2Tx, "mac join abp\r\n");
            stLoRa.wTimeOutResposta = 30;
            byTemRespostaIntermediaria = TRUE;
            break;
        }

        case MAC_SET_ADRON:
        {
            sprintf((char *) byBufferUART2Tx, "mac set adr on\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case MAC_SAVE:
        {
            sprintf((char *) byBufferUART2Tx, "mac save\r\n");
            stLoRa.wTimeOutResposta = 5;
            break;
        }

        case MAC_TX_UNCNF:
        {
            sprintf((char *) byBufferUART2Tx, "mac tx uncnf 1 123ABC\r\n");
            stLoRa.wTimeOutResposta = 30;
            byTemRespostaIntermediaria = TRUE;
            break;
        }

        case MAC_TX_CNF:
        {
            if (byTemDadosParaEnviar == TRUE) {
                stLoRa.wTimeOutResposta = 30;
                byTemRespostaIntermediaria = TRUE;
                byTemDadosParaEnviar = FALSE;
            } else {
                byEnviaBuffer = FALSE;
                wRetTrataCmdLoRa = 0;
            }
            break;
        }

        default:
        {
            cr18.lora.command = SYS_RESET;
            byEnviaBuffer = FALSE;
            wRetTrataCmdLoRa = -3; //-- Estado inválido
            break;
        }
    }
}

static void compare_command() {
    switch (cr18.lora.command) {
        case SYS_RESET:
            wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_RESET:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_SET_DEVADDR:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_SET_NWKSKEY:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_SET_APPSKEY:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_JOIN_ABP:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\naccepted\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_SET_ADRON:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_SAVE:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        case MAC_TX_CNF:
            if (strcmp((char *) byBufferUART2Rx, "ok\r\nmac_tx_ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;

        default:
            cr18.lora.command = SYS_RESET;
            wRetTrataCmdLoRa = -3; //-- Estado Inválido
            break;
    }
}

static void proccess(void) {
    uint16_t i;
    if (stLoRa.byEstadoTrataComandos == AGUARDANDO_ENVIO_DE_COMANDO) {
        stLoRa.byEstadoTrataComandos = AGUARDANDO_RESPOSTA;
        byEnviaBuffer = TRUE;
        byTemRespostaIntermediaria = FALSE;

        load_command();

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
}

static void check_answer(uint8_t success_state) {
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
}

static void config(void) {
    proccess();

    switch (cr18.lora.command) {
        case SYS_RESET:
            check_answer(MAC_RESET);
            break;

        case MAC_RESET:
            check_answer(MAC_SET_DEVADDR);
            //check_answer(MAC_SET_DEVEUI);
            break;

        case MAC_SET_DEVADDR:
            check_answer(MAC_SET_NWKSKEY);
            break;

        case MAC_SET_NWKSKEY:
            check_answer(MAC_SET_APPSKEY);
            break;

        case MAC_SET_APPSKEY:
            check_answer(MAC_JOIN_ABP);
            break;

        case MAC_JOIN_ABP:
            check_answer(MAC_SET_ADRON);
            break;

        case MAC_SET_ADRON:
            check_answer(MAC_SAVE);
            break;

        case MAC_SAVE:
            check_answer(MAC_TX_CNF);
            break;

        default:
            cr18.lora.command = SYS_RESET;
            break;
    }

    cr18.lora.status = READY;
}

static void send(void) {

    proccess();

    switch (cr18.lora.command) {
        case MAC_TX_CNF:
            check_answer(MAC_TX_CNF);
            break;

        default:
            cr18.lora.command = SYS_RESET;
            break;
    }

    cr18.lora.status = READY;
}

void lora_proccess() {

    switch (cr18.lora.status) {
        case DISABLED:
            break;

        case CONFIG:
            config();
            break;

        case READY:
            break;

        case SENDING:
            break;

        default:
            break;
    }

}
