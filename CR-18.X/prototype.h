/* 
 * File:   prototype.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:33 PM
 */

#ifndef PROTOTYPE_H
#define	PROTOTYPE_H

/* counters */
void counters_reset(timeout_t * time, uint8_t enable);
void counters_proccess(timeout_t * sData, uint8_t ReStart);
void counters_interrupt();
void counters_init();

/* common */
void init_pic(void);
void init_variables(void);
void blink_led(void);
void cr18_proccess();

/* lora */
void lora_proccess();

/* uart */
void uart_init();
void uart_receive(uint8_t data);
void uart_send(unsigned char *byBufferTxASCII);

#endif	

