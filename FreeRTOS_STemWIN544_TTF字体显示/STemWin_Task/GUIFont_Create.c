/**
  ******************************************************************************
  * @file    GUIFont_Create.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   TTF格式字体emwin函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
//#include "./flash/bsp_spi_flash.h"
#include "GUIFont_Create.h"
#include "ff.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>


/* 存储器初始化标志 */
static uint8_t Storage_Init_Flag = 0;

/* 字库属性结构体 */
GUI_TTF_CS cs0, cs1, cs2, cs3, cs4;

/* 字库数据结构体 */
GUI_TTF_DATA ttf_data;

/* 定义emwin字体 */
GUI_FONT FONT_TTF_24;
GUI_FONT FONT_TTF_48;
GUI_FONT FONT_TTF_72;
GUI_FONT FONT_TTF_96;
GUI_FONT FONT_TTF_120;

/* 字库数据缓冲区 */
char *TTFfont_buffer;
GUI_HMEM hFontMem;

/* 字库存储路径 */
static const char FONT_STORAGE_ROOT_DIR[] = "0:";
static const char FONT_TTF_ADDR[] = "0:/Font/DroidSansFallbackFull.ttf";

/* 字库存储在文件系统时需要使用的变量 */
static FIL fnew;									/* file objects */
static FATFS fs;									/* Work area (file system object) for logical drives */
static FRESULT res; 
static UINT br;            				/* File R/W count */


/**
  * @brief  获取字体数据
  * @note 无
  * @param res_name：要读取的文件名
  * @retval 无
  */
static void FONT_TTF_GetData(const char *res_name)
{
  if (Storage_Init_Flag == 0)
	{
		/* 挂载sd卡文件系统 */
		res = f_mount(&fs,FONT_STORAGE_ROOT_DIR,1);
		Storage_Init_Flag = 1;
	}
  
  /* 打开文件 */
  taskENTER_CRITICAL();
  res = f_open(&fnew , res_name, FA_OPEN_EXISTING | FA_READ);
  taskEXIT_CRITICAL();
  if(res != FR_OK)
	{
		printf("无法找到字库文件\r\n");
		while(1);
	}
  
  /* 申请一块动态内存空间 */
	hFontMem = GUI_ALLOC_AllocZero(fnew.fsize);
	/* 转换动态内存的句柄为指针 */
	TTFfont_buffer = GUI_ALLOC_h2p(hFontMem);
  
  /* 读取内容 */
  taskENTER_CRITICAL();
	res = f_read( &fnew, TTFfont_buffer, fnew.fsize, &br );
  taskEXIT_CRITICAL();
  if(res != FR_OK)
	{
		printf("无法读取字库文件\r\n");
		while(1);
	}
  f_close(&fnew);
  
  /* 配置属性 */
  ttf_data.pData = TTFfont_buffer;
  ttf_data.NumBytes = fnew.fsize;
  
  /* 配置字体参数 */
  cs0.pTTF = &ttf_data;
  cs0.PixelHeight = 24;
  cs0.FaceIndex = 0;
  
  cs1.pTTF = &ttf_data;
  cs1.PixelHeight = 48;
  cs1.FaceIndex = 0;
  
  cs2.pTTF = &ttf_data;
  cs2.PixelHeight = 72;
  cs2.FaceIndex = 0;
  
  cs3.pTTF = &ttf_data;
  cs3.PixelHeight = 96;
  cs3.FaceIndex = 0;
  
  cs4.pTTF = &ttf_data;
  cs4.PixelHeight = 120;
  cs4.FaceIndex = 0;
}

/**
  * @brief  创建TTF字体
  * @note 无
  * @param 无
  * @retval 无
  */
void Create_TTF_Font(void)
{
  /* 获取字体数据 */
  FONT_TTF_GetData(FONT_TTF_ADDR);
  
  /* 创建TTF字体 */
  GUI_TTF_CreateFontAA(&FONT_TTF_24,
                       &cs0);
  GUI_TTF_CreateFontAA(&FONT_TTF_48,
                       &cs1);
  GUI_TTF_CreateFontAA(&FONT_TTF_72,
                       &cs2);
  GUI_TTF_CreateFontAA(&FONT_TTF_96,
                       &cs3);
  GUI_TTF_CreateFontAA(&FONT_TTF_120,
                       &cs4);
}
