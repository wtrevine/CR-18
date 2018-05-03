#include "main.h"

extern cr18_t cr18;

void init_pic(void)
{
  ANSA = 0x0000;
  ANSB = 0x0000;
  
  OSCCONbits.COSC = 0;
  OSCCONbits.NOSC = 0;
  OSCCONbits.CLKLOCK = 0;
  OSCCONbits.LOCK = 0;
  OSCCONbits.SOSCDRV = 0;
  OSCCONbits.SOSCEN = 0;
  OSCCONbits.OSWEN = 0;
  
  CLKDIVbits.ROI = 0;
  CLKDIVbits.DOZE = 0;
  CLKDIVbits.DOZEN = 0;
  CLKDIVbits.RCDIV = 0;
  
  OSCTUNbits.TUN = 0;
    
    PORTA = 0;
    PORTB = 0;
    TRISA = 0;
    TRISB = 0;
    
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB15 = 1;
       
}

void blink_led(void)
{
    cr18.led.period++;
    
    switch(cr18.status) {
        case STARTED:
            if(cr18.led.period >= LED_STARTED_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_STARTED) {
                RED = 1;
                GREEN = 0;
            }
            else {
                RED = 0;
                GREEN = 1;
            }
            break;
            
        case START:
            RED = 0;
            if(cr18.led.period >= LED_START_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_START)
                GREEN = 1;
            else
                GREEN = 0;
            break;
            
        case VIOLATION:
            GREEN = 0;
            if(cr18.led.period >= LED_VIOLATION_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_VIOLATION)
                RED = 1;
            else
                RED = 0;
            break;
            
        case ACTIVE:
            RED = 0;
            if(cr18.led.period >= LED_ACTIVE_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_ACTIVE)
                GREEN = 1;
            else
                GREEN = 0;
            break;
            
        case ALERT:
            GREEN = 0;
            if(cr18.led.period >= LED_ALERT_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_ALERT)
                RED = 1;
            else
                RED = 0;
            break;
            
        case ERROR:
            GREEN = 0;
            if(cr18.led.period >= LED_ERROR_PERIOD)
                cr18.led.period = 0;
            if(cr18.led.period < LED_ERROR)
                RED = 1;
            else
                RED = 0;
            break;
            
        default:
            cr18.led.period = 0;
            RED = 0;
            GREEN = 0;
    }
}