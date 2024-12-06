#include "config.h"
#include "system.h"


void CpuInit(void)
{
	//CPU相关设置
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择第2组中断优先级，即2位主优先级，2位次优先级
	
}
	
/********************************************************************
 * 函数名:Camera_settings
 * 描述  :摄像头的参数设置
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void Camera_settings(void)
{
  /*摄像头初始化*/
	while(OV7725_exit_Init() != SUCCESS);
	while(OV7725_entr_Init() != SUCCESS);

  /*出口的摄像头参数设置*/
	OV7725_exit_Light_Mode(Camera_mode);
	OV7725_exit_Color_Saturation(Camera_sat);
	OV7725_exit_Brightness(Camera_bri);	
	OV7725_exit_Contrast(Camera_cnst);
	OV7725_exit_Special_Effect(Camera_eff);
  
  /*入口的摄像头参数设置*/
  OV7725_entr_Light_Mode(Camera_mode);
	OV7725_entr_Color_Saturation(Camera_sat);
	OV7725_entr_Brightness(Camera_bri);	
	OV7725_entr_Contrast(Camera_cnst);
	OV7725_entr_Special_Effect(Camera_eff);
	
  /*设置摄像头采集图像大小*/
	OV7725_exit_Window_Set(0,0,320,240,0);
	OV7725_entr_Window_Set(0,0,320,240,0);
}

void SysInit(void)
{
	
	CpuInit();//配置系统信息
	
	//外设驱动初始化
	SysTickInit();//系统滴答定时器初始化
	UsartInit();//串口初始化

  ESP8266_Init ();  //WIFI初始化  
  GENERAL_TIM_Init();   //PWM初始化
  OV7725_exit_GPIO_Config();  //出口摄像头初始化
	OV7725_entr_GPIO_Config();   //入口摄像头初始化

  Camera_settings();//摄像头配置相关参数 
  IWDG_Config(IWDG_Prescaler_128 ,3125);// IWDG 10s 超时溢出	
}

