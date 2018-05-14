/* Includes */
#include "main.h"

/* Configura registradores */
// FBS
#pragma config BWRP = OFF 
#pragma config BSS = OFF

// FGS
#pragma config GWRP = OFF
#pragma config GSS0 = OFF

// FOSCSEL
#pragma config FNOSC = FRCDIV
#pragma config SOSCSRC = DIG
#pragma config LPRCSEL = HP
#pragma config IESO = OFF

// FOSC
#pragma config POSCMD = NONE
#pragma config OSCIOFNC = ON
#pragma config POSCFREQ = HS
#pragma config SOSCSEL = SOSCHP
#pragma config FCKSM = CSDCMD

// FWDT
#pragma config WDTPS = PS32768
#pragma config FWPSA = PR128
#pragma config FWDTEN = OFF
#pragma config WINDIS = OFF

// FPOR
#pragma config BOREN = BOR0
#pragma config PWRTEN = ON
#pragma config I2C1SEL = PRI
#pragma config BORV = V18

#pragma config MCLRE = ON

// FICD
#pragma config ICS = PGx2 

/* Variaveis globais */
cr18_t cr18;

int main(void) {
    init_pic();
    init_variables();
    counters_init();

    while (1) {
        ClrWdt();
        cr18_proccess();
        lora_proccess();
        uart_proccess();

        if (timeout_uart_receive.overflow == TRUE) {
            timeout_uart_receive.overflow = FALSE;
            cr18.uart.status = SEND;
            uart_error(TIMEOUT);
        }

        if (timeout_alert.overflow == TRUE) {
            timeout_alert.overflow = FALSE;
            cr18.status = ALERT;
            cr18.lora.event.alert = TRUE;
        }

        if (timeout_violation.overflow == TRUE) {
            timeout_violation.overflow = FALSE;
            cr18.status = VIOLATION;
            cr18.lora.event.violation = TRUE;
        }
        
        if (timeout_keepalive.overflow == TRUE) {
            timeout_keepalive.overflow = FALSE;
            cr18.lora.event.keepalive = TRUE;
        }

    }
    return 0;
}