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
cr18_t cr18;

int main (void)
{
    init_pic();
    init_variables();
    
    while(1) {
        
    blink_led();
    }
    return 0;
}