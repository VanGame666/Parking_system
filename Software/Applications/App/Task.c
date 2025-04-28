#include "config.h"
#include "Task.h"

extern uint8_t Update_P;//标记是否需要更新显示车位信息:0为不需要，1为需要

/*************************************************************************
*                            变量定义
*************************************************************************/
static uint16_t exit_num=0,entr_num=0;//计数进出车辆的数目
uint16_t  Detained_vehicle=0;//计数滞留车辆数目

static rt_timer_t Inspection_ProcessSoftTimer = RT_NULL;//软件定时器控制块指针
static rt_thread_t message_thread;//定义线程控制块指针

/*************************************************************************
*                            函数声明
*************************************************************************/
static void      IWDG_thread_entry     (void *parameter);//看门狗线程入口函数
static void      Event_thread_entry    (void *parameter);//切换成车位信息显示的线程入口函数
static void      exit_thread_entry     (void *parameter);// 有车要驶出的线程入口函数
static void      entr_thread_entry     (void *parameter);// 有车要驶入的线程入口函数
static void      WIFI_thread_entry     (void *parameter);//连接WIFI、本地服务器的线程入口函数
static void      Vehicle_count         (void);// 计算停车场的四种计数的函数

/*************************************************************************
*                            线程创建
*************************************************************************/
TaskStruct TaskThread[]={
{"IWDG_thread", IWDG_thread_entry, RT_NULL, 256, 2, 10},//看门狗线程
{"Event_thread", Event_thread_entry, RT_NULL, 512, 5, 20},//显示屏显示车位信息显示线程
{"exit_thread", exit_thread_entry, RT_NULL, 3072, 4, 80},//出口有车要驶出的线程
{"entr_thread", entr_thread_entry, RT_NULL, 3072, 4, 80},//入口有车要驶入的线程
{"WIFI_thread", WIFI_thread_entry, RT_NULL, 768, 3, 20},//连接WIFI，本地服务器

{"", RT_NULL, RT_NULL, RT_NULL, RT_NULL, RT_NULL},
};

/*************************************************************************
*                             软件定时器回调函数定义
*************************************************************************/
static void Inspection_ProcessSoftTimer_callback(void* parameter)
{ 
  Parking_management();//车位检测以及信息更新 
}

/*************************************************************************
*                            看门狗线程入口函数
*************************************************************************/
static void IWDG_thread_entry(void *parameter)
{
  while(1)
  {
    rt_thread_mdelay(5000);//喂狗的时间间隔
    IWDG_Feed();//喂狗
    rt_kprintf("喂狗\n");
  }
}

/*************************************************************************
*                     切换成车位信息显示的线程入口函数
*************************************************************************/
static void Event_thread_entry(void *parameter)
{
  rt_err_t uwRet = RT_EOK;
  rt_uint32_t e;
  while(1)
  {
    uwRet = rt_event_recv(Inspection_event, (EVENT_FLAG0 |EVENT_FLAG1 ),
                      RT_EVENT_FLAG_OR| RT_EVENT_FLAG_CLEAR ,
                      RT_WAITING_FOREVER, &e);
    if(RT_EOK == uwRet )
    {
      Parking_information();//车位信息显示
      Update_P=1;//为了刷新车位信息
      Parking_management();//显示车位信息
       /* 启动定时器 */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
        rt_timer_start(Inspection_ProcessSoftTimer);
    }
  }
}

/*************************************************************************
*                       有车要驶出的线程入口函数
*************************************************************************/
static void exit_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while(1)
	{    
    uwRet =rt_sem_take(exit_recv_sem, RT_WAITING_FOREVER);//获取有车驶出的信号量
    if(RT_EOK == uwRet )
    {
      /* 暂停定时器 */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
        rt_timer_stop(Inspection_ProcessSoftTimer);
      while(!Number_recognition_LP(exit_LP) && Key3In == 0 );//车牌识别及显示
      Exit_LP_communication(); //车牌号发给上位机
      rt_thread_mdelay(80);
      TIM_SetCompare3(TIM3,600);//舵机升起
      rt_thread_mdelay(20);
      exit_num++;//累计驶出车辆
      exit_num%=100;//本系统数值显示最大为99     
      Vehicle_count();
    }
	}
}

/*************************************************************************
*                       有车要驶入的线程入口函数
*************************************************************************/
static void entr_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while(1)
	{
		uwRet =rt_sem_take(entr_recv_sem, RT_WAITING_FOREVER);//获取有车驶入信号量
    if(RT_EOK == uwRet )
    {
      /* 暂停定时器 */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
      rt_timer_stop(Inspection_ProcessSoftTimer);
      while(!Number_recognition_LP(entrance_LP) && Key1In == 0 );//车牌识别及显示 
      Show_Str(76,320,BLACK,WHITE,"请前往",24,1);
      Show_Str(196,320,BLACK,WHITE,"车位",24,1);
      Entrance_LP_communication();//车牌号发给上位机
      Parking_recommendation();//液晶屏显示推荐车位
      rt_thread_mdelay(80);
      TIM_SetCompare4(TIM3,600);//舵机升起
      rt_thread_mdelay(20);
      entr_num++;//累计驶入车辆数目
      entr_num%=100;//本系统数值显示最大为99
      Vehicle_count();
    }
	}
}

/*************************************************************************
*                      连接WIFI、本地服务器的线程入口函数
*************************************************************************/
static void WIFI_thread_entry(void *parameter)
{
 	macESP8266_CH_ENABLE();//启用ESP8266
	
	ESP8266_AT_Test();//AT指令测试
	
	ESP8266_Net_Mode_Choose ( STA );//模式选择

	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	//连接WIFI
	
	ESP8266_Enable_MultipleId ( DISABLE );//是否多连接
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );//连接服务器
	
	while ( ! ESP8266_UnvarnishSend () );//进入透传
  
  Interface_init();//液晶屏系统界面显示
  Show_Str(112,58,BLACK,WHITE,"已",16,0);
  Vehicle_count();
      /* 启动定时器 */
  if (Inspection_ProcessSoftTimer != RT_NULL) 
      rt_timer_start(Inspection_ProcessSoftTimer);
      /* 启动定时器 */
  if (In_ProcessSoftTimer != RT_NULL) 
      rt_timer_start(In_ProcessSoftTimer);
}

/*************************************************************************
*                  线程创建、各种线程通讯方式的创建函数
*************************************************************************/
void TaskInit(void)
{
	uint8_t TaskThreadTndex = 0;
  
	Inspection_event = rt_event_create("Inspection_event", RT_IPC_FLAG_FIFO);
  
	exit_recv_sem = rt_sem_create(	"exit_recv_sem",  //信号量名字
									0,                  //信号量初始值
									RT_IPC_FLAG_FIFO);    //信号量模式 FIFO(0x00)
	
	entr_recv_sem = rt_sem_create(	"entr_recv_sem",  //信号量名字
									0,                  //信号量初始值
									RT_IPC_FLAG_FIFO);   //信号量模式 FIFO(0x00)
	                          
	Inspection_ProcessSoftTimer = rt_timer_create(	"Inspection_ProcessSoftTimer", /* 软件定时器的名称 */
													Inspection_ProcessSoftTimer_callback,/* 软件定时器的回调函数 */
													0,			/* 定时器超时函数的入口参数 */
													500,   /* 软件定时器的超时时间(周期回调时间)单位毫秒 */
													RT_TIMER_FLAG_PERIODIC );
	while(1)
	{
		if(strcmp(TaskThread[TaskThreadTndex].name,"") != 0 )
		{
			message_thread = rt_thread_create(	TaskThread[TaskThreadTndex].name,   /*线程名称*/
												TaskThread[TaskThreadTndex].entry, /*线程入口函数名*/
												TaskThread[TaskThreadTndex].parameter,      /*线程入口函数参数*/
												TaskThread[TaskThreadTndex].stack_size,      /*线程栈大小*/
												TaskThread[TaskThreadTndex].priority,        /*线程的优先级*/
												TaskThread[TaskThreadTndex].tick);      /*线程时间片*/
			if(message_thread != RT_NULL){rt_thread_startup(message_thread);} 
			TaskThreadTndex ++; 
		}else{break;}
	}
}

/*************************************************************************
*                       计算停车场的四种计数的函数
*************************************************************************/
static void Vehicle_count(void)
{
  char counternum[2]={0};//存储数字的每位数值
  
  counternum[0]=entr_num/10+'0';//十位的数值
  counternum[1]=entr_num%10+'0';//个位的数值
  Show_Str(180,80,RED,WHITE,counternum,24,0);//更新驶入车辆数值显示

  counternum[0]=exit_num/10+'0';//十位的数值
  counternum[1]=exit_num%10+'0';//个位的数值
  Show_Str(180,107,RED,WHITE,counternum,24,0); //更新驶出车辆数值显示

  Detained_vehicle=entr_num-exit_num;//计算滞留车辆数目
  counternum[0]=((Detained_vehicle+Stranded_Init)%100)/10+'0';////十位的数值
  counternum[1]=(Detained_vehicle+Stranded_Init)%10+'0';//个位的数值
  Show_Str(180,134,RED,WHITE,counternum,24,0);//更新滞留车辆数值
  
  counternum[0]=((Total_P-Detained_vehicle)%100)/10+'0';//计算剩余车位数的十位的数值
  counternum[1]=(Total_P-Detained_vehicle)%10+'0';//计算剩余车位数的个位的数值
  Show_Str(180,161,RED,WHITE,counternum,24,0);//更新剩余车位数值
}
