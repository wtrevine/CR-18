/* 
 * File:   definitions.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#define FCY 16000UL

#define TIMER1 38750 // 10s = 10 / (1/31000 * 8)
#define TIMER2 5 // 1ms = 255 - (0,001 / (1/4000000 * 16))

#define TRUE    1
#define FALSE   0

/* Ports */
#define LORA            PORTAbits.RA3
#define RED             PORTBbits.RB9
#define GREEN           PORTBbits.RB8
#define BUTTON_FRONT    PORTAbits.RA4
#define BUTTON_BACK     PORTBbits.RB4

/* Time leds */
#define LED_STARTED_CYCLE       200
#define LED_STARTED_PERIOD_ON   100

#define LED_START_PERIOD        1
#define LED_START_BLINK         2
#define LED_VIOLATION_PERIOD    1
#define LED_VIOLATION_BLINK     5
#define LED_ACTIVE_PERIOD       1 //6
#define LED_ACTIVE_BLINK        1
#define LED_ALERT_PERIOD        1
#define LED_ALERT_BLINK         2
#define LED_ERROR_PERIOD        1
#define LED_ERROR_BLINK         10

/* Timeout 1 MILLISECONDS */
#define TIMEOUT_UART_RECEIVE            2000    // Timeout para resposta do Lora
#define TIMEOUT_DEBOUNCE_ALERT          50      // Timeout de debounce botão alerta
#define TIMEOUT_DEBOUNCE_VIOLATION      1500    // Timeout de debounce botão violado
#define TIMEOUT_DEBOUNCE_INSTALATION    3000    // Timeout de debounce botão instalado
#define TIMEOUT_BLINK_LED_ON            100     // Timeout led ligado
#define TIMEOUT_BLINK_LED_OFF           100     // Timeout led desligado
#define TIMEOUT_DISABLE_LORA            500     // Timeout ocioso para desligar lora
#define TIMEOUT_ENABLING_LORA           1000    // Timeout para start do lora
#define TIMEOUT_SLEEP                   50      // Timeout para entrat em sleep

/* Timeout 10 SECONDS */
#define TIMEOUT_KEEPALIVE       8640    // Timeout para envio de keepalive (24 horas)
#define TIMEOUT_INSTALATION     6       // Timeout para tempo de instação, antes de habilitar violação
#define TIMEOUT_ALERT           6       // Timeout alerta
#define TIMEOUT_VIOLATION       6       // Timeout violação

#define SIZE_BUFFER 60

/* Erros de serial para reinicializar */
#define ERROR_NUMBER 3

//************************************************************************ lora

//*****************************************************************************

//***************************************************************** struct main

typedef struct {
    uint16_t period;
    uint8_t number_blink;
    uint8_t led_color_active;
} led_t;

typedef struct {
    uint8_t status;
    uint8_t index;
    uint8_t buffer_rx[SIZE_BUFFER];
    uint8_t buffer_tx[SIZE_BUFFER];

} uart_t;

typedef struct {
    uint8_t type;
    uint8_t keepalive;
    uint8_t alert;
    uint8_t violation;
    uint8_t low_battery;
} event_t;

typedef struct {
    uint8_t status;
    uint8_t command;
    uint8_t double_return;
    uint8_t error_timeout;
    uint8_t error_buffer;
    uint8_t error_aswer;
    uint8_t config;
    uint8_t join;
    uint8_t pause;
    uint8_t instalation;
    event_t event;
} lora_t;

typedef struct {
    uint8_t status;
    uint8_t status_previous;
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


//*****************************************************************************

//******************************************************************* enum main

enum status_cr18 {
    STARTED = 0, // Pisca led's indicando inicialização
    START, // Aguarda ser fixado na parede
    VIOLATION, // Violado - Botão Traseiro solto
    ACTIVE, // Ativo - Em funcionamento normal
    ALERT, // Modo alerta - Botão frontal precionado
    ERROR, // Modo de erro
    OFF // Led desligado - Aguardando instalação

};

enum status_lora {
    DISABLED = 0, // Modulo desligado
    CONFIG, // Configurando
    JOIN, // Juntando-se a rede LoRaWan
    READY, // Modo configurado e pronto para uso
    SENDING, // Fazendo envio de pacote
};

enum status_uart {
    IDLE = 0,
    RECEIVE,
    SEND,
    PROCCESS
};

enum error_lora {
    TIMEOUT = 0,
    BUFFER,
    ASWER
};

enum type_time {
    SECONDS = 0,
    MILLISECONDS
};

enum type_event {
    EVT_KEEPALIVE = 0,
    EVT_ALERT,
    EVT_VIOLATION,
    EVT_LOW_BATTERY
};

enum type_led {
    LED_RED = 0,
    LED_GREEN
};

//*****************************************************************************
//******************************************************************* enum lora

typedef enum {
    COMMAND_NULL = 0,
    SYS_RESET,
    MAC_RESET,
    MAC_SET_DEVADDR,
    MAC_SET_NWKSKEY,
    MAC_SET_APPSKEY,
    //MAC_SET_DEVEUI,
    //MAC_SET_APPEUI,
    //MAC_SET_APPKEY,
    //MAC_JOIN_OTAA,
    MAC_SET_ADRON,
    MAC_SAVE,
    MAC_JOIN_ABP,
    MAC_TX_CNF,
    MAC_PAUSE,
    RADIO_TX
} lora_state;
//*****************************************************************************
#endif