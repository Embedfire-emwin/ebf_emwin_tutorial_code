/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"  
#include "./sram/bsp_sram.h"	  
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./systick/bsp_SysTick.h"
#include "./touch/gt5xx.h"

#include "GUI.h"

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{
  //初始化外部SRAM  
  FSMC_SRAM_Init();
  
  /*CRC和emWin没有关系，只是他们为了库的保护而做的，这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  
  /* LED初始化 */
	LED_GPIO_Config();

	/* 配置串口1为：115200 8-N-1 */
	Debug_USART_Config();
  
  /* 初始化GUI */
	GUI_Init();
  
  /* 触摸屏初始化 */
  GTP_Init_Panel();
  
  /* 初始化定时器 */
	SysTick_Init();
  
  GUI_CURSOR_Show();
  
  printf("\r\n ********** emwin DEMO *********** \r\n");
  
  
  while(1)
  {
    MainTask();
  }
}


/*********************************************END OF FILE**********************/

