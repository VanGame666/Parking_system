#include "config.h"
#include "system.h"


void CpuInit(void)
{
	//CPU�������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���2���ж����ȼ�����2λ�����ȼ���2λ�����ȼ�
	
}
	
/********************************************************************
 * ������:Camera_settings
 * ����  :����ͷ�Ĳ�������
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Camera_settings(void)
{
  /*����ͷ��ʼ��*/
	while(OV7725_exit_Init() != SUCCESS);
	while(OV7725_entr_Init() != SUCCESS);

  /*���ڵ�����ͷ��������*/
	OV7725_exit_Light_Mode(Camera_mode);
	OV7725_exit_Color_Saturation(Camera_sat);
	OV7725_exit_Brightness(Camera_bri);	
	OV7725_exit_Contrast(Camera_cnst);
	OV7725_exit_Special_Effect(Camera_eff);
  
  /*��ڵ�����ͷ��������*/
  OV7725_entr_Light_Mode(Camera_mode);
	OV7725_entr_Color_Saturation(Camera_sat);
	OV7725_entr_Brightness(Camera_bri);	
	OV7725_entr_Contrast(Camera_cnst);
	OV7725_entr_Special_Effect(Camera_eff);
	
  /*��������ͷ�ɼ�ͼ���С*/
	OV7725_exit_Window_Set(0,0,320,240,0);
	OV7725_entr_Window_Set(0,0,320,240,0);
}

void SysInit(void)
{
	
	CpuInit();//����ϵͳ��Ϣ
	
	//����������ʼ��
	SysTickInit();//ϵͳ�δ�ʱ����ʼ��
	UsartInit();//���ڳ�ʼ��

  ESP8266_Init ();  //WIFI��ʼ��  
  GENERAL_TIM_Init();   //PWM��ʼ��
  OV7725_exit_GPIO_Config();  //��������ͷ��ʼ��
	OV7725_entr_GPIO_Config();   //�������ͷ��ʼ��

  Camera_settings();//����ͷ������ز��� 
  IWDG_Config(IWDG_Prescaler_128 ,3125);// IWDG 10s ��ʱ���	
}

