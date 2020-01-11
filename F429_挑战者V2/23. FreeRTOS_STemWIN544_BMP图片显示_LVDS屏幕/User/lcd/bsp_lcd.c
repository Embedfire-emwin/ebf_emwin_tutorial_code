/**
  ******************************************************************************
  * @file    stm32f429i_discovery_lcd.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    28-October-2013
  * @brief   This file includes the LCD driver for ILI9341 Liquid Crystal 
  *          Display Modules of STM32F429I-DISCO kit (MB1075).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "./lcd/bsp_lcd.h"



static void LCD_GPIO_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStruct;

 /* 使能LCD使用到的引脚时钟 */
                         //红色数据线
 RCC_AHB1PeriphClockCmd(LTDC_R0_GPIO_CLK | LTDC_R1_GPIO_CLK | LTDC_R2_GPIO_CLK|
                        LTDC_R3_GPIO_CLK | LTDC_R4_GPIO_CLK | LTDC_R5_GPIO_CLK|
                        LTDC_R6_GPIO_CLK | LTDC_R7_GPIO_CLK |
                         //绿色数据线
                         LTDC_G0_GPIO_CLK|LTDC_G1_GPIO_CLK|LTDC_G2_GPIO_CLK|
                         LTDC_G3_GPIO_CLK|LTDC_G4_GPIO_CLK|LTDC_G5_GPIO_CLK|
                         LTDC_G6_GPIO_CLK|LTDC_G7_GPIO_CLK|
                         //蓝色数据线
                         LTDC_B0_GPIO_CLK|LTDC_B1_GPIO_CLK|LTDC_B2_GPIO_CLK|
                         LTDC_B3_GPIO_CLK|LTDC_B4_GPIO_CLK|LTDC_B5_GPIO_CLK|
                         LTDC_B6_GPIO_CLK|LTDC_B7_GPIO_CLK|
                         //控制信号线
                         LTDC_CLK_GPIO_CLK | LTDC_HSYNC_GPIO_CLK |LTDC_VSYNC_GPIO_CLK|
                         LTDC_DE_GPIO_CLK  | LTDC_BL_GPIO_CLK    |LTDC_DISP_GPIO_CLK ,ENABLE);


/* GPIO配置 */

/* 红色数据线 */
 GPIO_InitStruct.GPIO_Pin = LTDC_R0_GPIO_PIN;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

 GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R0_GPIO_PORT, LTDC_R0_PINSOURCE, LTDC_R0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R1_GPIO_PIN;
 GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R1_GPIO_PORT, LTDC_R1_PINSOURCE, LTDC_R1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R2_GPIO_PIN;
 GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R2_GPIO_PORT, LTDC_R2_PINSOURCE, LTDC_R2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R3_GPIO_PIN;
 GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R3_GPIO_PORT, LTDC_R3_PINSOURCE, LTDC_R3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R4_GPIO_PIN;
 GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R4_GPIO_PORT, LTDC_R4_PINSOURCE, LTDC_R4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R5_GPIO_PIN;
 GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R5_GPIO_PORT, LTDC_R5_PINSOURCE, LTDC_R5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R6_GPIO_PIN;
 GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R6_GPIO_PORT, LTDC_R6_PINSOURCE, LTDC_R6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R7_GPIO_PIN;
 GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R7_GPIO_PORT, LTDC_R7_PINSOURCE, LTDC_R7_AF);

 //绿色数据线
 GPIO_InitStruct.GPIO_Pin = LTDC_G0_GPIO_PIN;
 GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G0_GPIO_PORT, LTDC_G0_PINSOURCE, LTDC_G0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G1_GPIO_PIN;
 GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G1_GPIO_PORT, LTDC_G1_PINSOURCE, LTDC_G1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G2_GPIO_PIN;
 GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G2_GPIO_PORT, LTDC_G2_PINSOURCE, LTDC_G2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G3_GPIO_PIN;
 GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G3_GPIO_PORT, LTDC_G3_PINSOURCE, LTDC_G3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G4_GPIO_PIN;
 GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G4_GPIO_PORT, LTDC_G4_PINSOURCE, LTDC_G4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G5_GPIO_PIN;
 GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G5_GPIO_PORT, LTDC_G5_PINSOURCE, LTDC_G5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G6_GPIO_PIN;
 GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G6_GPIO_PORT, LTDC_G6_PINSOURCE, LTDC_G6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G7_GPIO_PIN;
 GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G7_GPIO_PORT, LTDC_G7_PINSOURCE, LTDC_G7_AF);

 //蓝色数据线
 GPIO_InitStruct.GPIO_Pin = LTDC_B0_GPIO_PIN;
 GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B0_GPIO_PORT, LTDC_B0_PINSOURCE, LTDC_B0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B1_GPIO_PIN;
 GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B1_GPIO_PORT, LTDC_B1_PINSOURCE, LTDC_B1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B2_GPIO_PIN;
 GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B2_GPIO_PORT, LTDC_B2_PINSOURCE, LTDC_B2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B3_GPIO_PIN;
 GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B3_GPIO_PORT, LTDC_B3_PINSOURCE, LTDC_B3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B4_GPIO_PIN;
 GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B4_GPIO_PORT, LTDC_B4_PINSOURCE, LTDC_B4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B5_GPIO_PIN;
 GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B5_GPIO_PORT, LTDC_B5_PINSOURCE, LTDC_B5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B6_GPIO_PIN;
 GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B6_GPIO_PORT, LTDC_B6_PINSOURCE, LTDC_B6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B7_GPIO_PIN;
 GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B7_GPIO_PORT, LTDC_B7_PINSOURCE, LTDC_B7_AF);

 //控制信号线
 GPIO_InitStruct.GPIO_Pin = LTDC_CLK_GPIO_PIN;
 GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_CLK_GPIO_PORT, LTDC_CLK_PINSOURCE, LTDC_CLK_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_HSYNC_GPIO_PIN;
 GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_HSYNC_GPIO_PORT, LTDC_HSYNC_PINSOURCE, LTDC_HSYNC_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_VSYNC_GPIO_PIN;
 GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_VSYNC_GPIO_PORT, LTDC_VSYNC_PINSOURCE, LTDC_VSYNC_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_DE_GPIO_PIN;
 GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_DE_GPIO_PORT, LTDC_DE_PINSOURCE, LTDC_DE_AF);

 //BL DISP
 GPIO_InitStruct.GPIO_Pin = LTDC_DISP_GPIO_PIN;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

 GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);


 GPIO_InitStruct.GPIO_Pin = LTDC_BL_GPIO_PIN;
 GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);

 //拉高使能lcd
 GPIO_SetBits(LTDC_DISP_GPIO_PORT,LTDC_DISP_GPIO_PIN);
 GPIO_SetBits(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN);

}
/**
 * @brief  Initializes the LCD.
 * @param  None
 * @retval None
 */
void LCD_Init(void)
{
 LTDC_InitTypeDef       LTDC_InitStruct;

 /* Enable the LTDC Clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

 /* Enable the DMA2D Clock */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

 /* Configure the LCD Control pins */
 LCD_GPIO_Config();

 /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
// SDRAM_Init();

 /* LTDC Configuration *********************************************************/
 /* Polarity configuration */
 /* Initialize the horizontal synchronization polarity as active low */
 LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
 /* Initialize the vertical synchronization polarity as active low */
 LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
 /* Initialize the data enable polarity as active low */
 LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
 /* Initialize the pixel clock polarity as input pixel clock */
 LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

 /* Configure R,G,B component values for LCD background color */
 LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
 LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
 LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

 /* Configure PLLSAI prescalers for LCD */
 /* Enable Pixel Clock */
 /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
 /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 144 Mhz */
 /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 144/2 = 72 Mhz */
 /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 72/4 = 18 Mhz */ 
 //开启两层时：设置为24MHz时界面运行流畅，如果设置为36Mhz时界面程序闪烁情况
 //如果只使用单层，可以设置成较高频率
 //另外，时钟频率跟颜色模式有关系，使用ARGB8888模式时时钟减半
 RCC_PLLSAIConfig(130, 7, 1);
 RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);//18MHz
 
 /* Enable PLLSAI Clock */
 RCC_PLLSAICmd(ENABLE);
 /* Wait for PLLSAI activation */
 while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
 {
 }

  /* 时间参数配置 */  
 /* 配置行同步信号宽度(HSW-1) */
 LTDC_InitStruct.LTDC_HorizontalSync =HSW-1;
 /* 配置垂直同步信号宽度(VSW-1) */
 LTDC_InitStruct.LTDC_VerticalSync = VSW-1;
 /* 配置(HSW+HBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedHBP =HSW+HBP-1;
 /* 配置(VSW+VBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedVBP = VSW+VBP-1;
 /* 配置(HSW+HBP+有效像素宽度-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveW = HSW+HBP+LCD_PIXEL_WIDTH-1;
 /* 配置(VSW+VBP+有效像素高度-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveH = VSW+VBP+LCD_PIXEL_HEIGHT-1;
 /* 配置总宽度(HSW+HBP+有效像素宽度+HFP-1) */
 LTDC_InitStruct.LTDC_TotalWidth =HSW+ HBP+LCD_PIXEL_WIDTH  + HFP-1; 
 /* 配置总高度(VSW+VBP+有效像素高度+VFP-1) */
 LTDC_InitStruct.LTDC_TotalHeigh =VSW+ VBP+LCD_PIXEL_HEIGHT  + VFP-1;

 LTDC_Init(&LTDC_InitStruct);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
