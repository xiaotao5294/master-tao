/**
  * @file       led_task.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      LED 任务
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "led_task.h"
#include "freertos.h"
#include "task.h"

#include "led_operation.h"
#include "queue.h"

/// @brief      LED 参数设置队列
///
/// @note
QueueHandle_t qLedParamSet;

/// @brief      LED 任务
///
/// @note
void vLedTask()
{
    stLedOperationQueueParamTdf *pstParam;
    
    // 1. 初始化 LED
    vLedInit();
    
    // 2. 初始化 LED 参数设置队列
    qLedParamSet = xQueueCreate(LED_PARAM_SET_QUEUE_SIZE, sizeof(stLedOperationQueueParamTdf *));
    
    // 3. 开始循环任务
    while(1)
    {
        vTaskDelay(LED_TASK_PERIOD_MS);
        
        if(xQueueReceive(qLedParamSet, &pstParam, 0))
        {
            vLedOperationParamUpdate(pstParam);
        }
        vLedOperationExecute();
    }
}
