/**
  * @file       ui_frame_led_ctrl.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      UI LED CTRL 页面
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _UI_FRAME_LED_CTRL_H_
#define _UI_FRAME_LED_CTRL_H_

#include "stdint.h"
#include "ui_core.h"


#define LED_CTRL_SEND_BLOCK_COUNT_MAX                   2                               // LED CTRL 页面发送队列最大等待时间，单位 ms

/// @brief      LED CTRL 页面状态定义
typedef enum
{
    emLedCtrlStatus_NotModify                           = 0,                            // 0  不改变 LED 状态
    emLedCtrlStatus_On                                  = 1,                            // 1  ON
    emLedCtrlStatus_Off                                 = 2,                            // 2  OFF
}
emLedCtrlStatusTdf;

void vLedCtrlFrameInit(void);
stUiFrameTdf *pstGetLedCtrlFrame(void);

#endif
