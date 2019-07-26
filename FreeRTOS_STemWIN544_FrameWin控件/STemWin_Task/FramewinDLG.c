/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2018  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.48 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_FrameWin.c
Purpose     : Example demonstrating the use of a FRAMEWIN widget
Requirements: WindowManager - (x)
              MemoryDevices - ( )
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/
#include <stddef.h>
#include <string.h>
/* STemWIN头文件 */
#include "FramewinDLG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SPEED   1200

#define MSG_CHANGE_MAIN_TEXT (WM_USER + 0)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static FRAMEWIN_Handle  _hFrame;
static WM_CALLBACK*     _pcbOldFrame;
static char             _acMainText[100];
static int              _LockClose = 1;

/*******************************************************************
*
*       Static code
*
********************************************************************
*/
/**
  * @brief _ChangeMainText
  * @note 将消息发送到桌面窗口并使其无效，用来在桌面窗口上显示新内容
  * @param pStr：字符串
	*        Delay：延时时间
  * @retval 无
  */
static void _ChangeMainText(char* pStr, int Delay) {
  WM_MESSAGE Message;
  Message.MsgId  = MSG_CHANGE_MAIN_TEXT;
  Message.Data.p = pStr;
  GUI_Delay(Delay);
  WM_SendMessage(WM_HBKWIN, &Message);
  WM_InvalidateWindow(WM_HBKWIN);
  GUI_Delay(Delay/3);
}

/**
  * @brief 框架窗口的子窗口回调函数
  * @note 无
  * @param pMsg：消息指针：消息指针
  * @retval 无
  */
static void _cbChild(WM_MESSAGE * pMsg) {
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
    GUI_Clear();
    GUI_DispStringHCenterAt("Client window", 
                            WM_GetWindowSizeX(hWin) / 2, 
                            WM_GetWindowSizeY(hWin) / 2);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief Framewin控件回调函数
  * @note 无
  * @param pMsg：消息指针：消息指针
  * @retval 无
  */
static void _cbFrame(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) {
      int Id = WM_GetId(pMsg->hWinSrc);      // Id of widget
      if (Id == GUI_ID_CLOSE) {
        if (_LockClose) {
          return;
        }
        _hFrame = 0;
      }
    }
    break;
  }
  if (_pcbOldFrame) {
    (*_pcbOldFrame)(pMsg);
  }
}

/**
  * @brief 背景窗口回调函数
  * @note 用来在移动框架窗口时重绘背景
  * @param pMsg：消息指针：消息指针
  * @retval 无
  */
static void _cbBkWindow(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case MSG_CHANGE_MAIN_TEXT:
      strcpy(_acMainText, (char const *)pMsg->Data.p);
      WM_InvalidateWindow(pMsg->hWin);
      break;
    case WM_PAINT:
      GUI_SetBkColor(GUI_BLACK);
      GUI_Clear();
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_Font24_ASCII);
      GUI_DispStringHCenterAt("WIDGET_FrameWin - Sample", 160, 5);
      GUI_SetFont(&GUI_Font8x16);
      GUI_DispStringHCenterAt(_acMainText, 160, 40);
      GUI_SetFont(&GUI_Font6x8);
      GUI_DispStringHCenterAt("The function FRAMEWIN_Create creates both the\n"
                              "frame window and the client window.", 160, 190);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

/**
  * @brief Framewin控件的API演示函数
  * @note 无
  * @param 无
  * @retval 无
  */
static void _DemoFramewin(void) {
  int i;
  char acInfoText[] = "-- sec to play with window";
  WM_HWIN hChild;
 
	/* 重定向背景窗口回调函数 */
  WM_SetCallback(WM_HBKWIN, _cbBkWindow);
  /* 创建和配置框架窗口 */
  _ChangeMainText("FRAMEWIN_Create", SPEED);
  _hFrame = FRAMEWIN_Create("Frame window", 0, WM_CF_SHOW, 50, 75, 220, 100);
  _pcbOldFrame = WM_SetCallback(_hFrame, _cbFrame);
  hChild = WM_GetClientWindow(_hFrame);
  WM_SetCallback(hChild, _cbChild);
  FRAMEWIN_SetMoveable(_hFrame, 1);
  /* 创建按钮 */
  FRAMEWIN_AddCloseButton(_hFrame, FRAMEWIN_BUTTON_LEFT, 0);
  FRAMEWIN_AddMaxButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0);
  FRAMEWIN_AddMinButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 2);
  /* 修改框架窗口属性 */
  _ChangeMainText("FRAMEWIN_SetActive", SPEED);
  FRAMEWIN_SetActive(_hFrame, 1);
  _ChangeMainText("FRAMEWIN_SetFont", SPEED);
  FRAMEWIN_SetFont(_hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTitleHeight(_hFrame, 20);
  _ChangeMainText("FRAMEWIN_SetTextColor", SPEED);
  FRAMEWIN_SetTextColor(_hFrame, GUI_YELLOW);
  _ChangeMainText("FRAMEWIN_SetTextAlign", SPEED);
  FRAMEWIN_SetTextAlign(_hFrame, GUI_TA_HCENTER);
  _ChangeMainText("FRAMEWIN_Minimize", SPEED);
  FRAMEWIN_Minimize(_hFrame);
  _ChangeMainText("FRAMEWIN_Maximize", SPEED);
  FRAMEWIN_Maximize(_hFrame);
  _ChangeMainText("FRAMEWIN_Restore", SPEED);
  FRAMEWIN_Restore(_hFrame);
  _ChangeMainText("FRAMEWIN_SetTitleVis", SPEED);
  for (i = 0; i < 5; i++) {
    FRAMEWIN_SetTitleVis(_hFrame, 0);
    GUI_Delay(200);
    FRAMEWIN_SetTitleVis(_hFrame, 1);
    GUI_Delay(200);
  }
  //
  // Time to play with frame window
  //
  _LockClose = 0;
  for (i = 250; (i > 0) && _hFrame; i--) {
    acInfoText[0] = '0' + ((i + 9) / 100);
    acInfoText[1] = '0' + (((i + 9) / 10) % 10);
    _ChangeMainText(acInfoText, 0);
    GUI_Delay(100);
  }
  if (_hFrame) {
    _ChangeMainText("WM_Delete", SPEED);
    WM_DeleteWindow(_hFrame);
  } else {
    _ChangeMainText("", 50);
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/**
  * @brief GUI主任务
  * @note 无
  * @param 无
  * @retval 无
  */
void MainTask(void) {

  while(1) {
  	_DemoFramewin();
  }
}

/*************************** End of file ****************************/
