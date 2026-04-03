/**
  * @file       uart_task.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251217
  * @brief      UART 任务，包含发送任务，接收任务，命令解释任务
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _UART_TASK_H_
#define _UART_TASK_H_

#define UART_TX_TASK_PERIOD_MS                          1                               // UART TX 任务周期，单位 ms
#define UART_RX_TASK_PERIOD_MS                          1                               // UART RX 任务周期，单位 ms
#define UART_CMD_INTERPRETER_TASK_PERIOD_MS             1                               // UART 命令解析任务周期，单位 ms
#define UART_GET_CMD_QUEUE_SIZE                         2                               // UART 获取命令队列大小

#endif
