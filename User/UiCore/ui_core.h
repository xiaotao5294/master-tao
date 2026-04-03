/**
  * @file       ui_core.h
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      UI 内核
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#ifndef _UI_CORE_H_
#define _UI_CORE_H_

#include "stdint.h"
#include "string.h"
#include "ui_font.h"
#include "ui_config.h"


#ifndef NULL
    #define NULL 0
#endif

/// @brief      回调函数类型定义
///
/// @note
typedef void vUiFrameCallback(void *);

/// @brief      字号枚举定义
///
/// @note
typedef enum
{
    emUiFontSize_12x6                                   = 12,                           // 12  字符大小 12x6 
    emUiFontSize_16x8                                   = 16,                           // 16  字符大小 16x8
    emUiFontSize_24x12                                  = 24,                           // 24  字符大小 24x12
}
emUiFontSizeTdf;

/// @brief      像素显示模式枚举定义
///
/// @note
typedef enum
{
    emUiPixelShowMode_Positive                          = 0,                            // 0  正色
    emUiPixelShowMode_Negative                          = 1,                            // 1  反色
}
emUiPixelShowModeTdf;

/// @brief      控件类型枚举定义
///
/// @note
typedef enum
{
    emUiWidgetType_None                                 = 0,                            // 0  无类型
    emUiWidgetType_TextBox                              = 1,                            // 1  文本框
    emUiWidgetType_MenuBox                              = 2,                            // 2  菜单框
}
emUiWidgetTypeTdf;

/// @brief      文本框结构定义
///
/// @note
typedef struct
{
    uint16_t                                            x;                              // 起始 x 坐标
    uint16_t                                            y;                              // 起始 y 坐标
    emUiFontSizeTdf                                     emSize;                         // 字号
    uint8_t                                             *pucData;                       // 显示数据
    emUiPixelShowModeTdf                                emMode;                         // 显示模式
}
stUiTextBoxTdf;

/// @brief      菜单框单行数据结构定义
///
/// @note
typedef struct
{
    uint16_t                                            usIndex;                        // 索引，指示这是第几个选项
    uint8_t                                             *pucString;                     // 显示信息
}
stUiMenuBoxLineDataTdf;

/// @brief      菜单框结构定义
///
/// @note
typedef struct
{
    uint16_t                                            x;                              // 起始 x 坐标
    uint16_t                                            y;                              // 起始 y 坐标
    uint8_t                                             *pucSelectSign;                 // 选择指示图标
    uint8_t                                             ucLineSpacing;                  // 行距，单位为像素
    uint8_t                                             ucShowLineNum;                  // 显示行数
    uint8_t                                             ucMaxLineNum;                   // 最大行数
    uint8_t                                             ucCurrentSelectPosition;        // 当前选中的位置
    uint16_t                                            usCurrentSelectIndex;           // 当前选中的索引
    emUiFontSizeTdf                                     emSize;                         // 字号
    stUiMenuBoxLineDataTdf                              *pstData;                       // 显示数据，应该指向一个 const stUiMenuBoxLineDataTdf 型的数组，存储所有行的显示信息
    emUiPixelShowModeTdf                                emMode;                         // 显示模式
}
stUiMenuBoxTdf;

/// @brief      控件节点结构定义
///
/// @note
typedef struct stUiWidgetNodeTdf
{
    emUiWidgetTypeTdf                                   emType;                         // 当前控件节点的控件类型
    void                                                *pvWidget;                      // 当前控件的指针，指向控件实例
    struct stUiWidgetNodeTdf                            *pstNextNode;                   // 下一节点指针
}
stUiWidgetNodeTdf;

/// @brief      页面结构定义
///
/// @note
typedef struct
{
    stUiWidgetNodeTdf                                   *pstWidgetNodeHead;             // 控件链表首节点
    stUiWidgetNodeTdf                                   *pstWidgetNodeTail;             // 控件链表首节点
    vUiFrameCallback                                    *pvCallback;                    // 回调函数指针
}
stUiFrameTdf;

/// @brief      UI 对象结构定义
///
/// @note       整个 UI 的顶层结构。
///             UI 对象由多个页面（Frame）组成，而单个页面（Frame）由多个控件（Widget）组成
typedef struct
{
    stUiFrameTdf                                        **p2stFrame;                    // 页面首地址，应该指向一个 (stUiFrameTdf *) 型的数组
    uint32_t                                            ulCurrentFrameIndex;            // 当前页面的索引
}
stUiObjectTdf;

const uint8_t *c_pucUiGetFrameBuffer(void);
void vClearFrameBuffer(emUiPixelShowModeTdf emMode);
void vUiAddOneWidgetToFrameAsHeadWidgetNode(stUiFrameTdf *pstFrame, stUiWidgetNodeTdf *pstWidget);
void vUiAddOneWidgetToFrameAsTailWidgetNode(stUiFrameTdf *pstFrame, stUiWidgetNodeTdf *pstWidget);
void vUpdateUiObjectToFrameBuffer(stUiObjectTdf *pstUi);
void vUiFrameCallbackExecute(stUiObjectTdf *pstUi, void *p);
void vUiMenuBoxExecuteSelectDec(stUiMenuBoxTdf *pstMenuBox);
void vUiMenuBoxExecuteSelectInc(stUiMenuBoxTdf *pstMenuBox);

#endif
