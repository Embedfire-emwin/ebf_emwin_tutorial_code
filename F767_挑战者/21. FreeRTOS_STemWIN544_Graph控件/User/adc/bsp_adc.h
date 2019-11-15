#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f7xx.h"

// ADC GPIO 宏定义
#define RHEOSTAT_ADC_GPIO_PORT              GPIOC
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_3
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
    
// ADC 序号宏定义
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_13

// ADC 中断宏定义
#define Rheostat_ADC_IRQ                    ADC_IRQn
#define Rheostat_ADC_INT_FUNCTION           ADC_IRQHandler



void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



