/**
  * @file       oled_operation.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      OLED 操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */
 
#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "oled_device.h"
#include "oled_operation.h"

#include "ui_frame_main.h"
#include "ui_frame_dht11.h"
#include "ui_frame_led_ctrl.h"

/// @brief      UI 对象实例
///
/// @note
static stUiObjectTdf s_stUiObject;

/// @brief      页面实例
///
/// @note
static stUiFrameTdf *s_apstFrame[UI_FRAME_NUM];


/// @brief      UI 初始化
///
/// @note
void vUiInit()
{
    s_apstFrame[emUiFrame_Main] = pstGetMainFrame();
    vMainFrameInit();
    
    s_apstFrame[emUiFrame_LedCtrl] = pstGetLedCtrlFrame();
    vLedCtrlFrameInit();
    
    s_apstFrame[emUiFrame_Dht11] = pstGetDht11Frame();
    vDht11FrameInit();
    
    s_stUiObject.p2stFrame = s_apstFrame;
    s_stUiObject.ulCurrentFrameIndex = emUiFrame_Main;
}

/// @brief      OLED 初始化
///
/// @note
void vOledInit()
{
    stOledStaticParamTdf stParam;
    
    stParam.pstSclGpioBase  = GPIOB;
    stParam.usSclGpioPin    = GPIO_PIN_6;
    stParam.pstSdaGpioBase  = GPIOB;
    stParam.usSdaGpioPin    = GPIO_PIN_7;
    vOledDeviceInit(&stParam, OLED);
}

/// @brief      OLED 刷新页面
///
/// @note
void vOledRefreshCurrentFrame()
{
    vUpdateUiObjectToFrameBuffer(&s_stUiObject);
    vOledRefreshFromBuffer(c_pucUiGetFrameBuffer(), OLED);
}

/// @brief      OLED 执行当前页面回调函数
///
/// @note
void vOledExecuteCurrentFrameCallback(void *p)
{
    vUiFrameCallbackExecute(&s_stUiObject, p);
}

/// @brief      UI 设置当前页面
///
/// @note       相当于执行页面切换
void vUiSetCurrentFrame(uint32_t ulFrameIndex)
{
    s_stUiObject.ulCurrentFrameIndex = ulFrameIndex;
}

/// @brief      UI 清屏
///
/// @note
void vUiClearFrame(emUiPixelShowModeTdf emMode)
{
    vClearFrameBuffer(emMode);
}


