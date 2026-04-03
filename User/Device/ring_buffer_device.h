/**
  * @file       ring_buffer_device.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251215
  * @brief      环形缓冲区驱动
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#ifndef _RING_BUFFER_DEVICE_H_
#define _RING_BUFFER_DEVICE_H_

#include "project_config.h"
#include "stdint.h"

/// @brief      设备号定义
/// 
/// @note       
typedef enum
{
    emRingBufferDev0                                    = 0,                            // 0  设备 0
    emRingBufferDev1,
    emRingBufferDev2,
    emRingBufferDev3,
    emRingBufferDev4,
    emRingBufferDev5,
    emRingBufferDev6,
    emRingBufferDev7,
}
emRingBufferDevNumTdf;

/// @brief      错误编码定义
///
/// @note
typedef enum
{
    emRingBufferError_None                              = 0,                            // 0  无错误
    emRingBufferError_InitError                         = 1,                            // 1  初始化错误
    emRingBufferError_WriteFull                         = 2,                            // 2  写入满，即缓冲区已满，写入失败
    emRingBufferError_ReadEmpty                         = 3,                            // 3  读取空，即缓冲区为空，读取失败
}
emRingBufferErrorCodeTdf;

/// @brief      状态定义
///
/// @note
typedef enum
{
    emRingBufferStatus_Empty                            = 0,                            // 0  空
    emRingBufferStatus_Full                             = 1,                            // 1  满
    emRingBufferStatus_NotEmptyNotFull                  = 2,                            // 2  非空非满
}
emRingBufferStatusTdf;

/// @brief      静态参数结构定义
/// 
/// @note       
typedef struct
{
    void                                                *pvHead;                        // 缓冲区头，指向缓冲区地址的第一个元素
    void                                                *pvTail;                        // 缓冲区尾，指向缓冲区地址的最后一个元素
    uint32_t                                            ulElementLength;                // 元素长度
}
stRingBufferStaticParamTdf;

/// @brief      运行参数结构定义
/// 
/// @note      
typedef struct
{
    void                                                *pvWrite;                       // 写指针，指向第一个可写的地址
    void                                                *pvRead;                        // 读指针，指向第一个可读的地址
    emRingBufferStatusTdf                               emStatus;                       // 缓冲区状态
}
stRingBufferRunningParamTdf;

/// @brief      驱动结构定义
/// 
/// @note 
typedef struct
{
    stRingBufferStaticParamTdf                          stStaticParam;                  // 静态参数
    stRingBufferRunningParamTdf                         stRunningParam;                 // 运行参数
}
stRingBufferDevTdf;


const stRingBufferStaticParamTdf *c_pstRingBufferGetStaticParam(emRingBufferDevNumTdf emDevNum);
const stRingBufferRunningParamTdf *c_pstRingBufferGetRunningParam(emRingBufferDevNumTdf emDevNum);
emRingBufferErrorCodeTdf emRingBufferWriteSingleElement(void *pvElement, emRingBufferDevNumTdf emDevNum);
emRingBufferErrorCodeTdf emRingBufferReadSingleElement(void *pvElement, emRingBufferDevNumTdf emDevNum);
emRingBufferErrorCodeTdf emRingBufferDevInit(stRingBufferStaticParamTdf *pstInitParam, emRingBufferDevNumTdf emDevNum);

#endif
