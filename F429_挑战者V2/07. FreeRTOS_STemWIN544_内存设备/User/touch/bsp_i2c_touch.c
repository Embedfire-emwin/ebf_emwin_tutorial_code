/**
  ******************************************************************************
  * @file    bsp_i2c_touch.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c(gt9xx)应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 iSO STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt9xx.h"
#include "./usart/bsp_debug_usart.h"

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7      0x0A


__IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
   
  /*!< GTP_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(GTP_I2C_CLK, ENABLE);
  
  /*!< GTP_I2C_SCL_GPIO_CLK and GTP_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(GTP_I2C_SCL_GPIO_CLK | GTP_I2C_SDA_GPIO_CLK|GTP_RST_GPIO_CLK|GTP_INT_GPIO_CLK, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
  /*!< GPIO configuration */
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_SOURCE, GTP_I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_SOURCE, GTP_I2C_SDA_AF);  
  
  /*!< Configure GTP_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = GTP_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GTP_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure GTP_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = GTP_I2C_SDA_PIN;
  GPIO_Init(GTP_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
 
 
  /*!< Configure RST */   
  GPIO_InitStructure.GPIO_Pin = GTP_RST_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GTP_RST_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure INT */   
  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;       //设置为下拉，方便初始化
  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

}


/**
  * @brief  对GT91xx芯片进行复位
  * @param  无
  * @retval 无
  */
void I2C_ResetChip(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /*!< Configure INT */
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;       //设置为下拉，方便初始化
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);

	  /*初始化GT9157,rst为高电平，int为低电平，则gt9157的设备地址被配置为0xBA*/

	  /*复位为低电平，为初始化做准备*/
	  GPIO_ResetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*拉高一段时间，进行初始化*/
	  GPIO_SetBits (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
	  Delay(0x0FFFFF);

	  /*把INT引脚设置为浮空输入模式*/
	  /*!< Configure INT */
	  GPIO_InitStructure.GPIO_Pin = GTP_INT_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		                    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                            /* 通信速率 */
  I2C_Init(GTP_I2C, &I2C_InitStructure);	                                  /* I2C1 初始化 */
  I2C_Cmd(GTP_I2C, ENABLE);  	                                              /* 使能 I2C1 */

  I2C_AcknowledgeConfig(GTP_I2C, ENABLE);
  
//  I2C_GenerateSTOP (I2C1,DISABLE);
}


/**
  * @brief  I2C 外设(GT91xx)初始化
  * @param  无
  * @retval 无
  */
void I2C_Touch_Init(void)
{
  I2C_GPIO_Config(); 
 
  I2C_Mode_Config();

  I2C_ResetChip();

}


/**
  * @brief  IIC等待超时调用本函数输出调试信息
  * @param  None.
  * @retval None.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(void)
{
  GTP_ERROR("I2C Timeout error!");
  return 0xFF;
}


/**
  * @brief   使用IIC读取数据
  * @param   
  * 	@arg ClientAddr:从设备地址
  *		@arg pBuffer:存放由从机读取的数据的缓冲区指针
  *		@arg NumByteToRead:读取的数据长度
  * @retval  无
  */
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{  
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(GTP_I2C, I2C_FLAG_BUSY))   
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }

  /* Send STRAT condition  */
  I2C_GenerateSTART(GTP_I2C, ENABLE);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
   }
  /* Send GT91xx address for read */
  I2C_Send7bitAddress(GTP_I2C, ClientAddr, I2C_Direction_Receiver);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
   }
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(GTP_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(GTP_I2C, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(GTP_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(GTP_I2C, ENABLE);
  
  return 0;
}


/**
  * @brief   使用IIC写入数据
  * @param   
  * 	@arg ClientAddr:从设备地址
  *		@arg pBuffer:缓冲区指针
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(GTP_I2C, I2C_FLAG_BUSY))  
   {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(GTP_I2C, ENABLE);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
  } 
  
  /* Send GT91xx address for write */
  I2C_Send7bitAddress(GTP_I2C, ClientAddr, I2C_Direction_Transmitter);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
  } 
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(GTP_I2C, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(GTP_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback();
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(GTP_I2C, ENABLE);
  
  return 0;
}

/*********************************************END OF FILE**********************/
