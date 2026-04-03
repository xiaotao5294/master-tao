/**
  * @file       dht11_operation.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      DHT11 操作
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#ifndef _DHT11_OPERATION_H_
#define _DHT11_OPERATION_H_


#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "project_config.h"

#define DHT11_FILTER_BUFFER_SIZE                        20                              // 滤波缓冲区大小
#define DHT11_AVERAGE_VALUE_NUM                         12                              // 均值计算有效数据个数

/// @brief      运行参数结构定义
///
/// @note
typedef struct
{
    int16_t sCurrentTemperatureValueGain10;
    int16_t sCurrentHumidityValueGain10;
    int16_t asTemperatureOriginalData[DHT11_FILTER_BUFFER_SIZE];
    int16_t asTemperatureAverageData[DHT11_FILTER_BUFFER_SIZE];
    int16_t asHumidityOriginalData[DHT11_FILTER_BUFFER_SIZE];
    int16_t asHumidityAverageData[DHT11_FILTER_BUFFER_SIZE];
}
stDht11OperationRunningParamTdf;

const stDht11OperationRunningParamTdf *c_pstGetDht11OperationRunningParam(void);
void vDht11Init(void);
void vDht11RefreshData(void);

#endif
