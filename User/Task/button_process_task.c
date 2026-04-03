/**
  * @file       button_process_task.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      按键处理任务
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "button_process_task.h"
#include "button_operation.h"


/// @brief      按键处理队列
///
/// @note
QueueHandle_t qButtonProcess;

/// @brief      按键处理任务
///
/// @note
void vButtonProcessTask()
{
    uint32_t ulButtonValueCurrent;
    uint32_t ulButtonValueLast;
    
    qButtonProcess = xQueueCreate(BUTTON_PROCESS_QUEUE_SIZE, sizeof(uint32_t));
    
    while(1)
    {
        vTaskDelay(BUTTON_PROCESS_TASK_PERIOD_MS);
        
        if(xQueueReceive(qButtonProcess, &ulButtonValueCurrent, 0))
        {
            if(ulButtonValueLast != ulButtonValueCurrent)
            {
                vButtonValueProcess(ulButtonValueCurrent & (ulButtonValueCurrent ^ ulButtonValueLast));
                
                ulButtonValueLast = ulButtonValueCurrent;
            }
        }
    }
}

