/* 
 * File:   definitions.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#define TRUE    1
#define FALSE   0

#define LORA            PORTAbits.RA3
#define RED             PORTBbits.RB9
#define GREEN           PORTBbits.RB8
#define BUTTON_FRONT    PORTBbits.RB4
#define BUTTON_BACK     PORTAbits.RA4

#define LED_START_PERIOD        1000
#define LED_STARTED_PERIOD      500
#define LED_VIOLATION_PERIOD    1000
#define LED_ACTIVE_PERIOD       60000
#define LED_ALERT_PERIOD        1000
#define LED_ERROR_PERIOD        500

#define LED_START               500
#define LED_STARTED             250
#define LED_VIOLATION           100
#define LED_ACTIVE              100
#define LED_ALERT               500
#define LED_ERROR               100

#define TIME_START              3000
#define TIME_VIOLATION          15000
#define TIME_ALERT              15000
#define TIME_ERROR              5000

#define TIMEOUT_UART_RECEIVE    10
#define TIMEOUT_ALERT           1
#define TIMEOUT_VIOLATION       5
#define TIMEOUT_KEEPALIVE       14400 // 24 horas (processado a casa 6 seguntos)
#define TIMEOUT_INSTALATION     20 //6s

#define SIZE_BUFFER 60

#define ERROR_NUMBER 3

//************************************************************************ lora
#define LORA_Aguardando_CR_Inicial              0x00
#define LORA_Aguardando_LF_Inicial              0x01
#define LORA_Aguardando_CR_Intermediario        0x02
#define LORA_Aguardando_LF_Intermediario_ou_O   0x03
#define LORA_Aguardando_O                       0x04
#define LORA_Aguardando_K                       0x05
#define LORA_Aguardando_CR_Final                0x06
#define LORA_Aguardando_LF_Final                0x07

#define AGUARDANDO_ENVIO_DE_COMANDO             0x00
#define AGUARDANDO_RESPOSTA                     0x01

#define GPS_Aguardando_CIFRAO                 0x00
#define GPS_Aguardando_CR                     0x01
#define GPS_Aguardando_LF                     0x02

#define RS485_RX      0
#define RS485_TX      1

#define TAM_BUFF_TX_UART1   128
#define TAM_BUFF_RX_UART1   128
#define TAM_BUFF_TX_UART2   64
#define TAM_BUFF_RX_UART2   64

#define K_TEMPO_TIMEOUT_LORA  30 //-- 60 segundos
//*****************************************************************************



//***************************************************************** struct main

typedef struct {
    uint16_t period;
} led_t;

typedef struct {
    uint8_t status;
    uint8_t index;
    uint8_t buffer_rx[SIZE_BUFFER];
    uint8_t buffer_tx[SIZE_BUFFER];

} uart_t;

typedef struct {
    uint8_t keepalive;
    uint8_t alert;
    uint8_t violation;
    uint8_t low_battrey;
}event_t;

typedef struct {
    uint8_t status;
    uint8_t command;
    uint8_t double_return;
    uint8_t error_timeout;
    uint8_t error_buffer;
    uint8_t error_aswer;
    uint8_t config;
    event_t event;
} lora_t;

typedef struct {
    uint8_t status;
    led_t led;
    lora_t lora;
    uart_t uart;
    uint8_t bt_front_previous;
    uint8_t bt_back_previous;
} cr18_t;
//*****************************************************************************

//***************************************************** struct process_counters
typedef struct {
    uint8_t enable;
    uint8_t overflow;
    uint16_t count;
    uint16_t count_max;
    uint16_t count_overflow;
} timeout_t;
//*****************************************************************************

//***************************************************************** struct lora

typedef struct {
    unsigned char byEstadoTrataComandos; //-- Fazer virar um FLAG
    //unsigned char byAguardaMaisRespostasDoComando;  //-- Fazer virar um FLAG
    unsigned int wTimeOutResposta;
    unsigned char byContadorErrosLoRa;
} ST_LORA;

typedef union {

    struct {
        unsigned char byB0;
        unsigned char byB1;
    } stByte;
    unsigned int wWord;
} UN_INT_TO_CHAR;

typedef struct {
    unsigned char byEstadoAtual; // Estado do tratamento do Buffer entre STX e ETX
    unsigned char byBufferOk;
    unsigned char byTimeOut;
    unsigned int wTimeOut; // Contador de tempo (Time-out)
    unsigned int wIndexBuffer; // Indice do Buffer Rx/Tx
} ST_UART;

typedef struct {
    unsigned bFlag1s : 1; //-- bit 00 --//
    unsigned bFlagEDP0 : 1; //-- bit 01 --//
    unsigned bFlagEDP1 : 1; //-- bit 02 --//
    unsigned bFlagEDP2 : 1; //-- bit 03 --//
    unsigned bFlagEDN : 1; //-- bit 04 --//
    unsigned bFlagVBAT : 1; //-- bit 05 --//
    unsigned bFlagVBAT_BKP : 1; //-- bit 06 --//
    unsigned bFlagACCEL : 1; //-- bit 09 --//
    unsigned bFlagDebounceACCEL : 1; //-- bit 09 --//
    unsigned bFlagTxPrimVez : 1; //-- bit 07 --//
    unsigned bFlag_T_Tx : 1; //-- bit 08 --//
    unsigned bFlag11 : 1; //-- bit 11 --//
    unsigned bFlag12 : 1; //-- bit 12 --//
    unsigned bFlag13 : 1; //-- bit 13 --//
    unsigned bFlag14 : 1; //-- bit 14 --//
    unsigned bFlag15 : 1; //-- bit 15 --//
    unsigned int wContadorTempo1s;
    unsigned int wContadorTempoACCEL;
    unsigned int wContadorTempoDebounceACCEL;
    unsigned int wT_Tx;
    unsigned int wT_TimeoutMaqLoRa;
} ST_TEMPORIZACAO;
//*****************************************************************************

//******************************************************************* enum main

enum status_cr18 {
    STARTED = 0, // Pisca led's indicando inicialização
    START, // Aguarda ser fixado na parede
    VIOLATION, // Violado - Botão Traseiro solto
    ACTIVE, // Ativo - Em funcionamento normal
    ALERT, // Modo alerta - Botão frontal precionado
    ERROR // Modo de erro
};

enum status_lora {
    DISABLED = 0, // Modulo desligado
    CONFIG, // Configurando
    READY, // Modo configurado e pronto para uso
    SENDING, // Fazendo envio de pacote
};

enum status_uart {
    IDLE=0,
    RECEIVE,
    SEND,
    PROCCESS
};

enum error_lora {
    TIMEOUT=0,
    BUFFER,
    ASWER
};

//*****************************************************************************
//******************************************************************* enum lora

typedef enum {
    COMMAND_NULL=0,
    SYS_RESET,
    MAC_RESET,
    MAC_SET_DEVADDR,
    MAC_SET_NWKSKEY,
    MAC_SET_APPSKEY,
    MAC_JOIN_ABP,
    MAC_SET_DEVEUI,
    MAC_SET_APPEUI,
    MAC_SET_APPKEY,
    MAC_JOIN_OTAA,
    MAC_SET_ADRON,
    MAC_SAVE,
    MAC_TX_CNF
} lora_state;
//*****************************************************************************
#endif