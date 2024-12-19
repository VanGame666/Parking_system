#include "config.h"
#include "Parking_detection_processing.h"


extern uint8_t OV7725_entr_vsync;
extern uint8_t OV7725_exit_vsync;

extern uint16_t Detained_vehicle;//滞留车辆数目
static uint8_t PS_A_Recommend=0,PS_B_Recommend=0,Last_PSA_Recommend=0,Last_PSB_Recommend=0;//记录车位推荐情况
static uint8_t Last_PSA_use=15,Last_PSB_use=15;//记录车位停放情况
uint8_t Update_P=1;//标记是否需要更新显示车位信息:0为不需要，1为需要
uint8_t  sign_ps=0;//标记车位是否已满
//-----------------------------------------------------------------------------------------------
// 按键线程相关参数宏定义
#define BUTTON_THREAD_STACK_SIZE 256
#define BUTTON_THREAD_PRIORITY   5

//-----------------------------------------------------------------------------------------------
// 按键事件队列
static rt_mq_t button_mq = RT_NULL;//按键事件队列定义

//-----------------------------------------------------------------------------------------------
// 按键定义

/*进出口的四个红外蔽障模块*/
static Button_t s_tBtnKey1;//入口检测是否有车			
static Button_t s_tBtnKey2;//入口判断车是否完全驶入停车场		
static Button_t s_tBtnKey3;//出口检测是否有车	
static Button_t s_tBtnKey4;//出口判断车是否完全驶出停车场
/*车位上的两个红外蔽障模块*/
static Button_t s_tBtnKey5;//A区01号车位
static Button_t s_tBtnKey6;//B区01号车位
/*烟雾传感器模块*/
static Button_t s_tBtnKey7;//A区的烟雾警报器
                 

//-----------------------------------------------------------------------------------------------
// 模块检测到时的电平，=0,按下时为低电平;=1,按下时为高电平
#define KeyPressedLevel 0
//-----------------------------------------------------------------------------------------------
// 获取模块检测到的函数	
/*进出口的四个红外蔽障模块*/
static unsigned char IsKey1Down(void) 		{if (Key1In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey2Down(void) 		{if (Key2In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey3Down(void) 		{if (Key3In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey4Down(void) 		{if (Key4In != KeyPressedLevel) return 0; return 1;}
/*车位上的两个红外蔽障模块*/
static unsigned char IsKey5Down(void) 		{if (A1PS != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey6Down(void) 		{if (B1PS != KeyPressedLevel) return 0; return 1;}
/*烟雾传感器模块*/
static unsigned char IsKey7Down(void) 		{if (Smoke_A != KeyPressedLevel) return 0; return 1;}

/*******************************************************************************************************
 * 函数: ButtonVarInit,初始化各模块变量
 * 参数: void
 * 返回: void
 *******************************************************************************************************/
static void ButtonVarInit(void)
{
	s_tBtnKey1.IsKeyDownFunc=IsKey1Down;							// 检测按键按下函数
	s_tBtnKey1.wFilterCount=0;										// 滤波器计数器 
	s_tBtnKey1.wFilterTime =BUTTON_FILTER_TIME;						// 滤波时间 
	s_tBtnKey1.wLongCount =0;										// 长按计数器 
	s_tBtnKey1.wLongTime=BUTTON_LONG_TIME;							// 长按时间 	
	s_tBtnKey1.byState=0;											// 按键当前状态（按下还是弹起）
	s_tBtnKey1.byKeyCodeUp=KEY1_UP;									// 按键弹起的键值代码, 0表示不检测按键弹起 
	s_tBtnKey1.byKeyCodeDown=KEY1_DOWN;								// 按键按下的键值代码, 0表示不检测按键按下
	
	s_tBtnKey2.IsKeyDownFunc=IsKey2Down;						
	s_tBtnKey2.wFilterCount=0;									
	s_tBtnKey2.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey2.wLongCount =0;									
	s_tBtnKey2.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey2.byState=0;						
	s_tBtnKey2.byKeyCodeUp=KEY2_UP;								
	s_tBtnKey2.byKeyCodeDown=KEY2_DOWN;							

	s_tBtnKey3.IsKeyDownFunc=IsKey3Down;						
	s_tBtnKey3.wFilterCount=0;									
	s_tBtnKey3.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey3.wLongCount =0;									
	s_tBtnKey3.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey3.byState=0;										
	s_tBtnKey3.byKeyCodeUp=KEY3_UP;								
	s_tBtnKey3.byKeyCodeDown=KEY3_DOWN;							
  
  s_tBtnKey4.IsKeyDownFunc=IsKey4Down;							
	s_tBtnKey4.wFilterCount=0;									
	s_tBtnKey4.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey4.wLongCount =0;									
	s_tBtnKey4.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey4.byState=0;										
	s_tBtnKey4.byKeyCodeUp=KEY4_UP;								
	s_tBtnKey4.byKeyCodeDown=KEY4_DOWN;							
  
  s_tBtnKey5.IsKeyDownFunc=IsKey5Down;							
	s_tBtnKey5.wFilterCount=0;									
	s_tBtnKey5.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey5.wLongCount =0;									
	s_tBtnKey5.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey5.byState=0;										
	s_tBtnKey5.byKeyCodeUp=0;									
	s_tBtnKey5.byKeyCodeDown=0;	
  
  s_tBtnKey6.IsKeyDownFunc=IsKey6Down;							
	s_tBtnKey6.wFilterCount=0;									
	s_tBtnKey6.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey6.wLongCount =0;									
	s_tBtnKey6.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey6.byState=0;										
	s_tBtnKey6.byKeyCodeUp=0;									
	s_tBtnKey6.byKeyCodeDown=0;							
  
  s_tBtnKey7.IsKeyDownFunc=IsKey7Down;							
	s_tBtnKey7.wFilterCount=0;									
	s_tBtnKey7.wFilterTime =BUTTON_FILTER_TIME;					
	s_tBtnKey7.wLongCount =0;									
	s_tBtnKey7.wLongTime=BUTTON_LONG_TIME;						
	s_tBtnKey7.byState=0;										
	s_tBtnKey7.byKeyCodeUp=SmokeA_UP;							
	s_tBtnKey7.byKeyCodeDown=SmokeA_DOWN;						

}

/*******************************************************************************************************
 * 函数: ButtonDetect,进出口的模块检测函数
 * 参数: ptBtn 按键结构体指针
 * 返回: void
 *******************************************************************************************************/
static void ButtonDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// 检测按键函数是否存在，按键是否按下
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// 滤波检测，滤波计数器到达滤波时间
		{
			ptBtn->wFilterCount++;									// 计数器加一
			return;													// 退出检测函数
		}
		
		if(ptBtn->byState ==0 ) 									// 检测是否是按键按下							
		{
			ptBtn->byState = 1;
			rt_mq_send(button_mq,										// 写入（发送）队列的ID(句柄)
						&(ptBtn->byKeyCodeDown),  				// 写入（发送）的数据所对应地址 
						sizeof(ptBtn->byKeyCodeDown)			// 数据的长度 
						);
			return;
		}		
	}
	else 
	{
		if(ptBtn->wFilterCount) 									// 滤波检测，滤波计数器是否为0
		{
			ptBtn->wFilterCount--;									// 计数器减一
			return;													// 退出检测函数
		}
		
		if(ptBtn->byState ==1 )										// 检测是否是按键弹起
		{
			ptBtn->byState =0; 										
			rt_mq_send(button_mq,										// 写入（发送）队列的ID(句柄)
						&(ptBtn->byKeyCodeUp),  				// 写入（发送）的数据所对应地址 
						sizeof(ptBtn->byKeyCodeUp)			// 数据的长度 
						);
		}
		
		ptBtn->wLongCount = 0;										// 按键长按计数器清零
	}
}

/*******************************************************************************************************
 * 函数: ButtonDetect,车位的模块检测函数
 * 参数: ptBtn 按键结构体指针
 * 返回: void
 *******************************************************************************************************/
static void ParkingDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// 检测按键函数是否存在，按键是否按下
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// 滤波检测，滤波计数器到达滤波时间
		{
			ptBtn->wFilterCount++;									// 计数器加一
			return;													// 退出检测函数
		}
		
		if(ptBtn->byState ==0 ) 									// 检测是否是按键按下							
		{
			ptBtn->byState = 1;
			return;
		}		
	}
	else 
	{		
		if(ptBtn->wFilterCount) 									// 滤波检测，滤波计数器是否为0
		{
			ptBtn->wFilterCount--;									// 计数器减一
			return;													// 退出检测函数
		}			
		
		if(ptBtn->byState ==1 )										// 检测是否是按键弹起
		{
			ptBtn->byState =0; 										
		}
		
		ptBtn->wLongCount = 0;										// 按键长按计数器清零
	}
}

/*******************************************************************************************************
 *函数: ButtonDetect,烟雾传感器模块检测函数
 *参数: ptBtn 按键结构体指针
 *返回: void
 *******************************************************************************************************/
static void UrgentDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// 检测按键函数是否存在，按键是否按下
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// 滤波检测，滤波计数器到达滤波时间
		{
			ptBtn->wFilterCount++;									// 计数器加一
			return;													// 退出检测函数
		}
		
		if(ptBtn->byState ==0 ) 									// 检测是否是按键按下							
		{
			ptBtn->byState = 1;
			rt_mq_urgent(button_mq,										// 写入（发送）队列的ID(句柄)
								&(ptBtn->byKeyCodeDown),  				// 写入（发送）的数据所对应地址 
								sizeof(ptBtn->byKeyCodeDown)			// 数据的长度 
								);
			return;
		}		
	}
	else 
	{		
		if(ptBtn->wFilterCount) 									// 滤波检测，滤波计数器是否为0
		{
			ptBtn->wFilterCount--;									// 计数器减一
			return;													// 退出检测函数
		}			
		
		if(ptBtn->byState ==1 )										// 检测是否是按键弹起
		{
			ptBtn->byState =0; 										
			rt_mq_urgent(button_mq,										// 写入（发送）队列的ID(句柄)
								&(ptBtn->byKeyCodeUp),  				// 写入（发送）的数据所对应地址 
								sizeof(ptBtn->byKeyCodeUp)			// 数据的长度 
								);
		}
		
		ptBtn->wLongCount = 0;										// 按键长按计数器清零
	}
}

/*******************************************************************************************************
 * 函数: ButtonInit,Button初始化
 * 参数: void
 * 返回: void
 *******************************************************************************************************/
void ButtonInit(void)
{
  Smoke_Init();                           //烟雾模块GPIO初始化
	Infrared_Init();												// 红外蔽障模块GPIO初始化
	ButtonVarInit();												// 模块配置初始化
}



/********************************************************************
 * 函数名:Parking_recommendation
 * 描述  :车位推荐功能
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/	
void Parking_recommendation(void)
{
  uint8_t i;
  char cstr[3]={0};
  
  for(i=0;i<PS_Total;i++)//A区车位推荐
  {
    if((Last_PSA_use&(1<<i))==0)//检测A区域未停放的车位
    {    
      if((PS_A_Recommend&(1<<i))==0)//检测A区域未被推荐的车位
      {
        sprintf(cstr,"A0%d",1<<i);
        PS_A_Recommend |=(1<<i);//推荐车位进行标记
        Show_Str(148,320,RED,WHITE,cstr,24,0);//液晶屏显示推荐车位的编号
        return ;
      }
    }
  }
  
  for(i=0;i<PS_Total;i++)//B区车位推荐
  {
    if((Last_PSB_use&(1<<i))==0)//检测B区域未停放的车位
    {    
      if((PS_B_Recommend&(1<<i))==0)//检测B区域未被推荐的车位
      {
        sprintf(cstr,"B0%d",1<<i);
        PS_B_Recommend |=(1<<i);//推荐车位进行标记
        Show_Str(148,320,RED,WHITE,cstr,24,0);//液晶屏显示推荐车位的编号
        return ;
      }
    }
  }
}

/********************************************************************
 * 函数名:Parking_management
 * 描述  :车位检测及车位信息管理
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/	
void Parking_management(void)
{
  uint8_t i,PS_U_num=0,PS_A_use=0,PS_B_use=0;
  char cstr[2]={0};
      /*车位检测*/
  {
    if(s_tBtnKey5.byState == 1)//检测A01车位
    {
      PS_U_num++;//累计车位已使用的数目
      PS_A_use |=(1<<0);//A区域的车位使用标记进行标记
      PS_A_Recommend &=~(1<<0);//A区域的车位推荐标记清除
    }
    if(s_tBtnKey6.byState == 1)//检测B01车位
    {
      PS_U_num++;//累计车位已使用的数目
      PS_B_use |=(1<<0);//B区域的车位使用标记进行标记
      PS_B_Recommend &=~(1<<0);//B区域的车位推荐标记清除
    }
  }
  /***************************************************************/   
  for(i=0;i<PS_Total;i++)//计算车位处在推荐的个数
  {
    if((PS_A_Recommend&(1<<i))!=0)//A区域的车位推荐个数
        PS_U_num++;//累计车位已使用的数目
    if((PS_B_Recommend&(1<<i))!=0)//B区域的车位推荐个数
        PS_U_num++; //累计车位已使用的数目   
  }
  
  /**************************************************************/ 
  if(Detained_vehicle < PS_U_num)//累计使用车位（含已被推荐）与滞留车辆数比较
  {
    PS_U_num =PS_U_num - Detained_vehicle; //使用车位（含已被推荐） 中推荐有多余，因为有车乱停
    /*由于有车乱停，所以要去除多余的还是推荐车位，采用从A区域低编号开始*/
    for(i=0;i<PS_Total;i++)
    {
      if((PS_A_Recommend &(1<<i))!=0)
      {
        PS_A_Recommend &=~(1<<i);//A区域的车位推荐标记清除
        PS_U_num--;
      }
      if(PS_U_num==0)//累计使用车位（含已被推荐）与滞留车辆数相等
        break;
    }
    if(PS_U_num!=0)
      for(i=0;i<PS_Total;i++)
      {
        if((PS_B_Recommend &(1<<i))!=0)
        {
          PS_B_Recommend &=~(1<<i);//B区域的车位推荐标记清除
          PS_U_num--;
        }
        if(PS_U_num==0)//累计使用车位（含已被推荐）与滞留车辆数相等
          break;
      }
     PS_U_num =Detained_vehicle;    
  }
  
  if(Total_P==PS_U_num)//判断车位是否已满进行标记
    sign_ps=1;
  else
    sign_ps=0;
  
  /**********************************************************************/ 
  if(Last_PSA_use!=PS_A_use || Last_PSB_use!=PS_B_use || Update_P == 1)//车位停放信息情况有变化
  {
    macESP8266_Usart ( "GA%d,B%d,\r\n",PS_A_use,PS_B_use);//车位信息已发送
    
    Last_PSA_use=PS_A_use;
    
    for(i=0;i<PS_Total;i++)//液晶屏显示车位信息
    {
      if((PS_A_use&(1<<i))==1)//该车位有车停放
          POINT_COLOR=RED;
      else//该车位没车
          POINT_COLOR=GREEN;   
     
      DrawSolidRectangle(50+55*i,240,85+55*i,295,POINT_COLOR);
      sprintf(cstr,"0%d",i+1);
      Show_Str(51+55*i,255,BLACK,GREEN,cstr,24,1);//显示车位编号
    }

    Last_PSB_use=PS_B_use;
    for(i=0;i<PS_Total;i++)//液晶屏显示车位信息
    {
      if((PS_B_use&(1<<i))==1)//该车位有车停放
          POINT_COLOR=RED;          
      else//该车位没车
          POINT_COLOR=GREEN;   
      
      DrawSolidRectangle(210+55*i,240,245+55*i,295,POINT_COLOR);
      sprintf(cstr,"0%d",i+1);
      Show_Str(211+55*i,255,BLACK,GREEN,cstr,24,1);//显示车位编号
    }  
  }
  
  /*车位是否有新的车位被推荐出去进行液晶屏显示*/
  {  
    if((PS_A_Recommend!=0 && Last_PSA_Recommend!=PS_A_Recommend) || Update_P == 1)//A区域的车位是否有新的车位被推荐出去
    {
      for(i=0;i<PS_Total;i++)//液晶屏显示车位推荐信息
      {
        if((PS_A_Recommend&(1<<i))==1)
        {
          DrawSolidRectangle(50+55*i,240,85+55*i,295,YELLOW);
          sprintf(cstr,"0%d",i+1);
          Show_Str(51+55*i,255,BLACK,GREEN,cstr,24,1);//显示车位编号       
        }
      }
    }
    
    if((PS_B_Recommend!=0 && Last_PSB_Recommend!=PS_B_Recommend) || Update_P == 1)//B区域的车位是否有新的车位被推荐出去
    {
      for(i=0;i<PS_Total;i++)//液晶屏显示车位推荐信息
      { 
        if((PS_B_Recommend&(1<<i))==1)
        {
          DrawSolidRectangle(210+55*i,240,245+55*i,295,YELLOW);
          sprintf(cstr,"0%d",i+1);
          Show_Str(211+55*i,255,BLACK,GREEN,cstr,24,1); //显示车位编号         
        }    
      }   
    }
  }
  
  Update_P = 0;
  /*记录目前车位推荐情况*/
  Last_PSA_Recommend=PS_A_Recommend;
  Last_PSB_Recommend=PS_B_Recommend;
}

/*******************************************************************************************************
 * 函数: ButtonProj,模块处理（扫描）函数
 * 参数: void
 * 返回: void
 *******************************************************************************************************/
void ButtonProj(void)
{
  /*进出口的四个红外蔽障模块检测*/
	ButtonDetect(&s_tBtnKey1);										
	ButtonDetect(&s_tBtnKey2);										
	ButtonDetect(&s_tBtnKey3);										
	ButtonDetect(&s_tBtnKey4);										
	ParkingDetect(&s_tBtnKey5);										
	ParkingDetect(&s_tBtnKey6);										
	UrgentDetect(&s_tBtnKey7);
}

/*************************************************************************
*                             软件定时器回调函数定义
*************************************************************************/
static void In_ProcessSoftTimer_callback(void* parameter)
{ 
  ButtonProj();//各模块扫描，定时中断周期为1ms
}

/*******************************************************************************************************
 * 函数: button_thread_entry,获取各模块事件并进行处理
 * 参数: void
 * 返回: void
 *******************************************************************************************************/
void button_thread_entry(void *parameter)//用户消息处理入口函数
{
  static char Smoke[5]={"A0000"};//烟雾警报发送上位机数据存储
	rt_err_t uwRet = RT_EOK;
	uint8_t r_queue;//用于接收msg_mq消息队列信息
	
	button_mq = rt_mq_create("button_mq",							//消息队列名字
							1,  									//消息的最大长度, bytes
							10,										//消息队列的最大容量(个数)
							RT_IPC_FLAG_FIFO);						//队列模式 FIFO
						
	if(button_mq != RT_NULL)rt_kprintf("button_mq create success\n\n");
	ButtonInit();//按键硬件接口初始化
	while(1)
	{  //获取队列信息
		 uwRet = rt_mq_recv(button_mq,
							&r_queue,
							sizeof(r_queue),
							RT_WAITING_FOREVER);
		 if(RT_EOK == uwRet )
		 {
			 switch(r_queue)//根据接收到的消息内容分别进行处理
			 {         
				 case KEY1_DOWN:if(sign_ps==0)OV7725_entr_vsync=0;break;//有车要进停车场
				 case KEY1_UP:rt_event_send(Inspection_event, EVENT_FLAG0);if(Key2In)TIM_SetCompare4(TIM3,1400);break;//车基本驶入停车场或车已离开入口

				 case KEY2_DOWN:break;
				 case KEY2_UP:TIM_SetCompare4(TIM3,1400);break;//车完全驶入停车场

				 case KEY3_DOWN:OV7725_exit_vsync = 0;break;//有车要驶出停车场
				 case KEY3_UP:rt_event_send(Inspection_event, EVENT_FLAG1);if(Key4In)TIM_SetCompare3(TIM3,1600);break;//车基本驶出停车场
         
				 case KEY4_DOWN:break;
				 case KEY4_UP:TIM_SetCompare3(TIM3,1600);break;//车完全驶出停车场
         
				 case 	SmokeA_DOWN:     
						Smoke[1]='1';//更新烟雾警报信息，该区域有浓烟 
						macESP8266_Usart("%s\r\n",Smoke);//向上位机发送烟雾警报信息          
						DrawSolidCircular(52,420,20,RED);//液晶屏上的烟雾警报灯点亮
							break;
				 case 	SmokeA_UP:
                        Smoke[1]='0';//更新烟雾警报信息，该区域没有浓烟 
                        macESP8266_Usart("%s\r\n",Smoke);//向上位机发送烟雾警报信息          
                        DrawSolidCircular(52,420,20,BROWN);//液晶屏上的烟雾警报灯熄灭
                        break;

				 default:rt_kprintf("No button Message!\n\n");break;
			 }			
		 }
		 else
		 {
			 rt_kprintf("数据接收错误，错误代码:0x%lx\n\n",uwRet);
		 }
	}	
}
int button_process_init(void)
{
    rt_thread_t tid;
  
	In_ProcessSoftTimer =	rt_timer_create("In_ProcessSoftTimer", //软件定时器的名称
							In_ProcessSoftTimer_callback,//软件定时器的回调函数
							0,			//定时器超时函数的入口参数
							1,   //软件定时器的超时时间(周期回调时间)
							RT_TIMER_FLAG_PERIODIC );
							//软件定时器模式 周期模式

    tid = rt_thread_create("button_process",
                           button_thread_entry, RT_NULL,
                           BUTTON_THREAD_STACK_SIZE, BUTTON_THREAD_PRIORITY, 10);

    if (tid != NULL)rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(button_process_init);




