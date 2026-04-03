/**
  * @file       button_operation.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      独立按键操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _BUTTON_OPERATION_H_
#define _BUTTON_OPERATION_H_

#include "stdint.h"

#define BUTTON_OPERATION_SEND_BLOCK_MAX_COUNT           2                               // 按键操作发送阻塞最大计数

void vButtonInit(void);
void vButtonScan(void);
void vButtonValueProcess(uint32_t ulButtonValue);

#endif
