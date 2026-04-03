/**
  * @file       oled_operation.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      OLED 操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _OLED_OPERATION_H_
#define _OLED_OPERATION_H_

#include "stdint.h"
#include "project_config.h"
#include "ui_core.h"


/// @brief      UI 页面序号枚举定义
///
/// @note
typedef enum
{
    emUiFrame_Main                                      = 0,
    emUiFrame_LedCtrl                                   = 1,
    emUiFrame_Dht11                                     = 2,
}
emUiFrameTdf;



void vUiInit(void);
void vOledInit(void);
void vOledRefreshCurrentFrame(void);
void vOledExecuteCurrentFrameCallback(void *p);
void vUiSetCurrentFrame(uint32_t ulFrameIndex);
void vUiClearFrame(emUiPixelShowModeTdf emMode);

#endif
