/**
  * @file       uart_task.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      UART 任务，包含发送任务，接收任务，命令解释任务
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */
 
#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "uart_task.h"
#include "uart_operation.h"
#include "cmd_interpreter_operation.h"

/// @brief      UART 获取命令行队列
///
/// @note
QueueHandle_t qUartGetCmdLineQueue;

/// @brief      UART TX 任务
///
/// @note
void vUartTxTask()
{
    while(1)
    {
        vTaskDelay(UART_TX_TASK_PERIOD_MS);
        
        vUartSendAllBufferData();
    }
}

/// @brief      UART RX 任务
///
/// @note
void vUartRxTask()
{
    qUartGetCmdLineQueue = xQueueCreate(UART_GET_CMD_QUEUE_SIZE, sizeof(uint8_t *));
    
    vUartInit();
    while(1)
    {
        vTaskDelay(UART_RX_TASK_PERIOD_MS);
        
        vUartGetCmdLine();
    }
}

/// @brief      UART 命令解释器任务
///
/// @note
void vUartCmdInterpreterTask()
{
    uint8_t *pucData;
    
    vCmdInterpreterInit();
    
    while(1)
    {
        vTaskDelay(UART_CMD_INTERPRETER_TASK_PERIOD_MS);
        
        if(xQueueReceive(qUartGetCmdLineQueue, &pucData, 0))
        {
            vCmdInterpreterOperationExecute(pucData);
        }
        
    }
}
