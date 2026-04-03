/**
  * @file       led_operation.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      LED 操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _LED_OPERATION_H_
#define _LED_OPERATION_H_

#include "stdint.h"
#include "led_device.h"

/// @brief      LED 操作队列参数结构定义
///
/// @note       用于队列传参
typedef struct
{
    emLedDevNumTdf                                      emDevNum;                       // 设备号
    stLedRunningParamTdf                                stParam;                        // 要传递的参数
}
stLedOperationQueueParamTdf;

void vLedInit(void);
void vLedOperationExecute(void);
void vLedOperationParamUpdate(stLedOperationQueueParamTdf *pstParam);

#endif
