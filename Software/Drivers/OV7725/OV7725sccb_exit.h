#ifndef __OV7725SCCB_exit_H
#define __OV7725SCCB_exit_H



#include "stm32f10x.h"



/************************** OV7725_exit 连接引脚定义********************************/
#define      OV7725_exit_SIO_C_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_exit_SIO_C_GPIO_CLK                       RCC_APB2Periph_GPIOD
#define      OV7725_exit_SIO_C_GPIO_PORT                      GPIOD
#define      OV7725_exit_SIO_C_GPIO_PIN                       GPIO_Pin_7

#define      OV7725_exit_SIO_D_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_exit_SIO_D_GPIO_CLK                       RCC_APB2Periph_GPIOD
#define      OV7725_exit_SIO_D_GPIO_PORT                      GPIOD
#define      OV7725_exit_SIO_D_GPIO_PIN                       GPIO_Pin_6

#define OV7725_exit_SCCB_SDA_IN  {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X08000000;OV7725_exit_SDA_H;}
#define OV7725_exit_SCCB_SDA_OUT {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X03000000;}

#define OV7725_exit_SCL_H         GPIO_SetBits(OV7725_exit_SIO_C_GPIO_PORT , OV7725_exit_SIO_C_GPIO_PIN) 
#define OV7725_exit_SCL_L         GPIO_ResetBits(OV7725_exit_SIO_C_GPIO_PORT , OV7725_exit_SIO_C_GPIO_PIN) 
   
#define OV7725_exit_SDA_H         GPIO_SetBits(OV7725_exit_SIO_D_GPIO_PORT , OV7725_exit_SIO_D_GPIO_PIN) 
#define OV7725_exit_SDA_L         GPIO_ResetBits(OV7725_exit_SIO_D_GPIO_PORT , OV7725_exit_SIO_D_GPIO_PIN) 

#define OV7725_exit_SCL_read      GPIO_ReadInputDataBit(OV7725_exit_SIO_C_GPIO_PORT , OV7725_exit_SIO_C_GPIO_PIN) 
#define OV7725_exit_SDA_read      GPIO_ReadInputDataBit(OV7725_exit_SIO_D_GPIO_PORT , OV7725_exit_SIO_D_GPIO_PIN) 

#define ADDR_OV7725_exit   0x42



extern void OV7725_exit_SCCB_GPIO_Config(void);
extern int OV7725_exit_SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
extern int OV7725_exit_SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);



#endif 
