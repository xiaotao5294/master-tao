/**
  * @file       ring_buffer_device.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251215
  * @brief      环形缓冲区驱动
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "ring_buffer_device.h"
#include "string.h"

/// @brief      设备参数结构体定义
///             
/// @note
stRingBufferDevTdf astRingBufferDev[RING_BUFFER_DEV_NUM];

/// @brief      获取静态参数指针
///
/// @param      emDevNum        ：设备号
///
/// @return     运行结构体指针，注意，获取到的指针指向的值是只读的
///
/// @note
const stRingBufferStaticParamTdf *c_pstRingBufferGetStaticParam(emRingBufferDevNumTdf emDevNum)
{
    return &astRingBufferDev[emDevNum].stStaticParam;
}

/// @brief      获取运行参数指针
///
/// @param      emDevNum        ：设备号
///
/// @return     运行结构体指针，注意，获取到的指针指向的值是只读的
///
/// @note
const stRingBufferRunningParamTdf *c_pstRingBufferGetRunningParam(emRingBufferDevNumTdf emDevNum)
{
    return &astRingBufferDev[emDevNum].stRunningParam;
}

/// @brief      静态信息拷贝
///
/// @param      pstInit         ：静态结构体指针
///             emDevNum        ：设备号
///             
/// @note
static void s_vRingBufferStaticParamCopy(stRingBufferStaticParamTdf *pstInit, emRingBufferDevNumTdf emDevNum)
{
    memcpy(&(astRingBufferDev[emDevNum].stStaticParam), pstInit, sizeof(stRingBufferStaticParamTdf)/sizeof(uint8_t));
}

/// @brief      运行参数初始化
///
/// @param      emDevNum        ：设备号
///             
/// @note
static void s_vRingBufferRunningParamInit(emRingBufferDevNumTdf emDevNum)
{
    astRingBufferDev[emDevNum].stRunningParam.emStatus  = emRingBufferStatus_Empty;
    astRingBufferDev[emDevNum].stRunningParam.pvRead    = astRingBufferDev[emDevNum].stStaticParam.pvHead;
    astRingBufferDev[emDevNum].stRunningParam.pvWrite   = astRingBufferDev[emDevNum].stStaticParam.pvHead;
}

/// @brief      写入单个元素
///
/// @param      pvElement   ：要写入元素的首地址
///             emDevNum    ：设备号
///
/// @return     错误码，详见 emRingBufferErrorCodeTdf 定义
///
/// @note       为了可以在中断中使用，此函数不使用 memcpy 函数
emRingBufferErrorCodeTdf emRingBufferWriteSingleElement(void *pvElement, emRingBufferDevNumTdf emDevNum)
{
    uint32_t i;
    
    // 1. 缓冲区已满，则返回错误
    if(astRingBufferDev[emDevNum].stRunningParam.emStatus == emRingBufferStatus_Full)
    {
        return emRingBufferError_WriteFull;
    }
    
    // 2. 按 Byte 写入
    for(i = 0; i < astRingBufferDev[emDevNum].stStaticParam.ulElementLength; i++)
    {
        *(((uint8_t *)(astRingBufferDev[emDevNum].stRunningParam.pvWrite)) + i) = *((uint8_t *)(pvElement) + i);
    }
    
    // 3. 更新写指针
    astRingBufferDev[emDevNum].stRunningParam.pvWrite = (void *)(((uint8_t *)(astRingBufferDev[emDevNum].stRunningParam.pvWrite)) + i);
    if(astRingBufferDev[emDevNum].stRunningParam.pvWrite > astRingBufferDev[emDevNum].stStaticParam.pvTail)
    {
        astRingBufferDev[emDevNum].stRunningParam.pvWrite = astRingBufferDev[emDevNum].stStaticParam.pvHead;
    }
    
    // 4. 更新状态
    // 4.1 更新写指针后，写指针和读指针相等，则说明缓冲区已满
    if(astRingBufferDev[emDevNum].stRunningParam.pvWrite == astRingBufferDev[emDevNum].stRunningParam.pvRead)       
    {
        astRingBufferDev[emDevNum].stRunningParam.emStatus = emRingBufferStatus_Full;
    }
    // 4.2 缓冲区未满
    else
    {
        astRingBufferDev[emDevNum].stRunningParam.emStatus = emRingBufferStatus_NotEmptyNotFull;
    }
    
    return emRingBufferError_None;
}

/// @brief      读取单个元素
///
/// @param      pvElement   ：读取元素后存储的地址
///             emDevNum    ：设备号
///
/// @return     错误码，详见 emRingBufferErrorCodeTdf 定义
///
/// @note       为了可以在中断中使用，此函数不使用 memcpy 函数
emRingBufferErrorCodeTdf emRingBufferReadSingleElement(void *pvElement, emRingBufferDevNumTdf emDevNum)
{
    uint32_t i;
    
    // 1. 缓冲区为空，则返回错误
    if(astRingBufferDev[emDevNum].stRunningParam.emStatus == emRingBufferStatus_Empty)
    {
        return emRingBufferError_ReadEmpty;
    }
    
    // 2. 按 Byte 读出
    for(i = 0; i < astRingBufferDev[emDevNum].stStaticParam.ulElementLength; i++)
    {
        *((uint8_t *)(pvElement) + i) = *(((uint8_t *)(astRingBufferDev[emDevNum].stRunningParam.pvRead)) + i);
    }
    
    // 3. 更新读指针
    astRingBufferDev[emDevNum].stRunningParam.pvRead = (void *)(((uint8_t *)(astRingBufferDev[emDevNum].stRunningParam.pvRead)) + i);
    if(astRingBufferDev[emDevNum].stRunningParam.pvRead > astRingBufferDev[emDevNum].stStaticParam.pvTail)
    {
        astRingBufferDev[emDevNum].stRunningParam.pvRead = astRingBufferDev[emDevNum].stStaticParam.pvHead;
    }
    
    // 4. 更新状态
    // 4.1 更新读指针后，写指针与读指针相等，则说明缓冲区为空
    if(astRingBufferDev[emDevNum].stRunningParam.pvWrite == astRingBufferDev[emDevNum].stRunningParam.pvRead)
    {
        astRingBufferDev[emDevNum].stRunningParam.emStatus = emRingBufferStatus_Empty;
    }
    // 4.2 缓冲区不为空
    else
    {
        astRingBufferDev[emDevNum].stRunningParam.emStatus = emRingBufferStatus_NotEmptyNotFull;
    }
    
    return emRingBufferError_None;
}

/// @brief      设备初始化，即新建设备
///
/// @param      pstInitParam：初始化结构体指针
///             emDevNum    ：设备号
///
/// @return     错误码，详见 emRingBufferErrorCodeTdf 定义
///
/// @note
emRingBufferErrorCodeTdf emRingBufferDevInit(stRingBufferStaticParamTdf *pstInit, emRingBufferDevNumTdf emDevNum)
{
    if((pstInit->pvHead >= pstInit->pvTail) || 
       (pstInit->ulElementLength == 0))
    {
        return emRingBufferError_InitError;
    }
    
    s_vRingBufferStaticParamCopy(pstInit, emDevNum);
    s_vRingBufferRunningParamInit(emDevNum);
    
    return emRingBufferError_None;
}

