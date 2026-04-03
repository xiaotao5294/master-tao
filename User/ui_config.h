/**
  * @file       ui_config.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      UI 配置文件
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 
  
  
#ifndef _UI_CONFIG_H_
#define _UI_CONFIG_H_

#define SCREEN_POINT_WIDTH                              128
#define SCREEN_POINT_HEIGHT                             64

#define UI_FRAME_DATA_WIDTH                             SCREEN_POINT_WIDTH              // 一整页数据的行宽度，等于屏幕一行的像素数量
#define UI_FRAME_DATA_HEIGHT                            (SCREEN_POINT_HEIGHT / 8)       // 一整页数据的行高度，等于屏幕一列的像素数量 / 8
                                                                                        // （刷新方式为行列式，一个字节会自上往下写 8 个 bit）


#endif
