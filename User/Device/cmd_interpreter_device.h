/**
  * @file       cmd_interpreter_device.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      命令解释器驱动
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#ifndef _CMD_INTERPRETER_DEVICE_H_
#define _CMD_INTERPRETER_DEVICE_H_

#include "project_config.h"
#include "stdint.h"


/// @brief      设备号定义
/// 
/// @note       
typedef enum
{
    emCmdInterpreterDev0                                = 0,                            // 0  设备 0
    emCmdInterpreterDev1,
    emCmdInterpreterDev2,
    emCmdInterpreterDev3,
}
emCmdInterpreterDevNumTdf;

/// @brief      回调函数指针类型定义
///
/// @param      p2ucString  : 命令行分割好的字段。
///             注意，这是个二重指针，可以理解为指向【字符串数组】的指针。
///             【字符串数组】内是以命令行内空格为标识，分割好的各个字符串。
///
/// @note
typedef void pvfCmdInterpreterCallbackTdf(uint8_t **p2ucString);

/// @brief      命令结构表定义
///
/// @note
typedef struct
{
    const uint8_t                                       *c_pucObject;                   // 对象
    const uint8_t                                       *c_pucCmd;                      // 命令
    const uint8_t                                       *c_pucParam;                    // 参数
    const uint8_t                                       c_ucParamNum;                   // 参数数量
    pvfCmdInterpreterCallbackTdf                        *pvfCallback;                   // 回调函数
}
stCmdInterpreterListTdf;

/// @brief      函数调用错误编码定义
///
/// @note
typedef enum
{
    emCmdInterpreterFuncError_None                      = 0,                            // 0  无错误
    emCmdInterpreterFuncError_CmdLineIllegal            = 1,                            // 1  命令行非法
}
emCmdInterpreterFuncErrorCodeTdf;

/// @brief      静态参数结构定义
/// 
/// @note       
typedef struct
{
    const stCmdInterpreterListTdf                       *c_pstCmdInterpreterList;       // 命令结构表指针
    uint32_t                                            ulListSize;                     // 命令结构表大小
}
stCmdInterpreterStaticParamTdf;

/// @brief      运行参数结构定义
/// 
/// @note      
typedef struct
{
    uint32_t                                            ulCurrentListIndex;             // 当前表索引
}
stCmdInterpreterRunningParamTdf;

/// @brief      驱动结构定义
/// 
/// @note 
typedef struct
{
    stCmdInterpreterStaticParamTdf                      stStaticParam;                  // 静态参数
    stCmdInterpreterRunningParamTdf                     stRunningParam;                 // 运行参数
}
stCmdInterpreterDeviceParamTdf;

const stCmdInterpreterStaticParamTdf *c_pstCmdInterpreterGetStaticParam(emCmdInterpreterDevNumTdf emDevNum);
const stCmdInterpreterRunningParamTdf *c_pstCmdInterpreterGetRunningParam(emCmdInterpreterDevNumTdf emDevNum);
emCmdInterpreterFuncErrorCodeTdf emCmdInterpreterExecute(uint8_t *pucCmdLine, emCmdInterpreterDevNumTdf emDevNum);
emCmdInterpreterFuncErrorCodeTdf emCmdInterpreterDevInit(stCmdInterpreterStaticParamTdf *pstInitParam, emCmdInterpreterDevNumTdf emDevNum);

#endif
