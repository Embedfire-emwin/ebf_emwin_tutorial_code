#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H


#include "stm32f10x.h"


/******************************* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             XPT2046_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOF   
#define             XPT2046_PENIRQ_GPIO_PORT                       GPIOF
#define             XPT2046_PENIRQ_GPIO_PIN                        GPIO_Pin_9

//触屏信号有效电平
#define             XPT2046_PENIRQ_ActiveLevel                     0
#define             XPT2046_PENIRQ_Read()                          GPIO_ReadInputDataBit ( XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN )

/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             XPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOF| RCC_APB2Periph_GPIOG

#define             XPT2046_SPI_CS_PIN		                        GPIO_Pin_10
#define             XPT2046_SPI_CS_PORT		                      GPIOF

#define	            XPT2046_SPI_CLK_PIN	                        GPIO_Pin_7
#define             XPT2046_SPI_CLK_PORT	                        GPIOG

#define	            XPT2046_SPI_MOSI_PIN	                        GPIO_Pin_11
#define	            XPT2046_SPI_MOSI_PORT	                      GPIOF

#define	            XPT2046_SPI_MISO_PIN	                        GPIO_Pin_6
#define	            XPT2046_SPI_MISO_PORT	                      GPIOF


#define             XPT2046_CS_ENABLE()                          GPIO_SetBits ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )    
#define             XPT2046_CS_DISABLE()                         GPIO_ResetBits ( XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN )  

#define             XPT2046_CLK_HIGH()                           GPIO_SetBits ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN )    
#define             XPT2046_CLK_LOW()                            GPIO_ResetBits ( XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN ) 

#define             XPT2046_MOSI_1()                             GPIO_SetBits ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN ) 
#define             XPT2046_MOSI_0()                             GPIO_ResetBits ( XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN )

#define             XPT2046_MISO()                               GPIO_ReadInputDataBit ( XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN )

/*信息输出*/
#define XPT2046_DEBUG_ON         0

#define XPT2046_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define XPT2046_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)
#define XPT2046_DEBUG(fmt,arg...)          do{\
                                          if(XPT2046_DEBUG_ON)\
                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


/******************************* XPT2046 触摸屏参数定义 ***************************/
#define	            macXPT2046_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            macXPT2046_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字 	 


/******************************** XPT2046 触摸屏函数声明 **********************************/
void XPT2046_Init( void );
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel);

#endif /* __BSP_TOUCH_H */

