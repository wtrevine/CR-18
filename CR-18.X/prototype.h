/* 
 * File:   prototype.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:33 PM
 */

#ifndef PROTOTYPE_H
#define	PROTOTYPE_H

/* common */
void init_pic(void);
void blink_led(void);

/* app_lora */
void LORA_TrataComandosLoRa(void);
void LORA_VerificaRespostaLoRa(unsigned int wEstadoCasoSucesso, unsigned int wEstadoCasoErro, unsigned int wTentativas);
void LORA_MaquinaDeControleLoRa(void);

/* uart */
void UART_UART2EnviaTxBufferASCII(unsigned char *byBufferTxASCII);

#endif	

