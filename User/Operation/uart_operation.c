/**
  * @file       uart_operation.c
  * @author     ����
  * @version    V1.0.1
  * @date       20251216
  * @brief      UART 操作
  * 
  * <h2><center>&copy;此文件版权归【����】所有.</center></h2>
  */

#include "string.h"
#include "freertos.h"
#include "queue.h"
#include "uart_operation.h"
#include "ring_buffer_device.h"
#include STM32_IC_HAL


extern UART_HandleTypeDef huart1;

/// @brief      UART 缓冲区实例
///
/// @note
static uint8_t s_aucTxByteBuffer[UART_TX_BYTE_BUFFER_SIZE];
static uint8_t s_aucRxByteBuffer[UART_RX_BYTE_BUFFER_SIZE];

static uint8_t s_ucCmdLineBuffer[CMD_INTERPRETER_LINE_BUFFER_SIZE];

extern QueueHandle_t qUartGetCmdLineQueue;

/// @brief      UART 初始化
///
/// @note
void vUartInit()
{
    stRingBufferStaticParamTdf stInit;
    
    stInit.pvHead           = &s_aucTxByteBuffer[0];
    stInit.pvTail           = &s_aucTxByteBuffer[UART_TX_BYTE_BUFFER_SIZE - 1];
    stInit.ulElementLength  = sizeof(s_aucTxByteBuffer) / UART_TX_BYTE_BUFFER_SIZE;
    emRingBufferDevInit(&stInit, UART_TX_BYTE_BUFFER);
    
    stInit.pvHead           = &s_aucRxByteBuffer[0];
    stInit.pvTail           = &s_aucRxByteBuffer[UART_RX_BYTE_BUFFER_SIZE - 1];
    stInit.ulElementLength  = sizeof(s_aucRxByteBuffer) / UART_RX_BYTE_BUFFER_SIZE;
    emRingBufferDevInit(&stInit, UART_RX_BYTE_BUFFER);
    
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

/// @brief      UART 接收中断回调函数
///
/// @note
void vUart1InterruptCallback()
{
    static uint8_t s_ucRTxData;
    
    if((USART1->SR & UART_FLAG_RXNE) == UART_FLAG_RXNE)
    {
        s_ucRTxData = USART1->DR;
        emRingBufferWriteSingleElement(&s_ucRTxData, UART_RX_BYTE_BUFFER);
        USART1->SR &= ~UART_FLAG_RXNE;
    }
}

/// @brief      UART 发送所有 TX 缓冲区中的数据
///
/// @note
void vUartSendAllBufferData()
{
    const stRingBufferRunningParamTdf *c_pstRunningParam = c_pstRingBufferGetRunningParam(UART_TX_BYTE_BUFFER);
    uint8_t ucTxData;
    
    while(c_pstRunningParam->pvRead != c_pstRunningParam->pvWrite)
    {
        emRingBufferReadSingleElement(&ucTxData, UART_TX_BYTE_BUFFER);
        HAL_UART_Transmit(&huart1, &ucTxData, 1, 10);
    }
}

/// @brief      UART 获取命令行
///
/// @note
void vUartGetCmdLine()
{
    const stRingBufferRunningParamTdf *c_pstRunningParam = c_pstRingBufferGetRunningParam(UART_RX_BYTE_BUFFER);
    const stRingBufferStaticParamTdf *c_pstStaticParam = c_pstRingBufferGetStaticParam(UART_RX_BYTE_BUFFER);
    const uint8_t *p;
    uint8_t *q;
    uint8_t *addr = (uint8_t *)&s_ucCmdLineBuffer;
    
    p = c_pstRunningParam->pvRead;
    q = s_ucCmdLineBuffer;
    
    // 1. 找换行符，换行符作为分割标识
    while(p != c_pstRunningParam->pvWrite)
    {
        if(*p == '\n')
        {
            break;
        }
        
        p++;
        if(p > (uint8_t *)c_pstStaticParam->pvTail)
        {
            p = (uint8_t *)c_pstStaticParam->pvHead;
        }
    }
    
    // 2. 没找到换行符，直接返回
    if(p == c_pstRunningParam->pvWrite)
    {
        return;
    }
    
    // 3. 存储命令行
    memset(s_ucCmdLineBuffer, sizeof(s_ucCmdLineBuffer) / sizeof(uint8_t), 0);
    while(c_pstRunningParam->pvRead != p)
    {
        emRingBufferReadSingleElement(q, UART_RX_BYTE_BUFFER);
        q++;
    }
    // p 指向的也要存
    emRingBufferReadSingleElement(q, UART_RX_BYTE_BUFFER);
    
    // 4. 通知串口解释器任务
    xQueueSend(qUartGetCmdLineQueue, &addr, 10);
}
