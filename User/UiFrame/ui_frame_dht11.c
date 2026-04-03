/**
  * @file       ui_frame_dht11.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      UI DHT11 页面
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "freertos.h"
#include "queue.h"

#include "ui_frame_dht11.h"
#include "project_config.h"
#include "oled_operation.h"
#include "button_device.h"

extern QueueHandle_t qUiParamSet;

/// @brief      显示信息
///
/// @note
static uint8_t s_aucTextBoxData_TemperatureString[]     = "Temperature:";
static uint8_t s_aucTextBoxData_TemperatureValue[]      = "---.- C";
static uint8_t s_aucTextBoxData_HumidityString[]        = "Humidity   :";
static uint8_t s_aucTextBoxData_HumidityValue[]         = "---.- %";

/// @brief      【温度字符】文本框实例
///
/// @note
static stUiTextBoxTdf s_stDht11FrameTextBox_TemperatureString = 
{
    .x                          = 0,
    .y                          = 0,
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_TemperatureString,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【温度值】文本框实例
///
/// @note
static stUiTextBoxTdf s_stDht11FrameTextBox_TemperatureValue = 
{
    .x                          = emUiFontSize_12x6 / 2 * sizeof(s_aucTextBoxData_TemperatureString),
    .y                          = 0,
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_TemperatureValue,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【湿度字符】文本框实例
///
/// @note
static stUiTextBoxTdf s_stDht11FrameTextBox_HumidityString = 
{
    .x                          = 0,
    .y                          = 2 * (emUiFontSize_12x6 + 2),
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_HumidityString,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【湿度值】文本框实例
///
/// @note
static stUiTextBoxTdf s_stDht11FrameTextBox_HumidityValue = 
{
    .x                          = emUiFontSize_12x6 / 2 * sizeof(s_aucTextBoxData_HumidityString),
    .y                          = 2 * (emUiFontSize_12x6 + 2),
    .emSize                     = emUiFontSize_12x6,
    .pucData                    = s_aucTextBoxData_HumidityValue,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      【温度字符】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stDht11FrameWidgetNode_TemperatureString = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stDht11FrameTextBox_TemperatureString,
    .pstNextNode                = NULL,
};

/// @brief      【温度值】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stDht11FrameWidgetNode_TemperatureValue = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stDht11FrameTextBox_TemperatureValue,
    .pstNextNode                = NULL,
};

/// @brief      【湿度字符】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stDht11FrameWidgetNode_HumidityString = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stDht11FrameTextBox_HumidityString,
    .pstNextNode                = NULL,
};

/// @brief      【湿度值】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stDht11FrameWidgetNode_HumidityValue = 
{
    .emType                     = emUiWidgetType_TextBox,
    .pvWidget                   = &s_stDht11FrameTextBox_HumidityValue,
    .pstNextNode                = NULL,
};

/// @brief      DHT11 页面实例
///
/// @note
static stUiFrameTdf s_stDht11Frame;



/// @brief      DHT11 页面【ENTER】键执行
///
/// @note
static void s_vDht11FrameButtonEnterExecute()
{
    uint32_t ulSendParam = 0;

    xQueueSend(qUiParamSet, &ulSendParam, 10);
      
}

/// @brief      DHT11 页面回调函数
///
/// @note
static void s_vDht11FrameCallback(void *p)
{
    uint8_t i;
    uint32_t ulButtonValue = *((uint32_t *)p);
    uint32_t ulButtonValueMask = 0x00000001;
    
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
        case BUTTON_CANCEL:
        {
            s_vDht11FrameButtonEnterExecute();
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      DHT11 页面初始化
///
/// @note
void vDht11FrameInit()
{
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stDht11Frame, &s_stDht11FrameWidgetNode_TemperatureString);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stDht11Frame, &s_stDht11FrameWidgetNode_TemperatureValue);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stDht11Frame, &s_stDht11FrameWidgetNode_HumidityString);
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stDht11Frame, &s_stDht11FrameWidgetNode_HumidityValue);
    
    s_stDht11Frame.pvCallback = s_vDht11FrameCallback;
}

/// @brief      获取 DHT11 页面
///
/// @note
stUiFrameTdf *pstGetDht11Frame()
{
    return &s_stDht11Frame;
}

/// @brief      DHT11 页面更新【温度值】显示
///
/// @note
void vDht11FrameUpdateTemperatureValue(int16_t TemperatureValue)
{
    s_aucTextBoxData_TemperatureValue[4] = TemperatureValue % 10 + '0';
    s_aucTextBoxData_TemperatureValue[2] = TemperatureValue / 10 % 10 + '0';
    
    if((TemperatureValue / 100) != 0)
    {
        s_aucTextBoxData_TemperatureValue[1] = TemperatureValue / 100 % 10 + '0';
    }
    else
    {
        s_aucTextBoxData_TemperatureValue[1] = ' ';
    }
    
    if(TemperatureValue < 0)
    {
        s_aucTextBoxData_TemperatureValue[0] = '-';
    }
    else
    {
        s_aucTextBoxData_TemperatureValue[0] = ' ';
    }
}

/// @brief      DHT11 页面更新【湿度值】显示
///
/// @note
void vDht11FrameUpdateHumidityValue(int16_t HumidityValue)
{
    s_aucTextBoxData_HumidityValue[4] = HumidityValue % 10 + '0';
    s_aucTextBoxData_HumidityValue[2] = HumidityValue / 10 % 10 + '0';
    
    if((HumidityValue / 100) != 0)
    {
        s_aucTextBoxData_HumidityValue[1] = HumidityValue / 100 % 10 + '0';
    }
    else
    {
        s_aucTextBoxData_HumidityValue[1] = ' ';
    }
    
    if(HumidityValue < 0)
    {
        s_aucTextBoxData_HumidityValue[0] = '-';
    }
    else
    {
        s_aucTextBoxData_HumidityValue[0] = ' ';
    }
}
