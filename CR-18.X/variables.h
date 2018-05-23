/* 
 * File:   variables.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef VARIABLES_H
#define	VARIABLES_H

//************************************************************************ main
extern volatile cr18_t cr18;
//*****************************************************************************

//************************************************************************ lora

//*****************************************************************************

//************************************************************************ uart

//*****************************************************************************

//******************************************************************** counters
extern volatile timeout_t timeout_uart_receive;
extern volatile timeout_t timeout_alert;
extern volatile timeout_t timeout_violation;
extern volatile timeout_t timeout_debounce_alert;
extern volatile timeout_t timeout_debounce_alert_disable;
extern volatile timeout_t timeout_debounce_violation;
extern volatile timeout_t timeout_debounce_instalation;
extern volatile timeout_t timeout_keepalive;
extern volatile timeout_t timeout_instalation;
extern volatile timeout_t timeout_blink_led_on;
extern volatile timeout_t timeout_blink_led_off;
extern volatile timeout_t timeout_disable_lora;
extern volatile timeout_t timeout_enabling_lora;
extern volatile timeout_t timeout_sleep;
//*****************************************************************************
#endif

