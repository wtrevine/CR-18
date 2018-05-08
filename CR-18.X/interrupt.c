#include "main.h"

void __attribute__((interrupt, auto_psv)) _ISR _T1Interrupt(void)
{
    IFS0bits.T1IF = FALSE;
}

void __attribute__((interrupt, auto_psv)) _ISR _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF = FALSE;
    //unsigned char byDadoRec1;
	
	//byDadoRec1 = U1RXREG;			        // Leitura
  //UART_UART1ProcessaByteRecebido(byDadoRec1);

	IFS0bits.U1RXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _ISR _CNInterrupt(void)
{
    
   
}

void __attribute__((interrupt, auto_psv)) _ISR _HLVDInterrupt(void)
{
    
   
}