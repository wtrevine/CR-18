#include "main.h"

ST_UART stUART2;
ST_TEMPORIZACAO stTemporizacao;
EN_LORA_ESTADO_ATUAL enLoRaEstadoAtual;
ST_LORA stLoRa;
unsigned char byTemDadosParaEnviar;
int wRetTrataCmdLoRa;
unsigned char byTemRespostaIntermediaria;
unsigned char byBufferUART2Tx[TAM_BUFF_TX_UART2];
unsigned char byBufferUART2Rx[TAM_BUFF_RX_UART2];
unsigned char byBufferAux[32];

unsigned char byEnviaBuffer;

static void send_command() {
    switch (enLoRaEstadoAtual) {
        case enLoRaEstado_SYS_RESET:
        {
            sprintf((char *) byBufferUART2Tx, "sys reset\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_RESET:
        {
            sprintf((char *) byBufferUART2Tx, "mac reset\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_SET_DEVADDR:
        {
            sprintf((char *) byBufferUART2Tx, "mac set devaddr 001AD3EE\r\n"); //-- S9
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_SET_NWKSKEY:
        {
            sprintf((char *) byBufferUART2Tx, "mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_SET_APPSKEY:
        {
            sprintf((char *) byBufferUART2Tx, "mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_JOIN_ABP:
        {
            sprintf((char *) byBufferUART2Tx, "mac join abp\r\n");
            stLoRa.wTimeOutResposta = 30;
            byTemRespostaIntermediaria = TRUE;
            break;
        }

        case enLoRaEstado_MAC_SET_ADRON:
        {
            sprintf((char *) byBufferUART2Tx, "mac set adr on\r\n");
            stLoRa.wTimeOutResposta = 2;
            break;
        }

        case enLoRaEstado_MAC_SAVE:
        {
            sprintf((char *) byBufferUART2Tx, "mac save\r\n");
            stLoRa.wTimeOutResposta = 5;
            break;
        }

        case enLoRaEstado_MAC_TX_UNCNF:
        {
            sprintf((char *) byBufferUART2Tx, "mac tx uncnf 1 123ABC\r\n");
            stLoRa.wTimeOutResposta = 30;
            byTemRespostaIntermediaria = TRUE;
            break;
        }

        case enLoRaEstado_MAC_TX_CNF:
        {
            if (byTemDadosParaEnviar == TRUE) {
                //sprintf((char *) byBufferUART2Tx, "mac tx cnf 1 %X%X%X%X%X%X%X\r\n", (int) byBufferAux[0], (int) byBufferAux[1], (int) byBufferAux[2], (int) byBufferAux[3], (int) byBufferAux[4], (int) byBufferAux[5], (int) byBufferAux[6]);
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
            enLoRaEstadoAtual = enLoRaEstado_SYS_RESET;
            byEnviaBuffer = FALSE;
            wRetTrataCmdLoRa = -3; //-- Estado inválido
            break;
        }
    }
}

static uint8_t check_ok() {
    if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
        return TRUE;
    return FALSE;
}

static void receive_command() {
    switch (enLoRaEstadoAtual) {
        case enLoRaEstado_SYS_RESET:
        {
            //if (strcmp((char *) byBufferUART2Rx, "RN2903 0.9.5 Sep 02 2015 17:19:55\r\n") == 0)
            wRetTrataCmdLoRa = 0; //-- Resposta OK

            break;
        }

        case enLoRaEstado_MAC_RESET:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_SET_DEVADDR:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_SET_NWKSKEY:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_SET_APPSKEY:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_JOIN_ABP:
        {
            if (strcmp((char *) byBufferUART2Rx, "ok\r\naccepted\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_SET_ADRON:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        case enLoRaEstado_MAC_SAVE:
        {
            //if (strcmp((char *) byBufferUART2Rx, "ok\r\n") == 0)
            if (check_ok())
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }
            /*
            case enLoRaEstado_MAC_TX_UNCNF:
            {
                if (strcmp((char *) byBufferUART2Rx, "ok\r\nmac_tx_ok\r\n") == 0)
                    wRetTrataCmdLoRa = 0; //-- Resposta OK
                break;
            }*/

        case enLoRaEstado_MAC_TX_CNF:
        {
            if (strcmp((char *) byBufferUART2Rx, "ok\r\nmac_tx_ok\r\n") == 0)
                wRetTrataCmdLoRa = 0; //-- Resposta OK
            break;
        }

        default:
        {
            enLoRaEstadoAtual = enLoRaEstado_SYS_RESET;
            wRetTrataCmdLoRa = -3; //-- Estado Inválido
            break;
        }
    }
}

void LORA_TrataComandosLoRa(void) {
    uint16_t wAuxICmd;
    if (stLoRa.byEstadoTrataComandos == AGUARDANDO_ENVIO_DE_COMANDO) {
        stLoRa.byEstadoTrataComandos = AGUARDANDO_RESPOSTA;
        byEnviaBuffer = TRUE;
        byTemRespostaIntermediaria = FALSE;

        //send_command();

        if (byEnviaBuffer == TRUE) {
            for (wAuxICmd = 0; wAuxICmd < TAM_BUFF_RX_UART2; wAuxICmd++) {
                byBufferUART2Rx[wAuxICmd] = 0x00;
            }
            stUART2.wIndexBuffer = 0;
            UART_UART2EnviaTxBufferASCII(byBufferUART2Tx);
            wRetTrataCmdLoRa = 1; //-- Aguardando Resposta
        }
    } else if (stLoRa.byEstadoTrataComandos == AGUARDANDO_RESPOSTA) {
        if (stUART2.byBufferOk == TRUE) {
            //-- Trata resposta do Comando AT
            wRetTrataCmdLoRa = -1; //-- Erro na resposta

            //receive_command();

            stUART2.byBufferOk = FALSE;

            //ClrWdt();
        } else {
            if (stLoRa.wTimeOutResposta == 0) {
                //-- Houve Time-out
                stLoRa.byEstadoTrataComandos = AGUARDANDO_ENVIO_DE_COMANDO;
                wRetTrataCmdLoRa = -2; //-- Time-out na resposta do comando
            }
        }
    }
}

void LORA_VerificaRespostaLoRa(unsigned int wEstadoCasoSucesso, unsigned int wEstadoCasoErro, unsigned int wTentativas) {
    if (wRetTrataCmdLoRa != 1) {
        if (wRetTrataCmdLoRa == 0) {
            enLoRaEstadoAtual = wEstadoCasoSucesso;
            stLoRa.byContadorErrosLoRa = 0;
            stTemporizacao.wT_TimeoutMaqLoRa = K_TEMPO_TIMEOUT_LORA;
            stLoRa.byEstadoTrataComandos = AGUARDANDO_ENVIO_DE_COMANDO;
        } else {
            stLoRa.byContadorErrosLoRa++;
            if (stLoRa.byContadorErrosLoRa >= wTentativas) {
                enLoRaEstadoAtual = wEstadoCasoErro;
                stLoRa.byContadorErrosLoRa = 0;
            }
        }
    }
}

void app_lora_process(void) {
    //ClrWdt();

    LORA_TrataComandosLoRa();

    switch (enLoRaEstadoAtual) {
        case enLoRaEstado_SYS_RESET:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_RESET, enLoRaEstado_SYS_RESET, 5);
            break;
        }

        case enLoRaEstado_MAC_RESET:
        {
            //---------
            //-- ABP --
            //---------
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_DEVADDR, enLoRaEstado_SYS_RESET, 3);
            //----------
            //-- OTAA --
            //----------
            //LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_DEVEUI, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_SET_DEVADDR:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_NWKSKEY, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_SET_NWKSKEY:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_APPSKEY, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_SET_APPSKEY:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_JOIN_ABP, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_JOIN_ABP:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_ADRON, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_SET_ADRON:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SAVE, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_SAVE:
        {
            //------------------------
            //-- TX sem Confirmação --
            //------------------------
            //LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_TX_UNCNF, enLoRaEstado_SYS_RESET, 3);
            //------------------------
            //-- TX com Confirmação --
            //------------------------
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_TX_CNF, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_TX_UNCNF:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_SET_DEVADDR, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        case enLoRaEstado_MAC_TX_CNF:
        {
            LORA_VerificaRespostaLoRa(enLoRaEstado_MAC_TX_CNF, enLoRaEstado_SYS_RESET, 3);
            break;
        }

        default:
        {
            enLoRaEstadoAtual = enLoRaEstado_SYS_RESET;
            break;
        }
    }
}
