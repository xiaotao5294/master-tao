/**
  * @file       led_device.c
  * @author     ����
  * @version    V1.0.1
  * @date       20251203
  * @brief      LED 驱动，基于 STM32 HAL 库
  *
  * <h2><center>&copy;此文件版权归【����】所有.</center></h2>
  */

#include "led_device.h"
#include "string.h"


/// @brief      设备实例
///
/// @note
static stLedDeviceParamTdf s_astLedDevice[LED_DEV_NUM];

/// @brief      静态参数拷贝
///
/// @param      pstInit     : 初始化参数
/// @param      emDevNum    : 设备号
///
/// @note
static void s_vLedStaticParamCopy(stLedStaticParamTdf *pstInit, emLedDevNumTdf emDevNum)
{
    memcpy(&s_astLedDevice[emDevNum].stStaticParam, pstInit, sizeof(stLedStaticParamTdf) / sizeof(uint8_t));
}

/// @brief      获取运行参数指针
///
/// @param      emDevNum    : 设备号
///
/// @note       注意，获取的指针，指向的内容是只读的
const stLedRunningParamTdf *c_pstGetLedRunningParam(emLedDevNumTdf emDevNum)
{
    return &s_astLedDevice[emDevNum].stRunningParam;
}

/// @brief      运行参数设置
///
/// @param      pstInit     : 要设置的运行参数
/// @param      emDevNum    : 设备号
///
/// @note       参数为 0xFFFF 或 0xFFFFFFFF 时，表示不做更改
void vLedSetRunningParam(stLedRunningParamTdf *pstInit, emLedDevNumTdf emDevNum)
{
    if(pstInit->emCurrentStatus != emLedStatus_NotModify)
    {
        s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus = pstInit->emCurrentStatus;
    }
    
    if(pstInit->emCurrentMode != emLedMode_NotModify)
    {
        s_astLedDevice[emDevNum].stRunningParam.emCurrentMode = pstInit->emCurrentMode;
    }
    
    if(pstInit->ulBlinkCurrentCount != 0xFFFFFFFF)
    {
        s_astLedDevice[emDevNum].stRunningParam.ulBlinkCurrentCount = pstInit->ulBlinkCurrentCount;
    }
    
    if(pstInit->ulBlinkOnCountThreshold != 0xFFFFFFFF)
    {
        s_astLedDevice[emDevNum].stRunningParam.ulBlinkOnCountThreshold = pstInit->ulBlinkOnCountThreshold;
    }
    
    if(pstInit->ulBlinkOffCountThreshold != 0xFFFFFFFF)
    {
        s_astLedDevice[emDevNum].stRunningParam.ulBlinkOffCountThreshold = pstInit->ulBlinkOffCountThreshold;
    }
}

/// @brief      LED 刷新输出电平
///
/// @param      emDevNum    : 设备号
///
/// @note       真值表如下
///             -------------------------------------------------------------------------------------------------
///             |       stStaticParam.emOnLevel     |   stRunningParam.emCurrentStatus      |    OutputLevel    |
///             |------------------------------------------------------------------------------------------------
///             |               Low(0)              |               ON(1)                   |       Low(0)      |
///             |               High(1)             |               ON(1)                   |       High(1)     |
///             |               Low(0)              |               OFF(0)                  |       High(1)     |
///             |               High(1)             |               OFF(0)                  |       Low(0)      |
///             |------------------------------------------------------------------------------------------------
///
///             由真值表可知，OutputLevel = !(stStaticParam.emOnLevel ^ stRunningParam.emCurrentStatus);
void vLedRefreshOutputLevel(emLedDevNumTdf emDevNum)
{
    GPIO_PinState stGpioOutputLevel;

    stGpioOutputLevel = (GPIO_PinState)(!(s_astLedDevice[emDevNum].stStaticParam.emOnLevel ^ 
                                          s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus));
    
    HAL_GPIO_WritePin(s_astLedDevice[emDevNum].stStaticParam.pstGpioBase, 
                      s_astLedDevice[emDevNum].stStaticParam.usGpioPin, 
                      stGpioOutputLevel);
}

/// @brief      LED 翻转当前状态
///
/// @param      emDevNum    : 设备号
///
/// @note
void vLedToggleCurrentStatus(emLedDevNumTdf emDevNum)
{
    s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus = (emLedStatusTdf)(!s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus);
    vLedRefreshOutputLevel(emDevNum);
}

/// @brief      LED 执行
///
/// @param      emDevNum    : 设备号
///
/// @note       需要周期调用执行
void vLedExecute(emLedDevNumTdf emDevNum)
{
    switch(s_astLedDevice[emDevNum].stRunningParam.emCurrentMode)
    {
        case emLedMode_Blink:
        {
            s_astLedDevice[emDevNum].stRunningParam.ulBlinkCurrentCount++;
            
            if(s_astLedDevice[emDevNum].stRunningParam.ulBlinkCurrentCount < 
               s_astLedDevice[emDevNum].stRunningParam.ulBlinkOnCountThreshold)
            {
                s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus = emLedStatus_On;
            }
            else if(s_astLedDevice[emDevNum].stRunningParam.ulBlinkCurrentCount < 
                    (s_astLedDevice[emDevNum].stRunningParam.ulBlinkOnCountThreshold + 
                     s_astLedDevice[emDevNum].stRunningParam.ulBlinkOffCountThreshold))
            {
                s_astLedDevice[emDevNum].stRunningParam.emCurrentStatus = emLedStatus_Off;
            }
            else
            {
                s_astLedDevice[emDevNum].stRunningParam.ulBlinkCurrentCount = 0;
            }
        }
        case emLedMode_Static:
        {
            vLedRefreshOutputLevel(emDevNum);
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      设备初始化
///
/// @param      pstInit     : 初始化结构体实例指针
/// @param      emDevNum    : 设备号
///
/// @retval     调用是否成功，详见 emLedFuncErrorCodeTdf 定义
///
/// @note
emLedFuncErrorCodeTdf emLedDeviceInit(stLedStaticParamTdf *pstInit, emLedDevNumTdf emDevNum)
{
    // 1. 拷贝静态参数
    s_vLedStaticParamCopy(pstInit, emDevNum);

    // 2. 返回错误码
    return emLedFuncErrorCode_None;
}
