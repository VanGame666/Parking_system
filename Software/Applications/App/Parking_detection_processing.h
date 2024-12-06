#ifndef __Parking_detection_processing_H
#define __Parking_detection_processing_H	 
#include "config.h"


#define BUTTON_FILTER_TIME 	10  //单位为ms
#define BUTTON_LONG_TIME 	1000		// 持续1秒，认为长按事件
#define Total_P   2//停车场的车位总数
#define PS_Total  1//停车区域中每区域的车位总数
#define Stranded_Init 6//停车场滞留车辆初始值

#define Key1In					  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)	                  
#define Key2In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12)                  
#define Key3In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13)	
#define Key4In						GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14)

#define B1PS              GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define A1PS              GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

#define Smoke_A           GPIO_ReadInputDataBit(Smoke_A_INT_GPIO_PORT,Smoke_A_INT_GPIO_PIN)

/****************************************************************************************/
//用户添加自定义按键接口	

/****************************************************************************************/

/*******************************************************************************************************
** 函数: Button_t,初始化按键变量
********************************************************************************************************/
typedef struct
{
	unsigned char  (*IsKeyDownFunc)(void); 		// 函数指针，指向判断按键是否按下的函数 
	unsigned char 	wFilterCount;							// 滤波器计数器 
	unsigned char 	wFilterTime;							// 滤波时间(最大255,表示255ms) 
	unsigned short	wLongCount;								// 长按计数器 
	unsigned short	wLongTime;								// 长按键时间, 0表示不检测长按 
	unsigned char  	byState;									// 按键当前状态（按下还是弹起） 	
	unsigned char 	byKeyCodeUp;							// 按键弹起的键值代码, 0表示不检测按键弹起 	
	unsigned char 	byKeyCodeDown;						// 按键按下的键值代码, 0表示不检测按键按下 	
}Button_t;


/*******************************************************************************************************
** 函数: Button_t,按键枚举变量
********************************************************************************************************/
typedef enum
{
	KEY_NONE = 0,								// 0 表示无按键事件 

	KEY1_DOWN,									// KEY1键按下 
	KEY1_UP,									// KEY1键弹起 

	KEY2_DOWN,									// KEY2键按下 
	KEY2_UP,									// KEY2键弹起 

	KEY3_DOWN,									// KEY3键按下 
	KEY3_UP,									// KEY3键弹起 

  KEY4_DOWN,									// KEY3键按下 
	KEY4_UP,									// KEY3键弹起 
  
  SmokeA_DOWN,
  SmokeA_UP,
/***************************************************/
//用户添加其他按键枚举信息
	
/***************************************************/
}Key_em;

/*******************************************************************************************************
** 函数: ButtonProj,按键处理函数
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void ButtonProj(void);
void Parking_management(void);
void Parking_recommendation(void);
                                                      
#endif
