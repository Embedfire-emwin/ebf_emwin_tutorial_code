/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.44                          *
*        Compiled Nov 10 2017, 08:53:57                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "GUIFont_Create.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_TEXT_0   (GUI_ID_USER + 0x01)
#define ID_TEXT_1   (GUI_ID_USER + 0x02)
#define ID_MULTIEDIT_0   (GUI_ID_USER + 0x03)
#define ID_BUTTON_0   (GUI_ID_USER + 0x04)
#define ID_BUTTON_1   (GUI_ID_USER + 0x05)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
extern const char Framewin_text[];
extern const char text[];
extern const char MULTIEDIT_text[];
//extern const char *BUTTON_text[];


/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 240, 320, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 230, 55, 0, 0x64, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 0, 50, 230, 108, 0, 0x64, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 5, 120, 220, 110, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 5, 240, 100, 25, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 125, 240, 100, 25, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/**
  * @brief 对话框回调函数
  * @note 无
  * @param pMsg：消息指针
  * @retval 无
  */
static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			/* 初始化Framewin控件 */
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleHeight(hItem, 32);
			FRAMEWIN_SetText(hItem, Framewin_text);
			FRAMEWIN_SetFont(hItem, &FONT_SIYUANHEITI_20_4BPP);
			/* 初始化TEXT0 */
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetText(hItem, text);
			TEXT_SetFont(hItem, &FONT_XINSONGTI_16_4BPP);	
			/* 初始化TEXT1 */
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetText(hItem, text);
			TEXT_SetFont(hItem, &FONT_SIYUANHEITI_20_4BPP);
			/* 初始化MULTIEDIT0 */
			hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
			MULTIEDIT_SetReadOnly(hItem, 1);
			MULTIEDIT_SetBufferSize(hItem, 200);
			MULTIEDIT_SetWrapWord(hItem);
			MULTIEDIT_SetText(hItem, MULTIEDIT_text);
			MULTIEDIT_SetFont(hItem, &FONT_SIYUANHEITI_20_4BPP);
			MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_READONLY, GUI_GREEN);
			MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_READONLY, GUI_BLACK);
			MULTIEDIT_ShowCursor(hItem, 0);
//			/* 初始化Button0 */
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
//			BUTTON_SetFont(hItem, &FONT_SIYUANHEITI_20_4BPP);
//			BUTTON_SetText(hItem, BUTTON_text[0]);
//			/* 初始化Button1 */
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//			BUTTON_SetFont(hItem, &FONT_SIYUANHEITI_20_4BPP);
//			BUTTON_SetText(hItem, BUTTON_text[1]);
//			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id)
			{
				case ID_MULTIEDIT_0: // Notifications sent by 'Multiedit'
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;
				case ID_BUTTON_0: // Notifications sent by 'Button'
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
				case ID_BUTTON_1: // Notifications sent by 'Button'
					switch(NCode)
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
					}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/**
  * @brief 以对话框方式间接创建控件
  * @note 无
  * @param 无
  * @retval hWin：资源表中第一个控件的句柄
  */
WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void)
{
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/**
  * @brief GUI主任务
  * @note 无
  * @param 无
  * @retval 无
  */
void MainTask(void)
{
	/* 启用UTF-8编码 */
	GUI_UC_SetEncodeUTF8();
	/* 创建字体 */
	Create_SIF_Font();
	/* 创建窗口 */
	CreateFramewin();
	while(1)
	{
		GUI_Delay(100);
	}
}

/*************************** End of file ****************************/
