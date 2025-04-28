#include "config.h"
#include "Smoke_sensor.h"


/********************************************************************
 * ������:Exti_Config
 * ����  :���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Smoke_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(Smoke_A_INT_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = Smoke_A_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(Smoke_A_INT_GPIO_PORT, &GPIO_InitStructure);
}

