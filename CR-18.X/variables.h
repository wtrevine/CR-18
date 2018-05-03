/* 
 * File:   variables.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

typedef struct {
    uint16_t period;
} led_t;

typedef struct {
    uint8_t enable;
    uint8_t status;   
} lora_t;

typedef struct {
    uint8_t status;
    led_t led;
    lora_t lora;
    
}cr18_t;

enum status {
    STARTED=0,              // Pisca led's indicando inicialização
    START,                  // Aguarda ser fixado na parede
    VIOLATION,              // Violado - Botão Traseiro solto
    ACTIVE,                 // Ativo - Em funcionamento normal
    ALERT,                  // Modo alerta - Botão frontal precionado
    ERROR                   // Modo de erro
};

enum status_lora {
    DINABLE=0,              // Modulo desligado
    CONFIG,                 // Configurando
    READY,                  // Modo configurado e pronto para uso
    SEND                    // Fazendo envio de pacote
};

enum comunication {
    KEEPALIVE=0    
};



#endif

