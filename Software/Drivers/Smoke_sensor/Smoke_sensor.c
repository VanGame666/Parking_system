#include "config.h"
#include "Smoke_sensor.h"


/********************************************************************
 * 函数名:Exti_Config
 * 描述  :配置 IO为EXTI中断口，并设置中断优先级
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void Smoke_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(Smoke_A_INT_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = Smoke_A_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(Smoke_A_INT_GPIO_PORT, &GPIO_InitStructure);
}

