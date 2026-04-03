/**
  * @file       ui_frame_led_ctrl.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      UI LED CTRL 页面
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "freertos.h"
#include "queue.h"

#include "ui_frame_led_ctrl.h"
#include "project_config.h"
#include "oled_operation.h"
#include "led_operation.h"
#include "button_device.h"

extern QueueHandle_t qUiParamSet;
extern QueueHandle_t qLedParamSet;

/// @brief      LED 参数设置缓冲区
///
/// @note       用于队列传递 LED 参数
static stLedOperationQueueParamTdf s_stLedParamSetBuffer;

/// @brief      显示信息
///
/// @note
static uint8_t s_aucTextBoxData_LedDevString[]      = "LED";
static uint8_t s_aucTextBoxData_LedDevValue[]       = "0 ";
static uint8_t s_aucTextBoxData_LedOnString[]       = "ON";
static uint8_t s_aucTextBoxData_LedOffString[]      = "OFF";

/// @brief      【LED 字符】文本框实例
///
/// @note
static stUiTextBoxTdf s_stLedCtrlFrameTextBox_LedDevString = 
{
    .x                          = 45,
    .y                          = 0,
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_LedDevString,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【LED 值】文本框实例
///
/// @note
static stUiTextBoxTdf s_stLedCtrlFrameTextBox_LedDevValue = 
{
    .x                          = 64,
    .y                          = 0,
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_LedDevValue,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【ON】文本框实例
///
/// @note
static stUiTextBoxTdf s_stLedCtrlFrameTextBox_LedOnString = 
{
    .x                          = 24,
    .y                          = 2 * (emUiFontSize_12x6 + 2),
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_LedOnString,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【OFF】文本框实例
///
/// @note
static stUiTextBoxTdf s_stLedCtrlFrameTextBox_LedOffString = 
{
    .x                          = 88,
    .y                          = 2 * (emUiFontSize_12x6 + 2),
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_LedOffString,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【LED 字符】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stLedCtrlFrameWidgetNode_LedDevString = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stLedCtrlFrameTextBox_LedDevString,
    .pstNextNode                = NULL,
};

/// @brief      【LED 值】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stLedCtrlFrameWidgetNode_LedDevValue = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stLedCtrlFrameTextBox_LedDevValue,
    .pstNextNode                = NULL,
};

/// @brief      【ON 字符】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stLedCtrlFrameWidgetNode_LedOnString = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stLedCtrlFrameTextBox_LedOnString,
    .pstNextNode                = NULL,
};

/// @brief      【OFF 字符】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stLedCtrlFrameWidgetNode_LedOffString = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stLedCtrlFrameTextBox_LedOffString,
    .pstNextNode                = NULL,
};

/// @brief      LED CTRL 页面实例
///
/// @note
static stUiFrameTdf s_stLedCtrlFrame;

/// @brief      申请执行【LED ON】
///
/// @param      ucString    : 命令行首地址
///
/// @note
void vLedCtrlRequestLedOn(uint8_t ucLedDevNum)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ucLedDevNum;
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Static;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_On;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = 0xFFFFFFFF;
    
    xQueueSend(qLedParamSet, &p, LED_CTRL_SEND_BLOCK_COUNT_MAX);
}

/// @brief      申请执行【LED OFF】
///
/// @param      ucString    : 命令行首地址
///
/// @note
void vLedCtrlRequestLedOff(uint8_t ucLedDevNum)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ucLedDevNum;
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Static;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_Off;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = 0xFFFFFFFF;
    
    xQueueSend(qLedParamSet, &p, LED_CTRL_SEND_BLOCK_COUNT_MAX);
}

/// @brief      LED CTRL 页面【UP】键执行
///
/// @param      pucLedDevNum    : 设备号指针
/// @param      pemStatus       : 状态
///
/// @note
static void s_vLedCtrlFrameButtonUpExecute(uint8_t *pucLedDevNum, emLedCtrlStatusTdf *pemStatus)
{
    (*pucLedDevNum)++;
    if(*pucLedDevNum >= LED_DEV_NUM)
    {
        *pucLedDevNum = 0;
    }
    s_aucTextBoxData_LedDevValue[0] = '0' + *pucLedDevNum;
    
    *pemStatus = emLedCtrlStatus_NotModify;
    s_stLedCtrlFrameTextBox_LedOnString.emMode  = emUiPixelShowMode_Positive;
    s_stLedCtrlFrameTextBox_LedOffString.emMode = emUiPixelShowMode_Positive;
}

/// @brief      LED CTRL 页面【DOWN】键执行
///
/// @param      pucLedDevNum    : 设备号指针
/// @param      pemStatus       : 状态
///
/// @note
static void s_vLedCtrlFrameButtonDownExecute(uint8_t *pucLedDevNum, emLedCtrlStatusTdf *pemStatus)
{
    (*pucLedDevNum)--;
    if(*pucLedDevNum >= LED_DEV_NUM)
    {
        *pucLedDevNum = LED_DEV_NUM - 1;
    }
    s_aucTextBoxData_LedDevValue[0] = '0' + *pucLedDevNum;
    
    *pemStatus = emLedCtrlStatus_NotModify;
    s_stLedCtrlFrameTextBox_LedOnString.emMode  = emUiPixelShowMode_Positive;
    s_stLedCtrlFrameTextBox_LedOffString.emMode = emUiPixelShowMode_Positive;
}

/// @brief      LED CTRL 页面【ENTER】键执行
///
/// @param      pucLedDevNum    : 设备号指针
/// @param      pemStatus       : 状态
///
/// @note
static void s_vLedCtrlFrameButtonEnterExecute(uint8_t *pucLedDevNum, emLedCtrlStatusTdf *pemStatus)
{
    if(*pemStatus == emLedCtrlStatus_On)
    {
        *pemStatus = emLedCtrlStatus_Off; 
        s_stLedCtrlFrameTextBox_LedOnString.emMode  = emUiPixelShowMode_Positive;
        s_stLedCtrlFrameTextBox_LedOffString.emMode = emUiPixelShowMode_Negative;
        vLedCtrlRequestLedOff(*pucLedDevNum);
    }
    else
    {
        *pemStatus = emLedCtrlStatus_On;
        s_stLedCtrlFrameTextBox_LedOnString.emMode  = emUiPixelShowMode_Negative;
        s_stLedCtrlFrameTextBox_LedOffString.emMode = emUiPixelShowMode_Positive;
        vLedCtrlRequestLedOn(*pucLedDevNum);
    }
}

/// @brief      LED CTRL 页面【CANCEL】键执行
///
/// @note
static void s_vLedCtrlFrameButtonCancelExecute(emLedCtrlStatusTdf *pemStatus)
{
    uint32_t ulSendParam = 0;
    
    *pemStatus = emLedCtrlStatus_NotModify;
    s_stLedCtrlFrameTextBox_LedOnString.emMode  = emUiPixelShowMode_Positive;
    s_stLedCtrlFrameTextBox_LedOffString.emMode = emUiPixelShowMode_Positive;

    xQueueSend(qUiParamSet, &ulSendParam, 10);
}

/// @brief      LED CTRL 页面回调函数
///
/// @param      p       : 参数指针
///
/// @note
static void s_vLedCtrlFrameCallback(void *p)
{
    uint8_t i;
    uint32_t ulButtonValue = *((uint32_t *)p);
    uint32_t ulButtonValueMask = 0x00000001;
    static uint8_t s_ucLedDevNum;
    static emLedCtrlStatusTdf s_emStatus;
    
    for(i = 0; i < BUTTON_DEV_NUM; i++)
    {
        if((ulButtonValue & ulButtonValueMask) != 0)
        {
            break;
        }
        
        ulButtonValueMask <<= 1;
    }
    
    switch(i)
    {
        case BUTTON_UP:
        {
            s_vLedCtrlFrameButtonUpExecute(&s_ucLedDevNum, &s_emStatus);
            break;
        }
        case BUTTON_DOWN:
        {
            s_vLedCtrlFrameButtonDownExecute(&s_ucLedDevNum, &s_emStatus);
            break;
        }
        case BUTTON_ENTER:
        {
            s_vLedCtrlFrameButtonEnterExecute(&s_ucLedDevNum, &s_emStatus);
            break;
        }
        case BUTTON_CANCEL:
        {
            s_vLedCtrlFrameButtonCancelExecute(&s_emStatus);
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      LED CTRL 页面初始化
///
/// @note
void vLedCtrlFrameInit()
{
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stLedCtrlFrame, &s_stLedCtrlFrameWidgetNode_LedDevString);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stLedCtrlFrame, &s_stLedCtrlFrameWidgetNode_LedDevValue);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stLedCtrlFrame, &s_stLedCtrlFrameWidgetNode_LedOnString);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stLedCtrlFrame, &s_stLedCtrlFrameWidgetNode_LedOffString);
    
    s_stLedCtrlFrame.pvCallback = s_vLedCtrlFrameCallback;
}

/// @brief      获取 LED CTRL 页面
///
/// @note
stUiFrameTdf *pstGetLedCtrlFrame()
{
    return &s_stLedCtrlFrame;
}
