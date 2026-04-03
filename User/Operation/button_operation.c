/**
  * @file       button_operation.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      独立按键操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "project_config.h"
#include "freertos.h"
#include "queue.h"

#include "button_device.h"
#include "button_operation.h"

#include "oled_operation.h"

extern QueueHandle_t qButtonProcess;


/// @brief      独立按键初始化
///
/// @note
void vButtonInit()
{
    stButtonStaticParamTdf stInit;
    
    stInit.emActiveLevel    = emButtonActiveLevel_Low;
    stInit.pstGpioBase      = GPIOA;
    stInit.usGpioPin        = GPIO_PIN_1;
    stInit.usDebounceCount  = 1;
    emButtonDeviceInit(&stInit, BUTTON_CANCEL);
    
    stInit.emActiveLevel    = emButtonActiveLevel_Low;
    stInit.pstGpioBase      = GPIOA;
    stInit.usGpioPin        = GPIO_PIN_0;
    stInit.usDebounceCount  = 2;
    emButtonDeviceInit(&stInit, BUTTON_ENTER);
    
    stInit.emActiveLevel    = emButtonActiveLevel_Low;
    stInit.pstGpioBase      = GPIOC;
    stInit.usGpioPin        = GPIO_PIN_15;
    stInit.usDebounceCount  = 3;
    emButtonDeviceInit(&stInit, BUTTON_DOWN);
    
    stInit.emActiveLevel    = emButtonActiveLevel_Low;
    stInit.pstGpioBase      = GPIOC;
    stInit.usGpioPin        = GPIO_PIN_14;
    stInit.usDebounceCount  = 4;
    emButtonDeviceInit(&stInit, BUTTON_UP);
}

/// @brief      独立按键扫描
///
/// @note
void vButtonScan()
{
    static uint32_t s_ulButtonValueCurrent = 0;
    
    // 1. 获取当前所有键值
    s_ulButtonValueCurrent = ulButtonGetEvents(BUTTON_UP, BUTTON_DEV_NUM);
    
    // 2. 将键值通过按键处理队列发送出去
    xQueueSend(qButtonProcess, &s_ulButtonValueCurrent, BUTTON_OPERATION_SEND_BLOCK_MAX_COUNT);
}

/// @brief      键值处理
///
/// @note
void vButtonValueProcess(uint32_t ulButtonValue)
{
    static uint32_t s_ulButtonValue;
    
    s_ulButtonValue = ulButtonValue;
    
    vOledExecuteCurrentFrameCallback(&s_ulButtonValue);
}
