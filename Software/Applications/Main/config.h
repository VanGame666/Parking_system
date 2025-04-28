#ifndef  __CONFIG_H__
#define  __CONFIG_H__

//�޷������͵Ķ���
#define uchar unsigned char
#define uint unsigned int 
	
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
	
//----------------------------------------


//ȫ�ֺ궨��
#define CPU_INT_DISABLE()  {__set_PRIMASK(1);} //�ر��ж�
#define CPU_INT_ENABLE()   {__set_PRIMASK(0);}  //���ж�

//ȫ�����Ͷ���
typedef enum{FALSE = 0, TRUE = !FALSE}BOOL;

//-------------------------------------
#ifdef MAIN_CONFIG  //ͷ�ļ������C����ʱ�����������ͻ����
	#define EXT
#else 
	#define EXT extern
#endif

//-------------------------------------
//RTOS���ͷ�ļ�����
#include <rthw.h>
#include <rtthread.h>

#include "Task.h"	

//-------------------------------------
	
//����ͷ�ļ�����
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdlib.h"

//-------------------------------------
	
//��������ͷ�ļ�����
#include "system.h"
#include "SysTick.h"
#include "USart.h"
#include "IWDG.h"
#include "PWM.h"
#include "lcd.h"
#include "ESP8266.h"
#include "Smoke_sensor.h"
#include "Infrared_barrier.h"
#include "OV7725_entrance.h"
#include "OV7725sccb_entrance.h"
#include "OV7725_exit.h"
#include "OV7725sccb_exit.h"


#include "LCD_Basic_application.h"
#include "LCD_Interface_design.h"
#include "License_plate_recognition.h"
#include "Parking_detection_processing.h"
//----------------------------------------
//ȫ�ֱ�������,��ʹ��EXT����
EXT unsigned char g_Var;	
EXT rt_mq_t message_mq;//����һ����Ϣ���п��ƿ�
EXT rt_sem_t exit_recv_sem;//��������ź������ƿ�ָ��
EXT rt_sem_t entr_recv_sem;//��������ź������ƿ�ָ��
EXT rt_event_t Inspection_event;//�����¼����ƿ�ָ��
EXT rt_timer_t In_ProcessSoftTimer;//�����ʱ�����ƿ�ָ��


//Ҫ���ӵ��ȵ�����ƣ���WIFI����
#define      macUser_ESP8266_ApSsid           "ThinkBook"

//Ҫ���ӵ��ȵ����Կ
#define      macUser_ESP8266_ApPwd            "88888888"

//Ҫ���ӵķ������� IP�������Ե�IP
#define      macUser_ESP8266_TcpServer_IP     "192.168.0.100"

//Ҫ���ӵķ������Ķ˿�
#define      macUser_ESP8266_TcpServer_Port    "6345" 

/**********����ͷ��������**************/
#define  Camera_mode     0    //����ͷ����ģʽ
#define  Camera_sat      0    //����ͷ���Ͷ�
#define  Camera_bri      0    //����ͷ���ն�
#define  Camera_cnst     0    //����ͷ�Աȶ�
#define  Camera_eff      0    //����ͷ����Ч��

//----------------------------------------
#endif

