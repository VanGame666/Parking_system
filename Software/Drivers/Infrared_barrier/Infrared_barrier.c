#include "config.h"
#include "Infrared_barrier.h"
							    
/********************************************************************
 * ������:Infrared_Init
 * ����  :�������ģ�����ӵ�IO��ʼ��
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Infrared_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTFʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF11,12,13,14

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB13,14
}




