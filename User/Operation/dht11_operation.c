/**
  * @file       dht11_operation.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      DHT11 操作
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "string.h"
#include "dht11_operation.h"
#include "dht11_device.h"
#include "ui_frame_dht11.h"

/// @brief      DHT11 操作运行参数
///
/// @note
static stDht11OperationRunningParamTdf s_stRunningParam;

/// @brief      获取运行参数
///
/// @note
const stDht11OperationRunningParamTdf *c_pstGetDht11OperationRunningParam()
{
    return &s_stRunningParam;
}

/// @brief      DHT11 初始化
///
/// @note
void vDht11Init()
{
    stDht11StaticParamDevTdf stInit;
    
    stInit.pstGpioBase  = GPIOB;
    stInit.ulGpioPin    = GPIO_PIN_12;
    emDht11DevInit(&stInit, DHT11);
}

/// @brief      获取数组除最大值和最小值外的均值
///
/// @return     计算完成的均值
///
/// @note
int16_t sGetAverageWithoutMaxAndMin(int16_t *psSrc, int16_t *psAvr, uint16_t usSize, uint16_t usAvrNum)
{
    uint16_t i, j;
    int16_t temp;
    int16_t sStartIndex = (usSize - usAvrNum) / 2;
    int32_t avr = 0;
    
    // 1. 存原始数据到计算数组内
    memcpy(psAvr, psSrc, usSize * sizeof(uint16_t) / sizeof(uint8_t));
    
    // 2. 冒泡排序
    for(i = 0; i < usSize; i++)
    {
        for(j = i + 1; j < usSize; j++)
        {
            if(psAvr[i] < psAvr[j])
            {
                temp = psAvr[i];
                psAvr[i] = psAvr[j];
                psAvr[j] = temp;
            }
        }
    }
    
    // 3. 计算均值
    for(i = 0; i < usAvrNum; i++)
    {
        avr += psAvr[i + sStartIndex];
    }
    
    avr /= usAvrNum;
    
    return avr;
}

/// @brief      DHT11 刷新数据
///
/// @note
void vDht11RefreshData()
{
    static uint8_t i = 0;
    
    ucDht11ReadData(DHT11);
    
    s_stRunningParam.asHumidityOriginalData[i]        = c_pstDht11GetRunningParam(DHT11)->acHumidity[0] * 10 + 
                                                        c_pstDht11GetRunningParam(DHT11)->acHumidity[1];
    s_stRunningParam.asTemperatureOriginalData[i]     = c_pstDht11GetRunningParam(DHT11)->acTemperature[0] * 10 + 
                                                        c_pstDht11GetRunningParam(DHT11)->acTemperature[1];
    
    s_stRunningParam.sCurrentHumidityValueGain10      = sGetAverageWithoutMaxAndMin(s_stRunningParam.asHumidityOriginalData, 
                                                                                    s_stRunningParam.asHumidityAverageData,
                                                                                    DHT11_FILTER_BUFFER_SIZE, 
                                                                                    DHT11_AVERAGE_VALUE_NUM);
    s_stRunningParam.sCurrentTemperatureValueGain10   = sGetAverageWithoutMaxAndMin(s_stRunningParam.asTemperatureOriginalData, 
                                                                                    s_stRunningParam.asTemperatureAverageData,
                                                                                    DHT11_FILTER_BUFFER_SIZE, 
                                                                                    DHT11_AVERAGE_VALUE_NUM);

    i++;
    if(i >= DHT11_FILTER_BUFFER_SIZE)
    {
        i = 0;
    }
    
    vDht11FrameUpdateTemperatureValue(s_stRunningParam.sCurrentTemperatureValueGain10);
    vDht11FrameUpdateHumidityValue(s_stRunningParam.sCurrentHumidityValueGain10);
}
