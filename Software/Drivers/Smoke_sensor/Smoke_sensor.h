#ifndef __Smoke_sensor_H
#define	__Smoke_sensor_H


#include "stm32f10x.h"


//Òý½Å¶¨Òå
#define Smoke_A_INT_GPIO_PORT         GPIOD
#define Smoke_A_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define Smoke_A_INT_GPIO_PIN          GPIO_Pin_3

void Smoke_Init(void);


#endif 
