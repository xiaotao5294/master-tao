/**
  * @file       project_config.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251130
  * @brief      工程配置文件
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 
  
#ifndef _PROJECT_CONFIG_H_
#define _PROJECT_CONFIG_H_

#define TRUE                                            1
#define FALSE                                           0

#define STM32_IC_HAL                                    "stm32f1xx_hal.h"

#define LED_DEV_NUM                                     9                               // LED 设备数
#define LED_STATUS                                      emLedDevNum0
#define LED1                                            emLedDevNum1
#define LED2                                            emLedDevNum2
#define LED3                                            emLedDevNum3
#define LED4                                            emLedDevNum4
#define LED5                                            emLedDevNum5
#define LED6                                            emLedDevNum6
#define LED7                                            emLedDevNum7
#define LED8                                            emLedDevNum8

#define RING_BUFFER_DEV_NUM                             2                               // 环形缓冲区设备数
#define UART_TX_BYTE_BUFFER                             emRingBufferDev0
#define UART_RX_BYTE_BUFFER                             emRingBufferDev1
#define UART_TX_BYTE_BUFFER_SIZE                        100
#define UART_RX_BYTE_BUFFER_SIZE                        100

#define CMD_INTERPRETER_DEV_NUM                         1                               // 命令解释器设备数
#define CMD_INTERPRETER_LINE_BUFFER_SIZE                50                              // 命令解释器行缓存大小
#define CMD_INTERPRETER_DILIVERY_NUM_MAX                5                               // 命令解释器分割最大段数
#define CMD_INTERPRETER_DILIVER_STRING                  " "                             // 命令解释器分割字符串

#define BUTTON_DEV_NUM                                  4                               // 按键设备数
#define BUTTON_UP                                       emButtonDevNum0
#define BUTTON_DOWN                                     emButtonDevNum1
#define BUTTON_ENTER                                    emButtonDevNum2
#define BUTTON_CANCEL                                   emButtonDevNum3

#define OLED_DEVICE_NUM                                 1                               // OLED 设备数
#define OLED                                            emOledDevNum0
#define OLED_POINT_WIDTH                                128
#define OLED_POINT_HEIGHT                               64

#define UI_FRAME_NUM                                    3                               // UI 页面数

#define DHT11_DEVICE_NUM                                1                               // DHT11 设备数
#define DHT11                                           emDht11DevNum0

#endif

