/**
  * @file       ui_frame_main.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251220
  * @brief      UI 主界面
  * 
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */

#include "freertos.h"
#include "queue.h"

#include "ui_frame_main.h"
#include "project_config.h"
#include "oled_operation.h"
#include "button_device.h"

extern QueueHandle_t qUiParamSet;

/// @brief      显示信息
///
/// @note
static uint8_t s_aucMenuBoxData_SelectSign[]    = ">";
static uint8_t s_aucMenuBoxData_LedCtrl[]       = "  01.Led Control     ";
static uint8_t s_aucMenuBoxData_Dht11Show[]     = "  02.Dht11 Show      ";
static uint8_t s_aucMenuBoxData_Game[]          = "  03.Game            ";
static uint8_t s_aucMenuBoxData_TimeShow[]      = "  04.Time Show       ";
static uint8_t s_aucMenuBoxData_ButtonTest[]    = "  05.Button Test     ";
static uint8_t s_aucMenuBoxData_About[]         = "  06.About           ";
static uint8_t s_aucMenuBoxData_07[]            = "  07.                ";
static uint8_t s_aucMenuBoxData_08[]            = "  08.                ";
static uint8_t s_aucMenuBoxData_09[]            = "  09.                ";
static uint8_t s_aucMenuBoxData_10[]            = "  10.TEST1           ";
static uint8_t s_aucMenuBoxData_11[]            = "  11.   TEST2        ";
static uint8_t s_aucMenuBoxData_12[]            = "  12.                ";
static uint8_t s_aucMenuBoxData_13[]            = "  13.                ";
static uint8_t s_aucMenuBoxData_14[]            = "  14.                ";

/// @brief     菜单显示信息实例
///
/// @note
static stUiMenuBoxLineDataTdf s_astMainFrameMenuBoxData[] = 
{
    {.usIndex = 0,  .pucString = s_aucMenuBoxData_LedCtrl},
    {.usIndex = 1,  .pucString = s_aucMenuBoxData_Dht11Show},
    {.usIndex = 2,  .pucString = s_aucMenuBoxData_Game},
    {.usIndex = 3,  .pucString = s_aucMenuBoxData_TimeShow},
    {.usIndex = 4,  .pucString = s_aucMenuBoxData_ButtonTest},
    {.usIndex = 5,  .pucString = s_aucMenuBoxData_About},
    {.usIndex = 6,  .pucString = s_aucMenuBoxData_07},
    {.usIndex = 7,  .pucString = s_aucMenuBoxData_08},
    {.usIndex = 8,  .pucString = s_aucMenuBoxData_09},
    {.usIndex = 9,  .pucString = s_aucMenuBoxData_10},
    {.usIndex = 10, .pucString = s_aucMenuBoxData_11},
    {.usIndex = 11, .pucString = s_aucMenuBoxData_12},
    {.usIndex = 12, .pucString = s_aucMenuBoxData_13},
    {.usIndex = 13, .pucString = s_aucMenuBoxData_14},
};

/// @brief      菜单框实例
///
/// @note
static stUiMenuBoxTdf s_stMainFrameMenuBox = 
{
    .x                          = 0,
    .y                          = 0,
    .pucSelectSign              = s_aucMenuBoxData_SelectSign,
    .ucLineSpacing              = 2,
    .ucShowLineNum              = 5,
    .ucMaxLineNum               = sizeof(s_astMainFrameMenuBoxData) / sizeof(s_astMainFrameMenuBoxData[0]),
    .ucCurrentSelectPosition    = 0,
    .usCurrentSelectIndex       = 0,
    .emSize                     = emUiFontSize_12x6,
    .pstData                    = s_astMainFrameMenuBoxData,
    .emMode                     = emUiPixelShowMode_Positive,
};

/// @brief      主页面【菜单框】控件节点实例
///
/// @note
static stUiWidgetNodeTdf s_stMainFrameWidgetNode_MenuBox = 
{
    .emType                     = emUiWidgetType_MenuBox,
    .pvWidget                   = &s_stMainFrameMenuBox,
    .pstNextNode                = NULL,
};

/// @brief      主页面实例
///
/// @note
static stUiFrameTdf s_stMainFrame;

/// @brief      主页面【ENTER】键执行
///
/// @note
static void s_vMainFrameButtonEnterExecute()
{
    uint32_t ulSendParam = s_stMainFrameMenuBox.usCurrentSelectIndex + 1;
    
    switch(s_stMainFrameMenuBox.usCurrentSelectIndex)
    {
        case 0:
        case 1:
        {
            xQueueSend(qUiParamSet, &ulSendParam, 10);
            
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      主页面回调函数
///
/// @note
static void s_vMainFrameCallback(void *p)
{
    uint8_t i;
    uint32_t ulButtonValue = *((uint32_t *)p);
    uint32_t ulButtonValueMask = 0x00000001;
    
    for(i = 0; i < BUTTON_DEV_NUM; i++)
    {
        if((ulButtonValue & ulButtonValueMask) != 0)
        {
            break;
        }
        
        ulButtonValueMask <<= 1;
    }
    
    switch(i)
    {
        case BUTTON_UP:
        {
            vUiMenuBoxExecuteSelectDec(&s_stMainFrameMenuBox);
            break;
        }
        case BUTTON_DOWN:
        {
            vUiMenuBoxExecuteSelectInc(&s_stMainFrameMenuBox);
            break;
        }
        case BUTTON_ENTER:
        {
            s_vMainFrameButtonEnterExecute();
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      主页面初始化
///
/// @note
void vMainFrameInit()
{
    vUiAddOneWidgetToFrameAsTailWidgetNode(&s_stMainFrame, &s_stMainFrameWidgetNode_MenuBox);
    s_stMainFrame.pvCallback = s_vMainFrameCallback;
}

/// @brief      获取主页面
///
/// @note
stUiFrameTdf *pstGetMainFrame()
{
    return &s_stMainFrame;
}
