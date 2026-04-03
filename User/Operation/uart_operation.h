/**
  * @file       uart_operation.h
  * @author     ÌÒ×Ó
  * @version    V1.0.1
  * @date       20251216
  * @brief      UART æ“ä½œ
  * 
  * <h2><center>&copy;æ­¤æ–‡ä»¶ç‰ˆæƒå½’ã€ÌÒ×Óã€‘æ‰€æœ‰.</center></h2>
  */

#ifndef _UART_OPERATION_H_
#define _UART_OPERATION_H_

#include "stdint.h"



void vUartInit(void);
void vUart1InterruptCallback(void);
void vUartSendAllBufferData(void);
void vUartGetCmdLine(void);

#endif
