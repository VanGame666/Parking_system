#include "config.h"
#include "Parking_detection_processing.h"


extern uint8_t OV7725_entr_vsync;
extern uint8_t OV7725_exit_vsync;

extern uint16_t Detained_vehicle;//����������Ŀ
static uint8_t PS_A_Recommend=0,PS_B_Recommend=0,Last_PSA_Recommend=0,Last_PSB_Recommend=0;//��¼��λ�Ƽ����
static uint8_t Last_PSA_use=15,Last_PSB_use=15;//��¼��λͣ�����
uint8_t Update_P=1;//����Ƿ���Ҫ������ʾ��λ��Ϣ:0Ϊ����Ҫ��1Ϊ��Ҫ
uint8_t  sign_ps=0;//��ǳ�λ�Ƿ�����
//-----------------------------------------------------------------------------------------------
// �����߳���ز����궨��
#define BUTTON_THREAD_STACK_SIZE 256
#define BUTTON_THREAD_PRIORITY   5

//-----------------------------------------------------------------------------------------------
// �����¼�����
static rt_mq_t button_mq = RT_NULL;//�����¼����ж���

//-----------------------------------------------------------------------------------------------
// ��������

/*�����ڵ��ĸ��������ģ��*/
static Button_t s_tBtnKey1;//��ڼ���Ƿ��г�			
static Button_t s_tBtnKey2;//����жϳ��Ƿ���ȫʻ��ͣ����		
static Button_t s_tBtnKey3;//���ڼ���Ƿ��г�	
static Button_t s_tBtnKey4;//�����жϳ��Ƿ���ȫʻ��ͣ����
/*��λ�ϵ������������ģ��*/
static Button_t s_tBtnKey5;//A��01�ų�λ
static Button_t s_tBtnKey6;//B��01�ų�λ
/*��������ģ��*/
static Button_t s_tBtnKey7;//A������������
                 

//-----------------------------------------------------------------------------------------------
// ģ���⵽ʱ�ĵ�ƽ��=0,����ʱΪ�͵�ƽ;=1,����ʱΪ�ߵ�ƽ
#define KeyPressedLevel 0
//-----------------------------------------------------------------------------------------------
// ��ȡģ���⵽�ĺ���	
/*�����ڵ��ĸ��������ģ��*/
static unsigned char IsKey1Down(void) 		{if (Key1In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey2Down(void) 		{if (Key2In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey3Down(void) 		{if (Key3In != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey4Down(void) 		{if (Key4In != KeyPressedLevel) return 0; return 1;}
/*��λ�ϵ������������ģ��*/
static unsigned char IsKey5Down(void) 		{if (A1PS != KeyPressedLevel) return 0; return 1;}
static unsigned char IsKey6Down(void) 		{if (B1PS != KeyPressedLevel) return 0; return 1;}
/*��������ģ��*/
static unsigned char IsKey7Down(void) 		{if (Smoke_A != KeyPressedLevel) return 0; return 1;}

/*******************************************************************************************************
 * ����: ButtonVarInit,��ʼ����ģ�����
 * ����: void
 * ����: void
 *******************************************************************************************************/
static void ButtonVarInit(void)
{
	s_tBtnKey1.IsKeyDownFunc=IsKey1Down;							// ��ⰴ�����º���
	s_tBtnKey1.wFilterCount=0;										// �˲��������� 
	s_tBtnKey1.wFilterTime =BUTTON_FILTER_TIME;						// �˲�ʱ�� 
	s_tBtnKey1.wLongCount =0;										// ���������� 
	s_tBtnKey1.wLongTime=BUTTON_LONG_TIME;							// ����ʱ�� 	
	s_tBtnKey1.byState=0;											// ������ǰ״̬�����»��ǵ���
	s_tBtnKey1.byKeyCodeUp=KEY1_UP;									// ��������ļ�ֵ����, 0��ʾ����ⰴ������ 
	s_tBtnKey1.byKeyCodeDown=KEY1_DOWN;								// �������µļ�ֵ����, 0��ʾ����ⰴ������
	
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
 * ����: ButtonDetect,�����ڵ�ģ���⺯��
 * ����: ptBtn �����ṹ��ָ��
 * ����: void
 *******************************************************************************************************/
static void ButtonDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// ��ⰴ�������Ƿ���ڣ������Ƿ���
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// �˲���⣬�˲������������˲�ʱ��
		{
			ptBtn->wFilterCount++;									// ��������һ
			return;													// �˳���⺯��
		}
		
		if(ptBtn->byState ==0 ) 									// ����Ƿ��ǰ�������							
		{
			ptBtn->byState = 1;
			rt_mq_send(button_mq,										// д�루���ͣ����е�ID(���)
						&(ptBtn->byKeyCodeDown),  				// д�루���ͣ�����������Ӧ��ַ 
						sizeof(ptBtn->byKeyCodeDown)			// ���ݵĳ��� 
						);
			return;
		}		
	}
	else 
	{
		if(ptBtn->wFilterCount) 									// �˲���⣬�˲��������Ƿ�Ϊ0
		{
			ptBtn->wFilterCount--;									// ��������һ
			return;													// �˳���⺯��
		}
		
		if(ptBtn->byState ==1 )										// ����Ƿ��ǰ�������
		{
			ptBtn->byState =0; 										
			rt_mq_send(button_mq,										// д�루���ͣ����е�ID(���)
						&(ptBtn->byKeyCodeUp),  				// д�루���ͣ�����������Ӧ��ַ 
						sizeof(ptBtn->byKeyCodeUp)			// ���ݵĳ��� 
						);
		}
		
		ptBtn->wLongCount = 0;										// ������������������
	}
}

/*******************************************************************************************************
 * ����: ButtonDetect,��λ��ģ���⺯��
 * ����: ptBtn �����ṹ��ָ��
 * ����: void
 *******************************************************************************************************/
static void ParkingDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// ��ⰴ�������Ƿ���ڣ������Ƿ���
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// �˲���⣬�˲������������˲�ʱ��
		{
			ptBtn->wFilterCount++;									// ��������һ
			return;													// �˳���⺯��
		}
		
		if(ptBtn->byState ==0 ) 									// ����Ƿ��ǰ�������							
		{
			ptBtn->byState = 1;
			return;
		}		
	}
	else 
	{		
		if(ptBtn->wFilterCount) 									// �˲���⣬�˲��������Ƿ�Ϊ0
		{
			ptBtn->wFilterCount--;									// ��������һ
			return;													// �˳���⺯��
		}			
		
		if(ptBtn->byState ==1 )										// ����Ƿ��ǰ�������
		{
			ptBtn->byState =0; 										
		}
		
		ptBtn->wLongCount = 0;										// ������������������
	}
}

/*******************************************************************************************************
 *����: ButtonDetect,��������ģ���⺯��
 *����: ptBtn �����ṹ��ָ��
 *����: void
 *******************************************************************************************************/
static void UrgentDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// ��ⰴ�������Ƿ���ڣ������Ƿ���
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime)				// �˲���⣬�˲������������˲�ʱ��
		{
			ptBtn->wFilterCount++;									// ��������һ
			return;													// �˳���⺯��
		}
		
		if(ptBtn->byState ==0 ) 									// ����Ƿ��ǰ�������							
		{
			ptBtn->byState = 1;
			rt_mq_urgent(button_mq,										// д�루���ͣ����е�ID(���)
								&(ptBtn->byKeyCodeDown),  				// д�루���ͣ�����������Ӧ��ַ 
								sizeof(ptBtn->byKeyCodeDown)			// ���ݵĳ��� 
								);
			return;
		}		
	}
	else 
	{		
		if(ptBtn->wFilterCount) 									// �˲���⣬�˲��������Ƿ�Ϊ0
		{
			ptBtn->wFilterCount--;									// ��������һ
			return;													// �˳���⺯��
		}			
		
		if(ptBtn->byState ==1 )										// ����Ƿ��ǰ�������
		{
			ptBtn->byState =0; 										
			rt_mq_urgent(button_mq,										// д�루���ͣ����е�ID(���)
								&(ptBtn->byKeyCodeUp),  				// д�루���ͣ�����������Ӧ��ַ 
								sizeof(ptBtn->byKeyCodeUp)			// ���ݵĳ��� 
								);
		}
		
		ptBtn->wLongCount = 0;										// ������������������
	}
}

/*******************************************************************************************************
 * ����: ButtonInit,Button��ʼ��
 * ����: void
 * ����: void
 *******************************************************************************************************/
void ButtonInit(void)
{
  Smoke_Init();                           //����ģ��GPIO��ʼ��
	Infrared_Init();												// �������ģ��GPIO��ʼ��
	ButtonVarInit();												// ģ�����ó�ʼ��
}



/********************************************************************
 * ������:Parking_recommendation
 * ����  :��λ�Ƽ�����
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/	
void Parking_recommendation(void)
{
  uint8_t i;
  char cstr[3]={0};
  
  for(i=0;i<PS_Total;i++)//A����λ�Ƽ�
  {
    if((Last_PSA_use&(1<<i))==0)//���A����δͣ�ŵĳ�λ
    {    
      if((PS_A_Recommend&(1<<i))==0)//���A����δ���Ƽ��ĳ�λ
      {
        sprintf(cstr,"A0%d",1<<i);
        PS_A_Recommend |=(1<<i);//�Ƽ���λ���б��
        Show_Str(148,320,RED,WHITE,cstr,24,0);//Һ������ʾ�Ƽ���λ�ı��
        return ;
      }
    }
  }
  
  for(i=0;i<PS_Total;i++)//B����λ�Ƽ�
  {
    if((Last_PSB_use&(1<<i))==0)//���B����δͣ�ŵĳ�λ
    {    
      if((PS_B_Recommend&(1<<i))==0)//���B����δ���Ƽ��ĳ�λ
      {
        sprintf(cstr,"B0%d",1<<i);
        PS_B_Recommend |=(1<<i);//�Ƽ���λ���б��
        Show_Str(148,320,RED,WHITE,cstr,24,0);//Һ������ʾ�Ƽ���λ�ı��
        return ;
      }
    }
  }
}

/********************************************************************
 * ������:Parking_management
 * ����  :��λ��⼰��λ��Ϣ����
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/	
void Parking_management(void)
{
  uint8_t i,PS_U_num=0,PS_A_use=0,PS_B_use=0;
  char cstr[2]={0};
      /*��λ���*/
  {
    if(s_tBtnKey5.byState == 1)//���A01��λ
    {
      PS_U_num++;//�ۼƳ�λ��ʹ�õ���Ŀ
      PS_A_use |=(1<<0);//A����ĳ�λʹ�ñ�ǽ��б��
      PS_A_Recommend &=~(1<<0);//A����ĳ�λ�Ƽ�������
    }
    if(s_tBtnKey6.byState == 1)//���B01��λ
    {
      PS_U_num++;//�ۼƳ�λ��ʹ�õ���Ŀ
      PS_B_use |=(1<<0);//B����ĳ�λʹ�ñ�ǽ��б��
      PS_B_Recommend &=~(1<<0);//B����ĳ�λ�Ƽ�������
    }
  }
  /***************************************************************/   
  for(i=0;i<PS_Total;i++)//���㳵λ�����Ƽ��ĸ���
  {
    if((PS_A_Recommend&(1<<i))!=0)//A����ĳ�λ�Ƽ�����
        PS_U_num++;//�ۼƳ�λ��ʹ�õ���Ŀ
    if((PS_B_Recommend&(1<<i))!=0)//B����ĳ�λ�Ƽ�����
        PS_U_num++; //�ۼƳ�λ��ʹ�õ���Ŀ   
  }
  
  /**************************************************************/ 
  if(Detained_vehicle < PS_U_num)//�ۼ�ʹ�ó�λ�����ѱ��Ƽ����������������Ƚ�
  {
    PS_U_num =PS_U_num - Detained_vehicle; //ʹ�ó�λ�����ѱ��Ƽ��� ���Ƽ��ж��࣬��Ϊ�г���ͣ
    /*�����г���ͣ������Ҫȥ������Ļ����Ƽ���λ�����ô�A����ͱ�ſ�ʼ*/
    for(i=0;i<PS_Total;i++)
    {
      if((PS_A_Recommend &(1<<i))!=0)
      {
        PS_A_Recommend &=~(1<<i);//A����ĳ�λ�Ƽ�������
        PS_U_num--;
      }
      if(PS_U_num==0)//�ۼ�ʹ�ó�λ�����ѱ��Ƽ������������������
        break;
    }
    if(PS_U_num!=0)
      for(i=0;i<PS_Total;i++)
      {
        if((PS_B_Recommend &(1<<i))!=0)
        {
          PS_B_Recommend &=~(1<<i);//B����ĳ�λ�Ƽ�������
          PS_U_num--;
        }
        if(PS_U_num==0)//�ۼ�ʹ�ó�λ�����ѱ��Ƽ������������������
          break;
      }
     PS_U_num =Detained_vehicle;    
  }
  
  if(Total_P==PS_U_num)//�жϳ�λ�Ƿ��������б��
    sign_ps=1;
  else
    sign_ps=0;
  
  /**********************************************************************/ 
  if(Last_PSA_use!=PS_A_use || Last_PSB_use!=PS_B_use || Update_P == 1)//��λͣ����Ϣ����б仯
  {
    macESP8266_Usart ( "GA%d,B%d,\r\n",PS_A_use,PS_B_use);//��λ��Ϣ�ѷ���
    
    Last_PSA_use=PS_A_use;
    
    for(i=0;i<PS_Total;i++)//Һ������ʾ��λ��Ϣ
    {
      if((PS_A_use&(1<<i))==1)//�ó�λ�г�ͣ��
          POINT_COLOR=RED;
      else//�ó�λû��
          POINT_COLOR=GREEN;   
     
      DrawSolidRectangle(50+55*i,240,85+55*i,295,POINT_COLOR);
      sprintf(cstr,"0%d",i+1);
      Show_Str(51+55*i,255,BLACK,GREEN,cstr,24,1);//��ʾ��λ���
    }

    Last_PSB_use=PS_B_use;
    for(i=0;i<PS_Total;i++)//Һ������ʾ��λ��Ϣ
    {
      if((PS_B_use&(1<<i))==1)//�ó�λ�г�ͣ��
          POINT_COLOR=RED;          
      else//�ó�λû��
          POINT_COLOR=GREEN;   
      
      DrawSolidRectangle(210+55*i,240,245+55*i,295,POINT_COLOR);
      sprintf(cstr,"0%d",i+1);
      Show_Str(211+55*i,255,BLACK,GREEN,cstr,24,1);//��ʾ��λ���
    }  
  }
  
  /*��λ�Ƿ����µĳ�λ���Ƽ���ȥ����Һ������ʾ*/
  {  
    if((PS_A_Recommend!=0 && Last_PSA_Recommend!=PS_A_Recommend) || Update_P == 1)//A����ĳ�λ�Ƿ����µĳ�λ���Ƽ���ȥ
    {
      for(i=0;i<PS_Total;i++)//Һ������ʾ��λ�Ƽ���Ϣ
      {
        if((PS_A_Recommend&(1<<i))==1)
        {
          DrawSolidRectangle(50+55*i,240,85+55*i,295,YELLOW);
          sprintf(cstr,"0%d",i+1);
          Show_Str(51+55*i,255,BLACK,GREEN,cstr,24,1);//��ʾ��λ���       
        }
      }
    }
    
    if((PS_B_Recommend!=0 && Last_PSB_Recommend!=PS_B_Recommend) || Update_P == 1)//B����ĳ�λ�Ƿ����µĳ�λ���Ƽ���ȥ
    {
      for(i=0;i<PS_Total;i++)//Һ������ʾ��λ�Ƽ���Ϣ
      { 
        if((PS_B_Recommend&(1<<i))==1)
        {
          DrawSolidRectangle(210+55*i,240,245+55*i,295,YELLOW);
          sprintf(cstr,"0%d",i+1);
          Show_Str(211+55*i,255,BLACK,GREEN,cstr,24,1); //��ʾ��λ���         
        }    
      }   
    }
  }
  
  Update_P = 0;
  /*��¼Ŀǰ��λ�Ƽ����*/
  Last_PSA_Recommend=PS_A_Recommend;
  Last_PSB_Recommend=PS_B_Recommend;
}

/*******************************************************************************************************
 * ����: ButtonProj,ģ�鴦��ɨ�裩����
 * ����: void
 * ����: void
 *******************************************************************************************************/
void ButtonProj(void)
{
  /*�����ڵ��ĸ��������ģ����*/
	ButtonDetect(&s_tBtnKey1);										
	ButtonDetect(&s_tBtnKey2);										
	ButtonDetect(&s_tBtnKey3);										
	ButtonDetect(&s_tBtnKey4);										
	ParkingDetect(&s_tBtnKey5);										
	ParkingDetect(&s_tBtnKey6);										
	UrgentDetect(&s_tBtnKey7);
}

/*************************************************************************
*                             �����ʱ���ص���������
*************************************************************************/
static void In_ProcessSoftTimer_callback(void* parameter)
{ 
  ButtonProj();//��ģ��ɨ�裬��ʱ�ж�����Ϊ1ms
}

/*******************************************************************************************************
 * ����: button_thread_entry,��ȡ��ģ���¼������д���
 * ����: void
 * ����: void
 *******************************************************************************************************/
void button_thread_entry(void *parameter)//�û���Ϣ������ں���
{
  static char Smoke[5]={"A0000"};//������������λ�����ݴ洢
	rt_err_t uwRet = RT_EOK;
	uint8_t r_queue;//���ڽ���msg_mq��Ϣ������Ϣ
	
	button_mq = rt_mq_create("button_mq",							//��Ϣ��������
							1,  									//��Ϣ����󳤶�, bytes
							10,										//��Ϣ���е��������(����)
							RT_IPC_FLAG_FIFO);						//����ģʽ FIFO
						
	if(button_mq != RT_NULL)rt_kprintf("button_mq create success\n\n");
	ButtonInit();//����Ӳ���ӿڳ�ʼ��
	while(1)
	{  //��ȡ������Ϣ
		 uwRet = rt_mq_recv(button_mq,
							&r_queue,
							sizeof(r_queue),
							RT_WAITING_FOREVER);
		 if(RT_EOK == uwRet )
		 {
			 switch(r_queue)//���ݽ��յ�����Ϣ���ݷֱ���д���
			 {         
				 case KEY1_DOWN:if(sign_ps==0)OV7725_entr_vsync=0;break;//�г�Ҫ��ͣ����
				 case KEY1_UP:rt_event_send(Inspection_event, EVENT_FLAG0);if(Key2In)TIM_SetCompare4(TIM3,1400);break;//������ʻ��ͣ���������뿪���

				 case KEY2_DOWN:break;
				 case KEY2_UP:TIM_SetCompare4(TIM3,1400);break;//����ȫʻ��ͣ����

				 case KEY3_DOWN:OV7725_exit_vsync = 0;break;//�г�Ҫʻ��ͣ����
				 case KEY3_UP:rt_event_send(Inspection_event, EVENT_FLAG1);if(Key4In)TIM_SetCompare3(TIM3,1600);break;//������ʻ��ͣ����
         
				 case KEY4_DOWN:break;
				 case KEY4_UP:TIM_SetCompare3(TIM3,1600);break;//����ȫʻ��ͣ����
         
				 case 	SmokeA_DOWN:     
						Smoke[1]='1';//������������Ϣ����������Ũ�� 
						macESP8266_Usart("%s\r\n",Smoke);//����λ��������������Ϣ          
						DrawSolidCircular(52,420,20,RED);//Һ�����ϵ��������Ƶ���
							break;
				 case 	SmokeA_UP:
                        Smoke[1]='0';//������������Ϣ��������û��Ũ�� 
                        macESP8266_Usart("%s\r\n",Smoke);//����λ��������������Ϣ          
                        DrawSolidCircular(52,420,20,BROWN);//Һ�����ϵ���������Ϩ��
                        break;

				 default:rt_kprintf("No button Message!\n\n");break;
			 }			
		 }
		 else
		 {
			 rt_kprintf("���ݽ��մ��󣬴������:0x%lx\n\n",uwRet);
		 }
	}	
}
int button_process_init(void)
{
    rt_thread_t tid;
  
	In_ProcessSoftTimer =	rt_timer_create("In_ProcessSoftTimer", //�����ʱ��������
							In_ProcessSoftTimer_callback,//�����ʱ���Ļص�����
							0,			//��ʱ����ʱ��������ڲ���
							1,   //�����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��)
							RT_TIMER_FLAG_PERIODIC );
							//�����ʱ��ģʽ ����ģʽ

    tid = rt_thread_create("button_process",
                           button_thread_entry, RT_NULL,
                           BUTTON_THREAD_STACK_SIZE, BUTTON_THREAD_PRIORITY, 10);

    if (tid != NULL)rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(button_process_init);




