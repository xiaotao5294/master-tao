/**
  * @file       dht11_device.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      DHT11 驱动，基于 STM32 HAL 库
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 


#ifndef _DHT11_DEVICE_H_
#define _DHT11_DEVICE_H_

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "led_device.h"

                                                      
/// @brief      设备号定义
///
/// @note
typedef enum 
{
    emDht11DevNum0                                      = 0,                            // 0  设备 0
    emDht11DevNum1,
}
emDht11DevNumTdf;

/// @brief      调用函数错误定义
///
/// @note
typedef enum 
{
    emDht11CallFuncError_None                           = 0,                            // 0，无错误
}
emDht11CallFuncErrorTdf;


/// @brief      设备静态参数结构定义
///
/// @note
typedef struct
{
    GPIO_TypeDef                                        *pstGpioBase;                   // 使用的 GPIOx
    uint32_t                                            ulGpioPin;                      // 使用的 GPIO_PIN_x
}
stDht11StaticParamDevTdf;

/// @brief      设备运行参数结构定义
///
/// @note
typedef struct
{
    int8_t                                              acHumidity[2];                  // 当前湿度，整数部分 + 小数部分
    int8_t                                              acTemperature[2];               // 当前温度，整数部分 + 小数部分
}
stDht11RunningParamDevTdf;

/// @brief      设备结构定义
///
/// @note
typedef struct
{
    stDht11StaticParamDevTdf                            stStaticParam;                  // 静态参数
    stDht11RunningParamDevTdf                           stRunningParam;                 // 运行参数
}
stDht11DeviceParamTdf;

const stDht11RunningParamDevTdf *c_pstDht11GetRunningParam(emDht11DevNumTdf emDevNum);
uint8_t ucDht11ReadData(emDht11DevNumTdf emDevNum);
emDht11CallFuncErrorTdf emDht11DevInit(stDht11StaticParamDevTdf *pstInit, emDht11DevNumTdf emDevNum);

#endif
