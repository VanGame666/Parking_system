#ifndef  __CONFIG_H__
#define  __CONFIG_H__

//无符号类型的定义
#define uchar unsigned char
#define uint unsigned int 
	
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
	
//----------------------------------------


//全局宏定义
#define CPU_INT_DISABLE()  {__set_PRIMASK(1);} //关闭中断
#define CPU_INT_ENABLE()   {__set_PRIMASK(0);}  //打开中断

//全局类型定义
typedef enum{FALSE = 0, TRUE = !FALSE}BOOL;

//-------------------------------------
#ifdef MAIN_CONFIG  //头文件被多个C调用时，避免变量冲突问题
	#define EXT
#else 
	#define EXT extern
#endif

//-------------------------------------
//RTOS相关头文件包含
#include <rthw.h>
#include <rtthread.h>

#include "Task.h"	

//-------------------------------------
	
//常用头文件包含
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdlib.h"

//-------------------------------------
	
//外设驱动头文件包含
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
//全局变量定义,请使用EXT修饰
EXT unsigned char g_Var;	
EXT rt_mq_t message_mq;//定义一个消息队列控制块
EXT rt_sem_t exit_recv_sem;//定义出口信号量控制块指针
EXT rt_sem_t entr_recv_sem;//定义入口信号量控制块指针
EXT rt_event_t Inspection_event;//定义事件控制块指针
EXT rt_timer_t In_ProcessSoftTimer;//软件定时器控制块指针


//要连接的热点的名称，即WIFI名称
#define      macUser_ESP8266_ApSsid           "ThinkBook"

//要连接的热点的密钥
#define      macUser_ESP8266_ApPwd            "88888888"

//要连接的服务器的 IP，即电脑的IP
#define      macUser_ESP8266_TcpServer_IP     "192.168.0.100"

//要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_Port    "6345" 

/**********摄像头参数设置**************/
#define  Camera_mode     0    //摄像头光照模式
#define  Camera_sat      0    //摄像头饱和度
#define  Camera_bri      0    //摄像头光照度
#define  Camera_cnst     0    //摄像头对比度
#define  Camera_eff      0    //摄像头特殊效果

//----------------------------------------
#endif

