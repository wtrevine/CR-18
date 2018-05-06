#include "main.h"

void UART_UART2EnviaTxBufferASCII(unsigned char *byBufferTxASCII)
{
  unsigned int  wITxBufferASCII;
  unsigned char byByteToTx;
      
  for (wITxBufferASCII = 0; *(byBufferTxASCII + wITxBufferASCII) != 0x00; wITxBufferASCII++)
  {
    byByteToTx = *(byBufferTxASCII + wITxBufferASCII);
    U1TXREG = byByteToTx;
    while (U1STAbits.TRMT);

  }
}
