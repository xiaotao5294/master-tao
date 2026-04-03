/**
  * @file       cmd_interpreter_device.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      命令解释器驱动
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "cmd_interpreter_device.h"
#include "string.h"

/// @brief      设备参数结构体定义
///             
/// @note
stCmdInterpreterDeviceParamTdf astCmdInterpreterDevice[CMD_INTERPRETER_DEV_NUM];

/// @brief      获取静态参数指针
///
/// @param      emDevNum        ：设备号
///
/// @return     运行结构体指针，注意，获取到的指针指向的值是只读的
///
/// @note
const stCmdInterpreterStaticParamTdf *c_pstCmdInterpreterGetStaticParam(emCmdInterpreterDevNumTdf emDevNum)
{
    return &astCmdInterpreterDevice[emDevNum].stStaticParam;
}

/// @brief      获取运行参数指针
///
/// @param      emDevNum        ：设备号
///
/// @return     运行结构体指针，注意，获取到的指针指向的值是只读的
///
/// @note
const stCmdInterpreterRunningParamTdf *c_pstCmdInterpreterGetRunningParam(emCmdInterpreterDevNumTdf emDevNum)
{
    return &astCmdInterpreterDevice[emDevNum].stRunningParam;
}

/// @brief      静态信息拷贝
///
/// @param      pstInit         ：静态结构体指针
///             emDevNum        ：设备号
///             
/// @note
static void s_vCmdInterpreterStaticParamCopy(stCmdInterpreterStaticParamTdf *pstInit, emCmdInterpreterDevNumTdf emDevNum)
{
    memcpy(&(astCmdInterpreterDevice[emDevNum].stStaticParam), pstInit, sizeof(stCmdInterpreterStaticParamTdf)/sizeof(uint8_t));
}

/// @brief      命令执行
///
/// @param      pucCmdLine      : 命令行首地址
/// @param      emDevNum        : 设备号
///
/// @retval     错误代码，详见 emCmdInterpreterFuncErrorCodeTdf 定义
///
/// @note       命令格式为：【对象】 + 【命令】 + （【参数 1】 + 【参数 2】 + ......）
///             其中，【对象】和【命令】为必选项，【参数 n】为可选项
emCmdInterpreterFuncErrorCodeTdf emCmdInterpreterExecute(uint8_t *pucCmdLine, emCmdInterpreterDevNumTdf emDevNum)
{
    uint8_t i;
    uint8_t ucCmdHasFound                                   = FALSE;
    uint8_t *apucString[CMD_INTERPRETER_DILIVERY_NUM_MAX]   = {0};
    uint8_t ucStringNum                                     = 0;
    uint8_t aucDiliverString[]                              = CMD_INTERPRETER_DILIVER_STRING;
    
    // 1. 按空格分割原始字符串（首次）
    ucStringNum = 0;
    apucString[ucStringNum] = (uint8_t *)strtok((char *)pucCmdLine, (char *)aucDiliverString);
    
    // 2. 按空格分割原始字符串（多次）
    while(1)
    {
        if(apucString[ucStringNum] == 0)
        {
            break;
        }
        
        ucStringNum++;
        apucString[ucStringNum] = (uint8_t *)strtok(0, (char *)aucDiliverString);
    }
    
    // 3. 匹配【参数个数】、【对象】、【命令】
    for(i = 0; i < astCmdInterpreterDevice[emDevNum].stStaticParam.ulListSize; i++)
    {
        if((ucStringNum - 2) != astCmdInterpreterDevice[emDevNum].stStaticParam.c_pstCmdInterpreterList[i].c_ucParamNum)
        {
            continue;
        }
        
        if(strcmp((char *)apucString[0], (char *)astCmdInterpreterDevice[emDevNum].stStaticParam.c_pstCmdInterpreterList[i].c_pucObject)!=0)
        {
            continue;
        }
        
        if(strcmp((char *)apucString[1], (char *)astCmdInterpreterDevice[emDevNum].stStaticParam.c_pstCmdInterpreterList[i].c_pucCmd)!=0)
        {
            continue;
        }
        
        ucCmdHasFound = TRUE;
        break;
    }
    
    // 4. 匹配成功，转到回调函数
    if(ucCmdHasFound == TRUE)
    {
        astCmdInterpreterDevice[emDevNum].stStaticParam.c_pstCmdInterpreterList[i].pvfCallback(apucString);
        return emCmdInterpreterFuncError_None;
    }
    
    // 5. 匹配不成功，返回错误
    return emCmdInterpreterFuncError_CmdLineIllegal;
}

/// @brief      设备初始化，即新建设备
///
/// @param      pstInit     ：初始化结构体指针
///             emDevNum        ：设备号
///
/// @return     错误码，详见 emCmdInterpreterFuncErrorCodeTdf 定义
///
/// @note
emCmdInterpreterFuncErrorCodeTdf emCmdInterpreterDevInit(stCmdInterpreterStaticParamTdf *pstInit, emCmdInterpreterDevNumTdf emDevNum)
{
    s_vCmdInterpreterStaticParamCopy(pstInit, emDevNum);
    return emCmdInterpreterFuncError_None;
}

