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

//*****************************************************************************

//************************************************************************ uart

//*****************************************************************************

//******************************************************************** counters
extern timeout_t timeout_uart_receive;
extern timeout_t timeout_alert;
extern timeout_t timeout_violation;
extern timeout_t timeout_debounce_alert;
extern timeout_t timeout_debounce_violation;
extern timeout_t timeout_debounce_instalation;
extern timeout_t timeout_keepalive;
extern timeout_t timeout_instalation;
extern timeout_t timeout_blink_led_on;
extern timeout_t timeout_blink_led_off;
extern timeout_t timeout_disable_lora;
extern timeout_t timeout_enabling_lora;
extern timeout_t timeout_sleep;
//*****************************************************************************
#endif

