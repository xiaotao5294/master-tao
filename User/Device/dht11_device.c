/**
  * @file       dht11_device.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      DHT11 驱动，基于 STM32 HAL 库
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "dht11_device.h"
#include "string.h"

/// @brief      设备结构实例
///
/// @note
static stDht11DeviceParamTdf s_stDht11Device[DHT11_DEVICE_NUM];

/// @brief      延时 us
///
/// @note       仅在此文件中有效
static void s_vDelayUs(uint32_t n)
{
    uint32_t volatile i;
    
    for(i = 0; i < n; i++)
    {
        ;
    }
}

/// @brief      获取只读静态参数
///
/// @note       获取的指针，其指向的内容是只读的
const stDht11StaticParamDevTdf *c_pstDht11GetStaticParam(emDht11DevNumTdf emDevNum)
{
    return &s_stDht11Device[emDevNum].stStaticParam;
}

/// @brief      获取只读运行参数
///
/// @note       获取的指针，其指向的内容是只读的
const stDht11RunningParamDevTdf *c_pstDht11GetRunningParam(emDht11DevNumTdf emDevNum)
{
    return &s_stDht11Device[emDevNum].stRunningParam;
}

/// @brief      初始化参数拷贝
///
/// @note
static void s_vDht11InitParamCopy(const stDht11StaticParamDevTdf *c_pstInit, emDht11DevNumTdf emDevNum)
{
    memcpy(&(s_stDht11Device[emDevNum].stStaticParam), c_pstInit, sizeof(stDht11StaticParamDevTdf) / sizeof(uint8_t));
}

/// @brief      设置信号引脚方向为输入
///
/// @note
void vDht11SetPinDirInput(emDht11DevNumTdf emDevNum)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin     = s_stDht11Device[emDevNum].stStaticParam.ulGpioPin;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase, &GPIO_InitStruct);
}

/// @brief      设置信号引脚方向为输出
///
/// @note
void vDht11SetPinDirOutput(emDht11DevNumTdf emDevNum)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin     = s_stDht11Device[emDevNum].stStaticParam.ulGpioPin;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase, &GPIO_InitStruct);
}

/// @brief      设置信号引脚为高电平
///
/// @note
void vDht11WritePinHigh(emDht11DevNumTdf emDevNum)
{
    HAL_GPIO_WritePin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase, 
                      s_stDht11Device[emDevNum].stStaticParam.ulGpioPin,
                      GPIO_PIN_SET);
}

/// @brief      设置信号引脚为低电平
///
/// @note
void vDht11WritePinLow(emDht11DevNumTdf emDevNum)
{
    HAL_GPIO_WritePin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase, 
                      s_stDht11Device[emDevNum].stStaticParam.ulGpioPin,
                      GPIO_PIN_RESET);
}

/// @brief      开始通信
///
/// @note
void vDht11Start(emDht11DevNumTdf emDevNum)
{
    vDht11SetPinDirOutput(emDevNum);
    vDht11WritePinLow(emDevNum);
//    s_vDelayUs(20000);
    vTaskDelay(20);
    vDht11WritePinHigh(emDevNum);
    s_vDelayUs(7);
}

/// @brief      等待设备回复
///
/// @note
uint8_t ucDht11WaitForAck(emDht11DevNumTdf emDevNum)
{
    uint8_t ucCount;
    uint8_t ucPinValue;

    // 1. 引脚方向改为输入
    vDht11SetPinDirInput(emDevNum);
    
    ucCount = 0;
    // 2. 等引脚被拉低
    do
    {
        ucPinValue = HAL_GPIO_ReadPin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase,s_stDht11Device[emDevNum].stStaticParam.ulGpioPin);
        ucCount++;
        if(ucCount >= 100)      // 100us 没有被拉低，则认为出错
        {
            return 0xFF;
        }
        
        s_vDelayUs(1);
    }
    while(ucPinValue == 1);
    
    ucCount = 0;
    // 3. 等引脚被拉高
    do
    {
        ucPinValue = HAL_GPIO_ReadPin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase,s_stDht11Device[emDevNum].stStaticParam.ulGpioPin);
        ucCount++;
        if(ucCount >= 100)      // 100us 没有被拉高，则认为出错
        {
            return 0xFF;
        }
        
        s_vDelayUs(1);
    }
    while(ucPinValue == 0);
    
    ucCount = 0;
    // 4. 等引脚被拉低
    do
    {
        ucPinValue = HAL_GPIO_ReadPin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase,s_stDht11Device[emDevNum].stStaticParam.ulGpioPin);
        ucCount++;
        if(ucCount >= 100)      // 100us 没有被拉低，则认为出错
        {
            return 0xFF;
        }
        
        s_vDelayUs(1);
    }
    while(ucPinValue == 1);
    
    return 0;
}

/// @brief      读取单个 bit
///
/// @note
uint8_t ucDht11ReadBit(emDht11DevNumTdf emDevNum)
{
    uint8_t ucLowLevelCount = 0;
    uint8_t ucHighLevelCount = 0;
    uint8_t ucPinValue;
    
    // 1. 引脚方向改为输入
    vDht11SetPinDirInput(emDevNum);
    
    // 2. 等引脚被拉高
    do
    {
        ucPinValue = HAL_GPIO_ReadPin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase,s_stDht11Device[emDevNum].stStaticParam.ulGpioPin);
        ucLowLevelCount++;
        if(ucLowLevelCount >= 100)      // 100us 没有被拉低，则认为出错
        {
            return 0xFF;
        }
        
        s_vDelayUs(1);
    }
    while(ucPinValue == 0);
    
    // 3. 等引脚被拉低
    do
    {
        ucPinValue = HAL_GPIO_ReadPin(s_stDht11Device[emDevNum].stStaticParam.pstGpioBase,s_stDht11Device[emDevNum].stStaticParam.ulGpioPin);
        ucHighLevelCount++;
        if(ucHighLevelCount >= 100)     // 100us 没有被拉高，则认为出错
        {
            return 0xFF;
        }
        
        s_vDelayUs(1);
    }
    while(ucPinValue == 1);
    
    
    if(ucLowLevelCount > ucHighLevelCount)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/// @brief      读取数据
///
/// @note
uint8_t ucDht11ReadData(emDht11DevNumTdf emDevNum)
{
    uint8_t ucError;
    uint8_t aucData[5] = {0};
    uint8_t ucTempBit;
    uint8_t i, j;
    
    // 1. 发送起始标志
    vDht11Start(emDevNum);
    
    // 2. 等待设备回复
    ucError = ucDht11WaitForAck(emDevNum);
    if(ucError != 0)
    {
        return ucError;
    }
    
    // 3. 连续读取 5 个字节
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            ucTempBit = ucDht11ReadBit(emDevNum);
            if(ucTempBit == 0xFF)
            {
                return 1;
            }
            aucData[i] <<= 1;
            aucData[i] += ucTempBit;
        }
    }
    
    // 4. 累加校验数据
    if(aucData[4] != (aucData[0] + aucData[1] + aucData[2] + aucData[3]))
    {
        return 1;
    }
    
    // 5. 存储数据
    s_stDht11Device[emDevNum].stRunningParam.acHumidity[0] = aucData[0];
    s_stDht11Device[emDevNum].stRunningParam.acHumidity[1] = aucData[1];
    
    s_stDht11Device[emDevNum].stRunningParam.acTemperature[0] = aucData[2];
    s_stDht11Device[emDevNum].stRunningParam.acTemperature[1] = aucData[3];
    
    return 0;
}
/// @brief      设备结构定义
///
/// @note
emDht11CallFuncErrorTdf emDht11DevInit(stDht11StaticParamDevTdf *pstInit, emDht11DevNumTdf emDevNum)
{
    // 1. 拷贝初始化参数
    s_vDht11InitParamCopy(pstInit, emDevNum);
    
    return emDht11CallFuncError_None;
}
