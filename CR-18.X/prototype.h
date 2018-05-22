/* 
 * File:   prototype.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:33 PM
 */

#ifndef PROTOTYPE_H
#define	PROTOTYPE_H

/* counters */
void counters_reset(volatile timeout_t * time, uint8_t enable);
void counters_proccess(volatile timeout_t * sData, uint8_t ReStart);
void counters_interrupt(uint8_t type);
void counters_init();
void counters_overflow_proccess(void);

/* common */
void init_pic(void);
void init_variables(void);
void blink_led(void);
void cr18_proccess();

/* lora */
void clean_event();
void lora_proccess();
uint8_t lora_compare_command();

/* uart */
void uart_init();
void uart_error(uint8_t error);
void uart_receive(uint8_t data);
void uart_send();
void uart_proccess();

#endif	

