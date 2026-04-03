/**
  * @file       dht11_task.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      DHT11 任务
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "dht11_task.h"
#include "dht11_operation.h"

/// @brief      DHT11 数据采集任务
///
/// @note       由于 DHT11 每次采集数据前都需要延时 20ms，
///             混在循环任务中容易对其他行为产生阻塞，
///             故单独开一个任务采集数据
void vDht11Task()
{
    vDht11Init();
    
    while(1)
    {
        osDelay(DHT11_TASK_PERIOD_MS);
        
        vDht11RefreshData();
    }
}

