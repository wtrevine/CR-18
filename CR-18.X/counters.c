/*includes*/
#include "main.h"

/* temporizaores*/
timeout_t timeout_uart_receive;

/*
 * Reseta temporizador
 */
void counters_reset(timeout_t * time, uint8_t enable) 
{
  time->count = 0;
  time->enable = enable;
  time->overflow = FALSE;
}
 
/*
 * Processa temporizador
 */
void counters_proccess(timeout_t * sData, uint8_t ReStart)
{
    if(sData->enable == TRUE)
    {
        if((++sData->count) >= sData->count_max)
        {
            sData->count = 0;
            sData->overflow = TRUE;
            sData->enable = ReStart;
        }
    }
    else
        sData->count = 0;
}

/*
 * Chama processos
 *  */
void counters_interrupt()
{
  counters_proccess(&timeout_uart_receive, FALSE);

}

/*
 * Inicia temporizadores
 */
void counters_init()
{
    timeout_uart_receive.count_max = TIMEOUT_UART_RECEIVE;

}


