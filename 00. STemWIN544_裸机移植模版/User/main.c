/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   STemWIN 裸机工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * 包含的头文件
 ******************************************************************************/
/* 开发板硬件bsp头文件 */
#include "./led/bsp_led.h" 
#include "./beep/bsp_beep.h" 
#include "./usart/bsp_debug_usart.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt9xx.h"
#include "./key/bsp_key.h"
#include "./sdram/bsp_sdram.h"
#include "./systick/bsp_Systick.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "ScreenShot.h"
#include "MainTask.h"

 /*
 *************************************************************************
 *                             全局变量声明
 *************************************************************************
 */
KEY Key1,Key2;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	
  /*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用同一个优先级分组，千万不要再分组，切记。
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* 启动系统滴答定时器 */
  SysTick_Init();
	/* LED 初始化 */
	LED_GPIO_Config();
	/* 串口初始化	*/
	Debug_USART_Config();
	/* 蜂鸣器初始化 */
	Beep_GPIO_Config();
	/* 按键初始化 */
	Key1_GPIO_Config();
	Key2_GPIO_Config();
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
	/* 触摸屏初始化 */
	GTP_Init_Panel();	
	/* SDRAM初始化 */
	SDRAM_Init();
	/* LCD初始化 */
	LCD_Init();
  
  /* 初始化GUI */
	GUI_Init();
	/* 开启三缓冲 */
	WM_MULTIBUF_Enable(1);
  
  while(1)
  {
    MainTask();
  }
}

///**
//  * @brief 按键任务主体
//  * @note 无
//  * @param 无
//  * @retval 无
//  */
//static void Key_Task(void* parameter)
//{
//	while(1)
//	{
//		Key_RefreshState(&Key1);//刷新按键状态
//		Key_RefreshState(&Key2);//刷新按键状态
//		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//按键被按下过
//		{
//			LED2_TOGGLE;
//			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
//		}
//		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//按键被按下过
//		{
//			LED1_TOGGLE;
//			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
//		}
//		vTaskDelay(50);
//	}
//}


/*********************************************END OF FILE**********************/

