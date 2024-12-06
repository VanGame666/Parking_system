#ifndef __Parking_detection_processing_H
#define __Parking_detection_processing_H	 
#include "config.h"


#define BUTTON_FILTER_TIME 	10  //��λΪms
#define BUTTON_LONG_TIME 	1000		// ����1�룬��Ϊ�����¼�
#define Total_P   2//ͣ�����ĳ�λ����
#define PS_Total  1//ͣ��������ÿ����ĳ�λ����
#define Stranded_Init 6//ͣ��������������ʼֵ

#define Key1In					  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)	                  
#define Key2In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12)                  
#define Key3In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13)	
#define Key4In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14)

#define B1PS              GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define A1PS              GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

#define Smoke_A           GPIO_ReadInputDataBit(Smoke_A_INT_GPIO_PORT,Smoke_A_INT_GPIO_PIN)

/****************************************************************************************/
//�û�����Զ��尴���ӿ�	

/****************************************************************************************/

/*******************************************************************************************************
** ����: Button_t,��ʼ����������
********************************************************************************************************/
typedef struct
{
	unsigned char  (*IsKeyDownFunc)(void); 		// ����ָ�룬ָ���жϰ����Ƿ��µĺ��� 
	unsigned char 	wFilterCount;							// �˲��������� 
	unsigned char 	wFilterTime;							// �˲�ʱ��(���255,��ʾ255ms) 
	unsigned short	wLongCount;								// ���������� 
	unsigned short	wLongTime;								// ������ʱ��, 0��ʾ����ⳤ�� 
	unsigned char  	byState;									// ������ǰ״̬�����»��ǵ��� 	
	unsigned char 	byKeyCodeUp;							// ��������ļ�ֵ����, 0��ʾ����ⰴ������ 	
	unsigned char 	byKeyCodeDown;						// �������µļ�ֵ����, 0��ʾ����ⰴ������ 	
}Button_t;


/*******************************************************************************************************
** ����: Button_t,����ö�ٱ���
********************************************************************************************************/
typedef enum
{
	KEY_NONE = 0,								// 0 ��ʾ�ް����¼� 

	KEY1_DOWN,									// KEY1������ 
	KEY1_UP,									// KEY1������ 

	KEY2_DOWN,									// KEY2������ 
	KEY2_UP,									// KEY2������ 

	KEY3_DOWN,									// KEY3������ 
	KEY3_UP,									// KEY3������ 

  KEY4_DOWN,									// KEY3������ 
	KEY4_UP,									// KEY3������ 
  
  SmokeA_DOWN,
  SmokeA_UP,
/***************************************************/
//�û������������ö����Ϣ
	
/***************************************************/
}Key_em;

/*******************************************************************************************************
** ����: ButtonProj,����������
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
void ButtonProj(void);
void Parking_management(void);
void Parking_recommendation(void);
                                                      
#endif
