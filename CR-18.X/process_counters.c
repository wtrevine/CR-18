/*includes*/
#include "main.h"

/*
 * Reseta temporizador
 */
void process_counters_reset(timeout_t * time, uint8_t enable) 
{
  time->count = 0;
  time->enable = enable;
  time->overflow = FALSE;
}
 
/*
 * Processa temporizador
 */
void process_counters(timeout_t * sData, uint8_t ReStart)
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
 * Processa temporizadores
 */
void process_counters_interrupt()
{
  //process_counters(&timeout_usart_receive, FALSE);

}

/*
 * Inicia temporizadores
 */
void process_counters_init()
{
    //timeout_usart_receive.count_max = TIMEOUT_USART_RECEIVE;
    //process_counters_reset(&timeout_usart_absence, TRUE);
}


