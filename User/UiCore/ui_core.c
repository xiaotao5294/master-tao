/**
  * @file       ui_core.c
  * @author     桃子
  * @version    V1.0.1
  * @date       20251219
  * @brief      UI 内核
  *
  * <h2><center>&copy;此文件版权归【桃子】所有.</center></h2>
  */ 

#include "ui_core.h"

/// @brief      单帧显存定义
///
/// @note       
static uint8_t s_aucUiOneFrameBuffer[UI_FRAME_DATA_WIDTH * UI_FRAME_DATA_HEIGHT];

/// @brief      获取 UI 显存首地址
///
/// @note       注意，读取到的指针，内容是只读的
const uint8_t *c_pucUiGetFrameBuffer()
{
    return (const uint8_t *)s_aucUiOneFrameBuffer;
}

/// @brief      显存清零
///
/// @note
void vClearFrameBuffer(emUiPixelShowModeTdf emMode)
{
    uint8_t ucClearData;
    
    if(emMode == emUiPixelShowMode_Positive)
    {
        ucClearData = 0;
    }
    else
    {
        ucClearData = 0xFF;
    }
    memset(s_aucUiOneFrameBuffer, ucClearData, sizeof(s_aucUiOneFrameBuffer) / sizeof(s_aucUiOneFrameBuffer[0]));
}

/// @brief      向页面内的控件链表添加一个控件，添加的控件作为控件链表的【首节点】
///
/// @param      pstFrame    : 页面实例首地址
/// @param      pstWidget   : 控件实例首地址
///
/// @note       如果页面内还没有控件，则必须将控件链表的首、尾指针置为 0
void vUiAddOneWidgetToFrameAsHeadWidgetNode(stUiFrameTdf *pstFrame, stUiWidgetNodeTdf *pstWidget)
{
    if(pstFrame->pstWidgetNodeHead == NULL)
    {
        pstFrame->pstWidgetNodeHead                 = pstWidget;
        pstFrame->pstWidgetNodeTail                 = pstWidget;
        pstFrame->pstWidgetNodeTail->pstNextNode    = NULL;
        
        return;
    }
    
    pstWidget->pstNextNode                          = pstFrame->pstWidgetNodeHead;
    pstFrame->pstWidgetNodeHead                     = pstWidget;
}

/// @brief      向页面内的控件链表添加一个控件，添加的控件作为控件链表的【尾节点】
///
/// @param      pstFrame    : 页面实例首地址
/// @param      pstWidget   : 控件实例首地址
///
/// @note       如果页面内还没有控件，则必须将控件链表的首、尾指针置为 0
void vUiAddOneWidgetToFrameAsTailWidgetNode(stUiFrameTdf *pstFrame, stUiWidgetNodeTdf *pstWidget)
{
    if(pstFrame->pstWidgetNodeTail == NULL)
    {
        pstFrame->pstWidgetNodeTail                 = pstWidget;
        pstFrame->pstWidgetNodeHead                 = pstWidget;
        pstFrame->pstWidgetNodeTail->pstNextNode    = NULL;
        
        return;
    }
    
    pstFrame->pstWidgetNodeTail->pstNextNode        = pstWidget;
    pstFrame->pstWidgetNodeTail                     = pstWidget;
    pstFrame->pstWidgetNodeTail->pstNextNode        = NULL;
}

/// @brief      向缓冲区指定位置画 1 个点
///
/// @param      x           ：X 坐标，取值范围 0 ~ UI_FRAME_DATA_WIDTH - 1
/// @param      y           ：Y 坐标，取值范围 0 ~ UI_FRAME_DATA_HEIGHT - 1
///
/// @note
static void s_vUiDrawOnePointToBuffer(uint16_t x, uint16_t y)
{
    uint16_t i, j, k;
    
    i = y >> 3;             // i = y / 8
    j = y & 0x07;           // j = y % 8
    k = 1 << j;
    
    s_aucUiOneFrameBuffer[x * UI_FRAME_DATA_HEIGHT + i] |= k;
}

/// @brief      向缓冲区指定位置清除 1 个点
///
/// @param      x           ：X 坐标，取值范围 0 ~ UI_FRAME_DATA_WIDTH - 1
/// @param      y           ：Y 坐标，取值范围 0 ~ UI_FRAME_DATA_HEIGHT - 1
///
/// @note
static void s_vUiClearOnePointToBuffer(uint16_t x, uint16_t y)
{
    uint16_t i, j, k;
    
    i = y >> 3;             // i = y / 8
    j = y & 0x07;           // j = y % 8
    k = 1 << j;
    
    s_aucUiOneFrameBuffer[x * UI_FRAME_DATA_HEIGHT + i] &= ~k;
}

/// @brief      向缓存写入 1 个字符
///
/// @param      x           ：X 坐标，取值范围 0 ~ UI_FRAME_DATA_WIDTH - 1
/// @param      y           ：Y 坐标，取值范围 0 ~ UI_FRAME_DATA_HEIGHT - 1
/// @param      ucChar      ：要显示的字符
/// @param      emFontSize  ：字号，见 emUiFontSizeTdf 定义
/// @param      emMode      : 显示模式，见 emUiPixelShowModeTdf 定义
///
/// @note
static void s_vUiWriteOneCharToBuffer(uint16_t x, uint16_t y, uint8_t ucChar, emUiFontSizeTdf emFontSize, emUiPixelShowModeTdf emMode)
{
    uint16_t i, j;
    uint16_t y0;
    uint8_t ucData, ucSizeTemp, ucCharIndex;
    
    // 1. 计算显示 1 个字符，需要写多少 BYTE
    //ucSizeTemp = ((emFontSize / 8) + ((emFontSize % 8) ? 1 : 0)) * (emFontSize / 2);        // 除法和求模太慢了，用位移和逻辑运算代替
    ucSizeTemp = ((emFontSize >> 3) + ((emFontSize & 0x07) ? 1 : 0)) * (emFontSize >> 1);
    
    // 2. 计算要显示字符，在字模数组中的索引
    ucCharIndex = ucChar - ' ';
    
    // 3. 初始化 Y0
    y0 = y;
    
    // 4. 将字符写入缓存
    for(i = 0; i < ucSizeTemp; i++)
    {
        // 4.1 选择字模数组
        if(emFontSize == emUiFontSize_12x6)
        {
            ucData = c_a2ucUiFont_Ascii_1206[ucCharIndex][i];
        }
        else if(emFontSize == emUiFontSize_16x8)
        {
            ucData = c_a2ucUiFont_Ascii_1608[ucCharIndex][i];
        }
        else if(emFontSize == emUiFontSize_24x12)
        {
            ucData = c_a2ucUiFont_Ascii_2412[ucCharIndex][i];
        }
        else 
        {
            return;
        }
        
        // 4.2 写入缓存
        for(j = 0; j < 8; j++)
        {
            if((ucData & 0x80) != 0)
            {
                if(emMode == emUiPixelShowMode_Positive)
                {
                    s_vUiDrawOnePointToBuffer(x, y);
                }
                else
                {
                    s_vUiClearOnePointToBuffer(x, y);
                }
            }
            else
            {
                if(emMode == emUiPixelShowMode_Positive)
                {
                    s_vUiClearOnePointToBuffer(x, y);
                }
                else
                {
                    s_vUiDrawOnePointToBuffer(x, y);
                }
            }
            
            ucData <<= 1;
            
            y++;
            if((y - y0) == (uint8_t)emFontSize)             // 当前列显示完成，显示下一列
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/// @brief      向缓存写入字符串
///
/// @param      x           ：X 坐标，取值范围 0 ~ UI_FRAME_DATA_WIDTH - 1
/// @param      y           ：Y 坐标，取值范围 0 ~ UI_FRAME_DATA_HEIGHT - 1
/// @param      pucChar     ：要显示的字符串首地址指针
/// @param      emFontSize  ：字号，见 emOledFontSizeTdf 定义
/// @param      emDevNum    ：设备号
/// @param      emMode      : 显示模式，见 emUiPixelShowModeTdf 定义
///
/// @note
static void s_vUiWriteStringToBuffer(uint16_t x, uint16_t y, const uint8_t *c_pucChar, emUiFontSizeTdf emFontSize, emUiPixelShowModeTdf emMode)
{
    while((*c_pucChar >= ' ') && (*c_pucChar <= '~'))           // 判断字符是否合法
    {
        s_vUiWriteOneCharToBuffer(x, y, *c_pucChar, emFontSize, emMode);
        
        x += emFontSize >> 1;
        
        if(x > (UI_FRAME_DATA_WIDTH - emFontSize))              // 换行
        {
            x  = 0;
            y += 2;
        }
        
        c_pucChar++;
    }
}

/// @brief      计算幂（m ^ n）
///
/// @param      m   ：底数
/// @param      n   ：指数
///
/// @note
static uint32_t s_ulUiPow(uint8_t m, uint8_t n)
{
    uint32_t ulResult = 1;
    
    while(n--)
    {
        ulResult *= m;
    }
    
    return ulResult;
}

/// @brief      显示整数，带前导零
///
/// @param      x           ：X 坐标，取值范围 0 ~ UI_FRAMR_DATA_WIDTH - 1
/// @param      y           ：Y 坐标，取值范围 0 ~ UI_FRAME_DATA_HEIGHT - 1
/// @param      lNum        ：要显示的整数
/// @param      ucLength    ：要显示的长度
/// @param      emFontSize  ：字号，见 emOledFontSizeTdf 定义
/// @param      emMode      : 显示模式，见 emUiPixelShowModeTdf 定义
///
/// @note       如果整数为负数，则会强制将符号显示到最前一位
void vUiShowIntWithFrontZero(uint16_t x, uint16_t y, int32_t lNum, uint8_t ucLength, emUiFontSizeTdf emFontSize, emUiPixelShowModeTdf emMode)
{
    uint8_t i, ucTemp;
    
    for(i = 0; i < ucLength; i++)
    {
        ucTemp = (lNum / s_ulUiPow(10, ucLength - i - 1)) % 10;
        
        s_vUiWriteOneCharToBuffer(x + (emFontSize >> 1) * i, y, ucTemp + '0', emFontSize, emMode);
    }
    
    if(lNum < 0)
    {
        s_vUiWriteOneCharToBuffer(x, y, '-', emFontSize, emMode);
    }
}

/// @brief      显示整数，不带前导零
///
/// @param      ucX         ：X 坐标，取值范围 0 ~ OLED_POINT_WIDTH - 1
/// @param      ucY         ：Y 坐标，取值范围 0 ~ OLED_POINT_HEIGHT - 1
/// @param      lNum        ：要显示的整数
/// @param      ucLength    ：要显示的长度
/// @param      emFontSize  ：字号，见 emOledFontSizeTdf 定义
/// @param      emMode      : 显示模式，见 emUiPixelShowModeTdf 定义
///
/// @note       如果整数为负数，则会强制将符号显示到最前一位
void vUiShowIntWithoutFrontZero(uint16_t x, uint16_t y, int32_t lNum, uint8_t ucLength, emUiFontSizeTdf emFontSize, emUiPixelShowModeTdf emMode)
{
    uint8_t i, ucTemp;
    
    for(i = 0; i < ucLength; i++)
    {
        ucTemp = (lNum / s_ulUiPow(10, ucLength - i - 1)) % 10;
        
        if((lNum / s_ulUiPow(10, ucLength - i - 1)) == 0)
        {
            if(i != (ucLength - 1))
            {
                s_vUiWriteOneCharToBuffer(x + (emFontSize >> 1) * i, y, ' ', emFontSize, emMode);
            }
            else
            {
                s_vUiWriteOneCharToBuffer(x + (emFontSize >> 1) * i, y, '0', emFontSize, emMode);
            }
        }
        else
        {
            s_vUiWriteOneCharToBuffer(x + (emFontSize >> 1) * i, y, ucTemp + '0', emFontSize, emMode);
        }
    }
    
    if(lNum < 0)
    {
        s_vUiWriteOneCharToBuffer(x, y, '-', emFontSize, emMode);
    }
}

/// @brief      更新文本框到显存
///
/// @param      pstWidget   : 文本框对象首地址
///
/// @note
static void s_vUpdateTextBoxToFrameBuffer(stUiWidgetNodeTdf *pstWidget, emUiPixelShowModeTdf emMode)
{
    stUiTextBoxTdf *pstTextBox = pstWidget->pvWidget;
    
    s_vUiWriteStringToBuffer(pstTextBox->x, pstTextBox->y, pstTextBox->pucData, pstTextBox->emSize, emMode);
}

/// @brief      更新菜单框到显存
///
/// @param      pstWidget   : 菜单框对象首地址
///
/// @note
static void s_vUpdateMenuBoxToFrameBuffer(stUiWidgetNodeTdf *pstWidget, emUiPixelShowModeTdf emMode)
{
    uint16_t i;
    stUiMenuBoxTdf *pstMenuBox = pstWidget->pvWidget;
    
    for(i = 0; i < pstMenuBox->ucShowLineNum; i++)
    {
        s_vUiWriteStringToBuffer(pstMenuBox->x, 
                                 pstMenuBox->y + (i * pstMenuBox->emSize + pstMenuBox->ucLineSpacing), 
                                 pstMenuBox->pstData[i + pstMenuBox->usCurrentSelectIndex - pstMenuBox->ucCurrentSelectPosition].pucString, 
                                 pstMenuBox->emSize,
                                 emMode);
    }
    
    s_vUiWriteStringToBuffer(pstMenuBox->x, 
                             pstMenuBox->y + (pstMenuBox->ucCurrentSelectPosition * pstMenuBox->emSize + pstMenuBox->ucLineSpacing), 
                             pstMenuBox->pucSelectSign,
                             pstMenuBox->emSize,
                             emMode);
}

/// @brief      更新控件到显存
///
/// @param      pstWidget   : 控件对象首地址
///
/// @note
static void s_vUpdateWidgetToFrameBuffer(stUiWidgetNodeTdf *pstWidget)
{
    switch(pstWidget->emType)
    {
        case emUiWidgetType_TextBox:
        {
            s_vUpdateTextBoxToFrameBuffer(pstWidget, ((stUiTextBoxTdf *)pstWidget->pvWidget)->emMode);
            break;
        }
        case emUiWidgetType_MenuBox:
        {
            s_vUpdateMenuBoxToFrameBuffer(pstWidget, ((stUiMenuBoxTdf *)pstWidget->pvWidget)->emMode);
            break;
        }
        default:
        {
            break;
        }
    }
}

/// @brief      更新页面到显存
///
/// @param      pstFrame   : 页面对象首地址
///
/// @note       将所有控件依此更新到显存内
static void s_vUpdateFrameToFrameBuffer(stUiFrameTdf *pstFrame)
{
    stUiWidgetNodeTdf *pstNode;
    
    for(pstNode = pstFrame->pstWidgetNodeHead; pstNode != 0; pstNode = pstNode->pstNextNode)
    {
        s_vUpdateWidgetToFrameBuffer(pstNode);
    }
}

/// @brief      更新 UI 对象到显存
///
/// @param      pstUi       : UI 对象首地址
///
/// @note       将 UI 对象更新到显存内
void vUpdateUiObjectToFrameBuffer(stUiObjectTdf *pstUi)
{
    s_vUpdateFrameToFrameBuffer(pstUi->p2stFrame[pstUi->ulCurrentFrameIndex]);
}

/// @brief      执行 UI 页面的回调函数
///
/// @param      pstUi       : UI 对象首地址
/// @param      p           : 参数首地址
///
/// @note
void vUiFrameCallbackExecute(stUiObjectTdf *pstUi, void *p)
{
    pstUi->p2stFrame[pstUi->ulCurrentFrameIndex]->pvCallback(p);
}

/// @brief      菜单框执行选项减 1 操作
///
/// @param      pstMenuBox  : 菜单框对象首地址
///
/// @note
void vUiMenuBoxExecuteSelectDec(stUiMenuBoxTdf *pstMenuBox)
{
    if((pstMenuBox->usCurrentSelectIndex == 0) && 
       (pstMenuBox->ucCurrentSelectPosition == 0))
    {
        pstMenuBox->ucCurrentSelectPosition = pstMenuBox->ucShowLineNum - 1;
        pstMenuBox->usCurrentSelectIndex = pstMenuBox->ucMaxLineNum - 1;
        return;
    }
    
    if(pstMenuBox->ucCurrentSelectPosition == 0)
    {
        pstMenuBox->usCurrentSelectIndex--;
        return;
    }
    
    pstMenuBox->usCurrentSelectIndex--;
    pstMenuBox->ucCurrentSelectPosition--;
}

/// @brief      菜单框执行选项加 1 操作
///
/// @param      pstMenuBox  : 菜单框对象首地址
///
/// @note
void vUiMenuBoxExecuteSelectInc(stUiMenuBoxTdf *pstMenuBox)
{
    if((pstMenuBox->usCurrentSelectIndex == (pstMenuBox->ucMaxLineNum - 1)) && 
       (pstMenuBox->ucCurrentSelectPosition == (pstMenuBox->ucShowLineNum - 1)))
    {
        pstMenuBox->ucCurrentSelectPosition = 0;
        pstMenuBox->usCurrentSelectIndex = 0;
        return;
    }
    
    if(pstMenuBox->ucCurrentSelectPosition == (pstMenuBox->ucShowLineNum - 1))
    {
        pstMenuBox->usCurrentSelectIndex++;
        return;
    }
    
    pstMenuBox->usCurrentSelectIndex++;
    pstMenuBox->ucCurrentSelectPosition++;
}


