/* 
 * File:   variables.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

//************************************************************************ main
extern cr18_t cr18;
//*****************************************************************************

//************************************************************************ lora
extern ST_UART stUART2;
extern ST_TEMPORIZACAO stTemporizacao;
extern ST_LORA stLoRa;
extern unsigned char byTemDadosParaEnviar;
extern int wRetTrataCmdLoRa;
extern unsigned char byTemRespostaIntermediaria;
extern unsigned char byBufferUART2Tx[TAM_BUFF_TX_UART2];
extern unsigned char byBufferUART2Rx[TAM_BUFF_RX_UART2];
extern unsigned char byBufferAux[32];

extern unsigned char byEnviaBuffer;
//*****************************************************************************

//************************************************************************ uart

//*****************************************************************************


#endif

