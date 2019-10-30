/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
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
#include <stdio.h>

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
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
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
/* FATFS */
#include "ff.h"
#include "diskio.h"
#include "integer.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
/* Key任务句柄 */
static TaskHandle_t Key_Task_Handle = NULL;
/* Touch任务句柄 */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI任务句柄 */
static TaskHandle_t GUI_Task_Handle = NULL;

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
 
/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
KEY Key1,Key2;

FATFS   fs;								/* FatFs文件系统对象 */
FIL     file;							/* file objects */
UINT    bw;            		/* File R/W count */
FRESULT result; 
FILINFO fno;
DIR dir;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);

static void LED_Task(void* parameter);
static void Key_Task(void* parameter);
static void Touch_Task(void* parameter);
static void GUI_Task(void* parameter);

static void BSP_Init(void);
static void AP6181_PDN_INIT(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	/* 开发板硬件初始化 */
	BSP_Init();
	
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* 任务入口函数 */
											 (const char*    )"AppTaskCreate",/* 任务名称 */
											 (uint16_t       )512,					/* 任务栈大小 */
											 (void*          )NULL,					/* 任务入口函数参数 */
											 (UBaseType_t    )1,						/* 任务的优先级 */
											 (TaskHandle_t   )&AppTaskCreate_Handle);/* 任务控制块指针 */
	/* 启动任务调度 */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* 启动任务，开启调度 */
	else
		return -1;
	
	while(1);/* 正常不会执行到这里 */
}

/**
  * @brief 任务创建函数
  * @note 为了方便管理，所有的任务创建都放在这个函数里面
  * @param 无
  * @retval 无
  */
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	taskENTER_CRITICAL();//进入临界区

	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* 任务入口函数 */
											 (const char*    )"LED_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )5,         /* 任务的优先级 */
											 (TaskHandle_t   )&LED_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LED1_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Key_Task,/* 任务入口函数 */
											 (const char*      )"Key_Task",/* 任务名称 */
											 (uint16_t         )256,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )4,       /* 任务的优先级 */
											 (TaskHandle_t     )&Key_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Key_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* 任务入口函数 */
											 (const char*      )"Touch_Task",/* 任务名称 */
											 (uint16_t         )256,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )3,       /* 任务的优先级 */
											 (TaskHandle_t     )&Touch_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Touch_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* 任务入口函数 */
											 (const char*      )"GUI_Task",/* 任务名称 */
											 (uint16_t         )4096,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )2,         /* 任务的优先级 */
											 (TaskHandle_t     )&GUI_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GUI_Task任务成功！\r\n");
	
	vTaskDelete(AppTaskCreate_Handle);//删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief LED任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void LED_Task(void* parameter)
{
	while(1)
	{
		LED3_TOGGLE;
		vTaskDelay(1000);
	}
}

/**
  * @brief 按键任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void Key_Task(void* parameter)
{
	while(1)
	{
		Key_RefreshState(&Key1);//刷新按键状态
		Key_RefreshState(&Key2);//刷新按键状态
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED2_TOGGLE;
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//按键被按下过
		{
			LED1_TOGGLE;
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}
		vTaskDelay(50);
	}
}

/**
  * @brief 触摸检测任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void Touch_Task(void* parameter)
{
	while(1)
	{
		GT9xx_GetOnePiont();
		vTaskDelay(20);
	}
}

/**
  * @brief GUI任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void GUI_Task(void* parameter)
{
	/* 初始化GUI */
	GUI_Init();
	/* 开启三缓冲 */
	WM_MULTIBUF_Enable(1);
	while(1)
	{
		MainTask();
	}
}

/**
  * @brief 板级外设初始化
  * @note 所有板子上的初始化均可放在这个函数里面
  * @param 无
  * @retval 无
  */
static void BSP_Init(void)
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
  /* 禁用WiFi模块 */
	AP6181_PDN_INIT();
  /* 挂载文件系统，挂载时会对SD卡初始化 */
  result = f_mount(&fs,"0:",1);
	if(result != FR_OK)
	{
		printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\n");
		while(1);
	}
}

/**
  * @brief AP6181_PDN_INIT
  * @note 禁止WIFI模块
  * @param 无
  * @retval 无
  */
static void AP6181_PDN_INIT(void)
{
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //禁用WiFi模块
}

/*********************************************END OF FILE**********************/

