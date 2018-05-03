/* 
 * File:   definitions.h
 * Author: William Trevine
 *
 * Created on April 30, 2018, 3:24 PM
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#define ENABLE          PORTAbits.RA3
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

#endif