#ifndef __OV7725SCCB_exit_H
#define __OV7725SCCB_exit_H



#include "stm32f10x.h"



/************************** OV7725 连接引脚定义********************************/
#define      OV7725_SIO_C_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_SIO_C_GPIO_CLK                       RCC_APB2Periph_GPIOE
#define      OV7725_SIO_C_GPIO_PORT                      GPIOE
#define      OV7725_SIO_C_GPIO_PIN                       GPIO_Pin_4

#define      OV7725_SIO_D_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_SIO_D_GPIO_CLK                       RCC_APB2Periph_GPIOE
#define      OV7725_SIO_D_GPIO_PORT                      GPIOE
#define      OV7725_SIO_D_GPIO_PIN                       GPIO_Pin_6

#define OV7725_SCCB_SDA_IN  {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=0X08000000;OV7725_SDA_H;}
#define OV7725_SCCB_SDA_OUT {GPIOE->CRL&=0XF0FFFFFF;GPIOE->CRL|=0X03000000;}

#define OV7725_SCL_H         GPIO_SetBits(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
#define OV7725_SCL_L         GPIO_ResetBits(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
   
#define OV7725_SDA_H         GPIO_SetBits(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 
#define OV7725_SDA_L         GPIO_ResetBits(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 

#define OV7725_SCL_read      GPIO_ReadInputDataBit(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
#define OV7725_SDA_read      GPIO_ReadInputDataBit(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 

#define ADDR_OV7725   0x42



void OV7725_SCCB_GPIO_Config(void);
int OV7725_SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
int OV7725_SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);



#endif 
