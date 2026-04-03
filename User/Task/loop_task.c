/**
  * @file       loop_task.c
  * @author     ����
  * @version    V1.0.1
  * @date       20251204
  * @brief      循环任务
  * 
  * <h2><center>&copy;此文件版权归【����】所有.</center></h2>
  */


#include "freertos.h"
#include "task.h"

#include "button_operation.h"

#include "loop_task.h"

/// @brief      循环任务
///
/// @note
void vLoopTask()
{
    vButtonInit();
    
    while(1)
    {
        vTaskDelay(LOOP_TASK_PERIOD_MS);
        
        vButtonScan();
    }
}
