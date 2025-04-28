#ifndef __TIMER__H__
#define __TIMER__H__

#include "stm32f10x.h"


/*******************************************************************************************************
** 函数: Tim3Init, Tim定时器延时函数
**------------------------------------------------------------------------------------------------------
** 参数: var 延时时间, 代表 var * 0.1ms延时
** 返回: void
********************************************************************************************************/
void Tim3Init(int var);


#define EN_GeneralTimerPWM  1   //1:开启通用定时器PWM功能，0:关闭通用定时器PWM功能

#if EN_GeneralTimerPWM
	/************通用定时器TIM参数定义，只限TIM3、4、5(通用定时器2如果未被用其他功能，则可用)************/
	// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
	// 我们这里默认使用TIM3

	#define            GENERAL_TIM                   TIM3
	#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
	#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
	#define            GENERAL_TIM_Period            19999
	#define            GENERAL_TIM_Prescaler         71
  #define            CCR0_Val                      (GENERAL_TIM_Period+1)/200 
	// TIM3 输出比较通道1
	#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
	#define            GENERAL_TIM_CH1_PORT          GPIOA
	#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

	// TIM3 输出比较通道2
	#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
	#define            GENERAL_TIM_CH2_PORT          GPIOA
	#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

	// TIM3 输出比较通道3
	#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
	#define            GENERAL_TIM_CH3_PORT          GPIOB
	#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

	// TIM3 输出比较通道4
	#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
	#define            GENERAL_TIM_CH4_PORT          GPIOB
	#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1

	/**************************函数声明********************************/

void GENERAL_TIM_Init(void);

#endif

#endif
/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
