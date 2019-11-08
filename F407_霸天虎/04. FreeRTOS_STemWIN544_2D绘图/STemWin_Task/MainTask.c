/**
  *********************************************************************
  * @file    MainTask.c
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
#include "MainTask.h"

#include <stdlib.h>

#include "./usart/bsp_debug_usart.h"

/*******************************************************************************
 * 全局变量
 ******************************************************************************/
GUI_RECT BasicRect = {10, 10, 100, 105};
static const unsigned aValues[] = {100, 135, 190, 240, 340, 360};
static const GUI_COLOR aColor[] = {GUI_BLUE, GUI_GREEN, GUI_RED,
                                   GUI_CYAN, GUI_MAGENTA, GUI_YELLOW};
static const char QR_TEXT[] = "EmbedFire";
static const GUI_POINT _aPointArrow[] = {
  {  0,   0 },
  {-40, -30 },
  {-10, -20 },
  {-10, -70 },
  { 10, -70 },
  { 10, -20 },
  { 40, -30 },
};
static const GUI_POINT DashCube_BackPoint[] = {
		{ 76 , 104 },
		{ 176, 104 },
		{ 176,   4 },
		{  76,   4 }
};
static const GUI_POINT DashCube_LeftPoint[] = {
		{ 40, 140 },
		{ 76, 104 },
		{ 76,   4 },
		{ 40,  40 }
};
static const GUI_POINT DashCube_BottonPoint[] = {
		{  40, 140 },
		{ 140, 140 },
		{ 176, 104 },
		{  76, 104 }
};
static const GUI_POINT DashCube_TopPoint[] = {
		{  40, 40 },
		{ 140, 40 },
		{ 176,  4 },
		{  76,  4 },
};
static const GUI_POINT DashCube_RightPoint[] = {
		{ 140, 140 },
		{ 176, 104 },
		{ 176,   4 },
		{ 140,  40 },
};
static const 	GUI_POINT DashCube_FrontPoint[] = {
		{  40, 140},
		{ 140, 140},
		{ 140,  40},
		{  40,  40},
};

/*******************************************************************************
 * 函数
 ******************************************************************************/
/**
  * @brief 饼图绘图函数
  * @note 无
  * @param x0：饼图圆心的x坐标
  *        y0：饼图圆心的y坐标
  *        r：饼图半径
  * @retval 无
  */
static void Pie_Chart_Drawing(int x0, int y0, int r)
{
	int i, a0 = 0, a1 = 0;
	
	for(i = 0; i < GUI_COUNTOF(aValues); i++)
	{
		if(i == 0) a0 = 0;
		else a0 = aValues[i - 1];
		a1 = aValues[i];	
		GUI_SetColor(aColor[i]);
		GUI_DrawPie(x0, y0, r, a0, a1, 0);
	}
}

/**
  * @brief 二维码生成
  * @note 无
  * @param pText：二维码内容
  *        PixelSize：二维码数据色块的大小，单位：像素
  *        EccLevel：纠错编码级别
  *        x0：二维码图像在LCD的坐标x
  *        y0：二维码图像在LCD的坐标y
  * @retval 无
  */
static void QR_Code_Drawing(const char *pText, int PixelSize, int EccLevel, int x0, int y0)
{
	GUI_HMEM hQR;
	
	/* 创建二维码对象 */
	hQR = GUI_QR_Create(pText, PixelSize, EccLevel, 0);
	/* 绘制二维码到LCD */
	GUI_QR_Draw(hQR, x0, y0);
	/* 删除二维码对象 */
	GUI_QR_Delete(hQR);
}

/**
  * @brief 2D绘图函数
  * @note 无
  * @param 无
  * @retval 无
  */
/* 用于存放多边形旋转后的点列表 */ 
GUI_POINT aArrowRotatedPoints[GUI_COUNTOF(_aPointArrow)];
static void _2D_Graph_Drawing(void)
{
	I16 aY[125] = {0};
	int i;
  float pi = 3.1415926L;
  float angle = 0.0f;
	
	/* 绘制各种矩形 */
	GUI_SetColor(GUI_GREEN);
	GUI_DrawRectEx(&BasicRect);
	BasicRect.x0 += 116;
	BasicRect.x1 += 116;
	GUI_FillRectEx(&BasicRect);
  GUI_SetColor(GUI_RED);
	GUI_DrawRoundedRect(240, 10, 330, 105, 10);
	GUI_DrawRoundedFrame(352, 10, 442, 105, 10, 10);
	
	GUI_FillRoundedRect(468-460, 140, 558-460, 235, 10);
	GUI_DrawGradientRoundedH(584-460, 140, 674-460, 235, 10, GUI_LIGHTMAGENTA, GUI_LIGHTCYAN);
	GUI_DrawGradientRoundedV(700-460, 140, 790-460, 235, 10, GUI_LIGHTMAGENTA, GUI_LIGHTCYAN);
	
	/* 绘制线条 */
	GUI_SetPenSize(10);
  GUI_SetColor(GUI_YELLOW);
	GUI_DrawLine(10, 140+110, 100, 240+110);
	
	/* 绘制多边形 */
	GUI_SetColor(GUI_RED);
	GUI_FillPolygon(_aPointArrow, 7, 190, 205+110);
  /* 旋转多边形 */
	angle = pi / 2;
	GUI_RotatePolygon(aArrowRotatedPoints,
	                  _aPointArrow, 
                    (sizeof(_aPointArrow) / sizeof(_aPointArrow[0])),
										angle);
	GUI_FillPolygon(&aArrowRotatedPoints[0], 7, 220, 250+110);
  
  /* 绘制线框正方体 */
  GUI_SetPenSize(1);
	GUI_SetColor(0x4a51cc);
	GUI_SetLineStyle(GUI_LS_DOT);
	GUI_DrawPolygon(DashCube_BackPoint, 4, 210, 145+100);
  GUI_DrawPolygon(DashCube_LeftPoint, 4, 210, 145+100);
  GUI_DrawPolygon(DashCube_BottonPoint, 4, 210, 145+100);
  GUI_SetPenSize(2);
  GUI_SetLineStyle(GUI_LS_SOLID);
  GUI_DrawPolygon(DashCube_TopPoint, 4, 210, 145+100);
  GUI_DrawPolygon(DashCube_RightPoint, 4, 210, 145+100);
  GUI_DrawPolygon(DashCube_FrontPoint, 4, 210, 145+100);
                    
	/* 绘制圆 */
	GUI_SetColor(GUI_LIGHTMAGENTA);
	for(i = 10; i <= 70; i += 10)
	{
		GUI_DrawCircle(100, 217+250, i);
	}
	GUI_SetColor(GUI_LIGHTCYAN);
	GUI_FillCircle(300, 217+250, 70);
	
	/* 绘制椭圆 */
	GUI_SetColor(GUI_BLUE);
	GUI_FillEllipse(80, 393+240, 50, 70);
	GUI_SetPenSize(2);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawEllipse(80, 393+240, 50, 10);
	
	/* 绘制圆弧 */
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_GRAY_3F);
	GUI_DrawArc(240, 393+240, 80, 80, -30, 210);
	
	/* 绘制折线图 */
	for(i = 0; i< GUI_COUNTOF(aY); i++)
	{
		aY[i] = rand() % 100;
	}
	GUI_SetColor(GUI_BLACK);
	GUI_DrawGraph(aY, GUI_COUNTOF(aY), 350, 340+240);
	
	/* 绘制饼图 */
	Pie_Chart_Drawing(120, 393+380, 60);
	
	/* 绘制二维码 */
	QR_Code_Drawing(QR_TEXT, 5, GUI_QR_ECLEVEL_L, 260, 330+390);
}

/**
  * @brief Alpha混合
  * @note 无
  * @param 无
  * @retval 无
  */
static void Alpha_Blending(void)
{
  /* 显示字符 */
	GUI_SetColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(GUI_FONT_24B_ASCII);
	GUI_DispStringHCenterAt("Alpha blending", 110, 700);
	GUI_DispStringHCenterAt("Not use blending",350, 700);

  /* 开启自动Alpha混合 */
  GUI_EnableAlpha(1);
	/* 将Alpha数值添加到颜色中并显示 */
	GUI_SetColor(GUI_MAKE_COLOR(0xC0uL << 24) | 0xFF0000);
	GUI_FillRect(20, 20, 235, 235);
	GUI_SetColor(GUI_MAKE_COLOR(0x80uL << 24) | 0x00FF00);
	GUI_FillRect(20, 245, 235, 465);
	GUI_SetColor(GUI_MAKE_COLOR(0x40uL << 24) | 0x0000FF);
	GUI_FillRect(20, 475, 235, 695);
  /* 关闭自动Alpha混合 */
  GUI_EnableAlpha(0);
	/* 不使用Alpha混合显示 */
	GUI_SetColor(GUI_MAKE_COLOR(0xC0uL << 24) | 0xFF0000);
	GUI_FillRect(250, 20, 465, 235);
	GUI_SetColor(GUI_MAKE_COLOR(0x80uL << 24) | 0x00FF00);
	GUI_FillRect(250, 245, 465, 465);
	GUI_SetColor(GUI_MAKE_COLOR(0x40uL << 24) | 0x0000FF);
	GUI_FillRect(250, 475, 465, 695);
	
	
}

/**
  * @brief GUI主任务
  * @note 无
  * @param 无
  * @retval 无
  */
void MainTask(void)
{
	/* 设置背景色 */
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	
	/* 2D绘图 */
	_2D_Graph_Drawing();
	
//	GUI_Delay(5000);
	GUI_Clear();

	/* Alpha混合 */
	Alpha_Blending();
	
	while(1)
	{
		GUI_Delay(100);
	}
}
