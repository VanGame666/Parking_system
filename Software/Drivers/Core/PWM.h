#ifndef __TIMER__H__
#define __TIMER__H__

#include "stm32f10x.h"


/*******************************************************************************************************
** ����: Tim3Init, Tim��ʱ����ʱ����
**------------------------------------------------------------------------------------------------------
** ����: var ��ʱʱ��, ���� var * 0.1ms��ʱ
** ����: void
********************************************************************************************************/
void Tim3Init(int var);


#define EN_GeneralTimerPWM  1   //1:����ͨ�ö�ʱ��PWM���ܣ�0:�ر�ͨ�ö�ʱ��PWM����

#if EN_GeneralTimerPWM
	/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM3��4��5(ͨ�ö�ʱ��2���δ�����������ܣ������)************/
	// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
	// ��������Ĭ��ʹ��TIM3

	#define            GENERAL_TIM                   TIM3
	#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
	#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
	#define            GENERAL_TIM_Period            19999
	#define            GENERAL_TIM_Prescaler         71
  #define            CCR0_Val                      (GENERAL_TIM_Period+1)/200 
	// TIM3 ����Ƚ�ͨ��1
	#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
	#define            GENERAL_TIM_CH1_PORT          GPIOA
	#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

	// TIM3 ����Ƚ�ͨ��2
	#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
	#define            GENERAL_TIM_CH2_PORT          GPIOA
	#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

	// TIM3 ����Ƚ�ͨ��3
	#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
	#define            GENERAL_TIM_CH3_PORT          GPIOB
	#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

	// TIM3 ����Ƚ�ͨ��4
	#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
	#define            GENERAL_TIM_CH4_PORT          GPIOB
	#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1

	/**************************��������********************************/

void GENERAL_TIM_Init(void);

#endif

#endif
/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
