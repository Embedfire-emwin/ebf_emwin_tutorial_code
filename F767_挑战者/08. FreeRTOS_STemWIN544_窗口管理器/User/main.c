/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   电容触摸屏-触摸画板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32F767 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f7xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include <stdlib.h>
#include "main.h"
#include "./touch/gt9xx.h"
#include "./mpu/bsp_mpu.h" 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
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
SemaphoreHandle_t ScreenShotSem_Handle = NULL;
 
/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);

static void LED_Task(void* parameter);
static void Touch_Task(void* parameter);
static void GUI_Task(void* parameter);

static void BSP_Init(void);
static void WIFI_PDN_INIT(void);
extern void MainTask(void);

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
											 (TaskHandle_t   )&AppTaskCraete_Handle);/* 任务控制块指针 */
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
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* 任务入口函数 */
											 (const char*      )"Touch_Task",/* 任务名称 */
											 (uint16_t         )256,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )4,       /* 任务的优先级 */
											 (TaskHandle_t     )&Touch_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Touch_Task任务成功！\r\n");
  
  xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* 任务入口函数 */
											 (const char*      )"GUI_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&GUI_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GUI_Task任务成功！\r\n");
	
	vTaskDelete(AppTaskCraete_Handle);//删除AppTaskCreate任务
	
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
  
  GUI_SetBkColor(GUI_BLACK);
  GUI_Clear();
  
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
  SCB_EnableICache();
  SCB_EnableDCache();

  Board_MPU_Config(0, MPU_Normal_WT, 0x20000000, MPU_REGION_SIZE_128KB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x20020000, MPU_REGION_SIZE_512KB);
  /* 如果配置为WT，则在使用Alpha混合时需要手动清空D-Cache，但RGB565下清空操作无效 */
  Board_MPU_Config(2, MPU_Normal_NonCache, 0xD0000000, MPU_REGION_SIZE_32MB);
  
  HAL_Init();
  
	/* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
  __HAL_RCC_CRC_CLK_ENABLE();
	
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用同一个优先级分组，千万不要再分组，切记。
	 */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  
	/* 系统时钟初始化成216MHz */
	SystemClock_Config();
	/* LED 端口初始化 */
	WIFI_PDN_INIT();
	LED_GPIO_Config();
	/* 配置串口1为：115200 8-N-1 */
	DEBUG_USART_Config();		
	printf("\r\n欢迎使用野火STM32F767开发板。\r\n");		 
  /* 初始化触摸屏 */
  GTP_Init_Panel(); 
  /* 初始化SDRAM 用作LCD 显存*/
	SDRAM_Init();
	/* LCD 端口初始化 */ 
	LCD_Init();
}

/**
	**************************************************************
	* Description : 初始化WiFi模块使能引脚，并禁用WiFi模块
	* Argument(s) : none.
	* Return(s)   : none.
	**************************************************************
	*/
static void WIFI_PDN_INIT(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*使能引脚时钟*/	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/*选择要控制的GPIO引脚*/															   
	GPIO_InitStruct.Pin = GPIO_PIN_13;	
	/*设置引脚的输出类型为推挽输出*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;      
	/*设置引脚为上拉模式*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	/*设置引脚速率为高速 */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; 
	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
	/*禁用WiFi模块*/
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);  
}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下 : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  无
  * @retval 无
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* 使能HSE，配置HSE为PLL的时钟源，配置PLL的各种分频因子M N P Q 
	 * PLLCLK = HSE/M*N/P = 25M / 25 *432 / 2 = 216M
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* 激活 OverDrive 模式以达到216M频率  */  
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* 选择PLLCLK作为SYSCLK，并配置 HCLK, PCLK1 and PCLK2 的时钟分频因子 
	 * SYSCLK = PLLCLK     = 216M
	 * HCLK   = SYSCLK / 1 = 216M
	 * PCLK2  = SYSCLK / 2 = 108M
	 * PCLK1  = SYSCLK / 4 = 54M
	 */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}  
}

/*********************************************END OF FILE**********************/

