/**
  * @file       oled_task.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      OLED 任务
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */
 
#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "oled_task.h"
#include "oled_operation.h"

/// @brief      UI 参数设置队列
///
/// @note
QueueHandle_t qUiParamSet;

/// @brief      OLED 任务
///
/// @note
void vOledTask()
{
    uint32_t ulUiParam;
    
    vUiInit();
    vOledInit();
    
    // 2. 初始化 LED 参数设置队列
    qUiParamSet = xQueueCreate(UI_PARAM_SET_QUEUE_SIZE, sizeof(uint32_t));
    
    while(1)
    {
        vTaskDelay(OLED_TASK_PERIOD_MS);
        
        if(xQueueReceive(qUiParamSet, &ulUiParam, 0))
        {
            vUiClearFrame(emUiPixelShowMode_Positive);
            vUiSetCurrentFrame(ulUiParam);
        }
        
        vOledRefreshCurrentFrame();
    }
}
