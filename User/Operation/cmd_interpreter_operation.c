/**
  * @file       cmd_interpreter_operation.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251216
  * @brief      命令解释器操作
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "freertos.h"
#include "queue.h"

#include "cmd_interpreter_operation.h"
#include "led_operation.h"

#include "cmd_interpreter_device.h"
#include "led_device.h"

extern QueueHandle_t qLedParamSet;
extern UART_HandleTypeDef huart1;

/// @brief      命令解释器字段定义
///
/// @note
const uint8_t c_aucStringObject_Led[]                   = "LED";
const uint8_t c_aucStringCmd_On[]                       = "ON";
const uint8_t c_aucStringCmd_Off[]                      = "OFF";
const uint8_t c_aucStringCmd_Toggle[]                   = "TOGGLE";
const uint8_t c_aucStringCmd_Blink[]                    = "BLINK";
const uint8_t c_aucStringParam_Led[]                    = "0~8 1~10000 1~10000";

/// @brief      LED 参数设置缓冲区
///
/// @note       用于队列传递 LED 参数
static stLedOperationQueueParamTdf s_stLedParamSetBuffer;



// 串口回显函数：发送字符串给上位机
void CmdSendString(const char *str)
{
    // 发送字符串，长度用 strlen，超时 10ms
    HAL_UART_Transmit(&huart1, (uint8_t *)str, sizeof(str), 10);
}



/// @brief      命令解释器对照表
///
/// @note
const static stCmdInterpreterListTdf cs_astCmdInterpreterList[] = 
{
    // 【LED ON】
    {
        .c_pucObject    = c_aucStringObject_Led,
        .c_pucCmd       = c_aucStringCmd_On,
        .c_pucParam     = c_aucStringParam_Led,
        .c_ucParamNum   = 1,
        .pvfCallback    = vCmdInterpreterExecuteCallback_LedOn,
    },
    
    // 【LED OFF】
    {
        .c_pucObject    = c_aucStringObject_Led,
        .c_pucCmd       = c_aucStringCmd_Off,
        .c_pucParam     = c_aucStringParam_Led,
        .c_ucParamNum   = 1,
        .pvfCallback    = vCmdInterpreterExecuteCallback_LedOff,
    },
    
    // 【LED TOGGLE】
    {
        .c_pucObject    = c_aucStringObject_Led,
        .c_pucCmd       = c_aucStringCmd_Toggle,
        .c_pucParam     = c_aucStringParam_Led,
        .c_ucParamNum   = 1,
        .pvfCallback    = vCmdInterpreterExecuteCallback_LedToggle,
    },
    
    // 【LED BLINK PERIOD】
    {
        .c_pucObject    = c_aucStringObject_Led,
        .c_pucCmd       = c_aucStringCmd_Blink,
        .c_pucParam     = c_aucStringParam_Led,
        .c_ucParamNum   = 2,
        .pvfCallback    = vCmdInterpreterExecuteCallback_LedBlink_ParamNum2,
    },
    
    // 【LED BLINK PERIOD】
    {
        .c_pucObject    = c_aucStringObject_Led,
        .c_pucCmd       = c_aucStringCmd_Blink,
        .c_pucParam     = c_aucStringParam_Led,
        .c_ucParamNum   = 3,
        .pvfCallback    = vCmdInterpreterExecuteCallback_LedBlink_ParamNum3,
    },
};

/// @brief      命令解释器初始化
///
/// @note
void vCmdInterpreterInit()
{
    stCmdInterpreterStaticParamTdf stInit;
    
    stInit.c_pstCmdInterpreterList  = cs_astCmdInterpreterList;
    stInit.ulListSize               = sizeof(cs_astCmdInterpreterList) / sizeof(stCmdInterpreterListTdf);
    emCmdInterpreterDevInit(&stInit, emCmdInterpreterDev0);
}

/// @brief      命令解释器操作执行
///
/// @note
void vCmdInterpreterOperationExecute(uint8_t *pucCmdLine)
{
    emCmdInterpreterExecute(pucCmdLine, emCmdInterpreterDev0);
}

/// @brief      字符串转换为 uint32_t 型整数
///
/// @retval     转换完成的整数
///
/// @note
uint32_t ulStringToUint32(uint8_t *str)
{
    uint32_t ulResult = 0;
    
    while((*str >= '0') && (*str <= '9'))
    {
        ulResult *= 10;
        ulResult += *str - '0';
        str++;
    }
    
    return ulResult;
}

/// @brief      命令解释器执行回调【LED ON】
///
/// @param      ucString    : 命令行首地址
///
/// @note
void vCmdInterpreterExecuteCallback_LedOn(uint8_t **p2ucString)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ulStringToUint32(p2ucString[2]);
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Static;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_On;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = 0xFFFFFFFF;
	
    xQueueSend(qLedParamSet, &p, LED_SEND_BLOCK_COUNT_MAX);
    CmdSendString("OK\r\n");                       // 命令执行完，加一句：

}

/// @brief      命令解释器执行回调【LED OFF】
///
/// @param      ucString    : 命令行首地址
///
/// @note
void vCmdInterpreterExecuteCallback_LedOff(uint8_t **p2ucString)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ulStringToUint32(p2ucString[2]);
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Static;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_Off;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = 0xFFFFFFFF;
    
    xQueueSend(qLedParamSet, &p, LED_SEND_BLOCK_COUNT_MAX);
    CmdSendString("OK\r\n");                       // 命令执行完，加一句：

}

/// @brief      Cmd 命令解释器执行回调【LED TOGGLE】
///
/// @param      ucString    : 命令行首地址
///
/// @note
void vCmdInterpreterExecuteCallback_LedToggle(uint8_t **p2ucString)
{
    const stLedRunningParamTdf *c_pstRunningParam;
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ulStringToUint32(p2ucString[2]);
    
    c_pstRunningParam = c_pstGetLedRunningParam(s_stLedParamSetBuffer.emDevNum);
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Static;
    if(c_pstRunningParam->emCurrentStatus != emLedStatus_Off)
    {
        s_stLedParamSetBuffer.stParam.emCurrentStatus       = emLedStatus_Off;
    }
    else
    {
        s_stLedParamSetBuffer.stParam.emCurrentStatus       = emLedStatus_On;
    }
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = 0xFFFFFFFF;
    
    xQueueSend(qLedParamSet, &p, LED_SEND_BLOCK_COUNT_MAX);
    CmdSendString("OK\r\n");                       // 命令执行完，加一句：

}

/// @brief      命令解释器执行回调【LED BLINK】
///
/// @param      ucString    : 命令行首地址
///
/// @note       带 2 个参数
void vCmdInterpreterExecuteCallback_LedBlink_ParamNum2(uint8_t **p2ucString)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ulStringToUint32(p2ucString[2]);
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Blink;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_NotModify;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = ulStringToUint32(p2ucString[3]);
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = ulStringToUint32(p2ucString[3]);
    
    xQueueSend(qLedParamSet, &p, LED_SEND_BLOCK_COUNT_MAX);
    CmdSendString("OK\r\n");                       // 命令执行完，加一句：

}

/// @brief      命令解释器执行回调【LED BLINK】
///
/// @param      ucString    : 命令行首地址
///
/// @note       带 3 个参数
void vCmdInterpreterExecuteCallback_LedBlink_ParamNum3(uint8_t **p2ucString)
{
    stLedOperationQueueParamTdf *p = &s_stLedParamSetBuffer;
    
    s_stLedParamSetBuffer.emDevNum                          = (emLedDevNumTdf)ulStringToUint32(p2ucString[2]);
    s_stLedParamSetBuffer.stParam.emCurrentMode             = emLedMode_Blink;
    s_stLedParamSetBuffer.stParam.emCurrentStatus           = emLedStatus_NotModify;
    s_stLedParamSetBuffer.stParam.ulBlinkCurrentCount       = 0xFFFFFFFF;
    s_stLedParamSetBuffer.stParam.ulBlinkOnCountThreshold   = ulStringToUint32(p2ucString[3]);
    s_stLedParamSetBuffer.stParam.ulBlinkOffCountThreshold  = ulStringToUint32(p2ucString[4]);
    
    xQueueSend(qLedParamSet, &p, LED_SEND_BLOCK_COUNT_MAX);
	CmdSendString("OK\r\n");                       // 命令执行完，加一句：

}
