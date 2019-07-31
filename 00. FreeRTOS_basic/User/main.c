/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
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
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"

/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;

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


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);
static void LED1_Task(void* parameter);
static void LED2_Task(void* parameter);
static void BSP_Init(void);

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
	
	xReturn = xTaskCreate((TaskFunction_t)LED1_Task,/* 任务入口函数 */
											 (const char*    )"LED1_Task",/* 任务名称 */
											 (uint16_t       )512,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )2,         /* 任务的优先级 */
											 (TaskHandle_t   )&LED1_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LED1_Task任务成功！\r\n");
	
		xReturn = xTaskCreate((TaskFunction_t)LED2_Task,/* 任务入口函数 */
											 (const char*      )"LED2_Task",/* 任务名称 */
											 (uint16_t         )512,       /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&LED2_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LED1_Task任务成功！\r\n");
	
	vTaskDelete(AppTaskCraete_Handle);//删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief LED1_Task
  * @note LED1_Task任务主主体
  * @param 无
  * @retval 无
  */
static void LED1_Task(void* parameter)
{
	while(1)
	{
		LED1_TOGGLE;
		vTaskDelay(500);
	}
}

/**
  * @brief LED2_Task
  * @note LED2_Task任务主主体
  * @param 无
  * @retval 无
  */
static void LED2_Task(void* parameter)
{
	while(1)
	{
		LED2_TOGGLE;
		vTaskDelay(1000);
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
}
/*********************************************END OF FILE**********************/

