/**
  * @file       ui_frame_dht11.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      UI DHT11 页面
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#ifndef _UI_FRAME_DHT11_H_
#define _UI_FRAME_DHT11_H_

#include "stdint.h"
#include "ui_core.h"


void vDht11FrameInit(void);
stUiFrameTdf *pstGetDht11Frame(void);
void vDht11FrameUpdateTemperatureValue(int16_t TemperatureValue);
void vDht11FrameUpdateHumidityValue(int16_t HumidityValue);


#endif

