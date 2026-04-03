/**
  * @file       led_operation.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      LED 操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */


#include "freertos.h"
#include "led_operation.h"
#include "led_device.h"
#include "project_config.h"
#include "queue.h"

extern QueueHandle_t qLedParamSet;


/// @brief      LED 初始化
///
/// @note
void vLedInit()
{
    stLedStaticParamTdf     stStaticInit;
    stLedRunningParamTdf    stRunningInit;
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 500;
    stRunningInit.ulBlinkOffCountThreshold  = 500;
    vLedSetRunningParam(&stRunningInit, LED_STATUS);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOC;
    stStaticInit.usGpioPin                  = GPIO_PIN_13;
    emLedDeviceInit(&stStaticInit, LED_STATUS);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED8);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_2;
    emLedDeviceInit(&stStaticInit, LED8);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED7);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_3;
    emLedDeviceInit(&stStaticInit, LED7);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED6);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_4;
    emLedDeviceInit(&stStaticInit, LED6);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED5);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_5;
    emLedDeviceInit(&stStaticInit, LED5);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED4);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_6;
    emLedDeviceInit(&stStaticInit, LED4);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED3);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOA;
    stStaticInit.usGpioPin                  = GPIO_PIN_7;
    emLedDeviceInit(&stStaticInit, LED3);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED2);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOB;
    stStaticInit.usGpioPin                  = GPIO_PIN_0;
    emLedDeviceInit(&stStaticInit, LED2);
    
    stRunningInit.emCurrentMode             = emLedMode_Blink;
    stRunningInit.emCurrentStatus           = emLedStatus_On;
    stRunningInit.ulBlinkCurrentCount       = 0;
    stRunningInit.ulBlinkOnCountThreshold   = 1000;
    stRunningInit.ulBlinkOffCountThreshold  = 1000;
    vLedSetRunningParam(&stRunningInit, LED1);
    
    stStaticInit.emOnLevel                  = emLedOnLevel_Low;
    stStaticInit.pstGpioBase                = GPIOB;
    stStaticInit.usGpioPin                  = GPIO_PIN_1;
    emLedDeviceInit(&stStaticInit, LED1);
}

/// @brief      LED 操作执行
///
/// @note       需要在任务循环中执行
void vLedOperationExecute()
{
    uint8_t i;
    
    for(i = 0; i < LED_DEV_NUM; i++)
    {
        vLedExecute((emLedDevNumTdf)i);
    }
}

/// @brief      LED 更新操作参数
///
/// @note       需要在任务循环中执行
void vLedOperationParamUpdate(stLedOperationQueueParamTdf *pstParam)
{
    vLedSetRunningParam(&pstParam->stParam, pstParam->emDevNum);
}
