/** 
  * @file       cmd_interpreter_operation.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251216
  * @brief      命令解释器操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _CMD_INTERPRETER_OPERATION_H_
#define _CMD_INTERPRETER_OPERATION_H_

#include "stdint.h"

#define LED_SEND_BLOCK_COUNT_MAX                        5                               // LED 发送队列阻塞最大计数

void vCmdInterpreterInit(void);
void vCmdInterpreterOperationExecute(uint8_t *pucCmdLine);
void vCmdInterpreterExecuteCallback_LedOn(uint8_t **p2ucString);
void vCmdInterpreterExecuteCallback_LedOff(uint8_t **p2ucString);
void vCmdInterpreterExecuteCallback_LedToggle(uint8_t **p2ucString);
void vCmdInterpreterExecuteCallback_LedBlink_ParamNum2(uint8_t **p2ucString);
void vCmdInterpreterExecuteCallback_LedBlink_ParamNum3(uint8_t **p2ucString);

#endif
