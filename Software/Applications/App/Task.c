#include "config.h"
#include "Task.h"

extern uint8_t Update_P;//����Ƿ���Ҫ������ʾ��λ��Ϣ:0Ϊ����Ҫ��1Ϊ��Ҫ

/*************************************************************************
*                            ��������
*************************************************************************/
static uint16_t exit_num=0,entr_num=0;//����������������Ŀ
uint16_t  Detained_vehicle=0;//��������������Ŀ

static rt_timer_t Inspection_ProcessSoftTimer = RT_NULL;//�����ʱ�����ƿ�ָ��
static rt_thread_t message_thread;//�����߳̿��ƿ�ָ��

/*************************************************************************
*                            ��������
*************************************************************************/
static void      IWDG_thread_entry     (void *parameter);//���Ź��߳���ں���
static void      Event_thread_entry    (void *parameter);//�л��ɳ�λ��Ϣ��ʾ���߳���ں���
static void      exit_thread_entry     (void *parameter);// �г�Ҫʻ�����߳���ں���
static void      entr_thread_entry     (void *parameter);// �г�Ҫʻ����߳���ں���
static void      WIFI_thread_entry     (void *parameter);//����WIFI�����ط��������߳���ں���
static void      Vehicle_count         (void);// ����ͣ���������ּ����ĺ���

/*************************************************************************
*                            �̴߳���
*************************************************************************/
TaskStruct TaskThread[]={
{"IWDG_thread", IWDG_thread_entry, RT_NULL, 256, 2, 10},//���Ź��߳�
{"Event_thread", Event_thread_entry, RT_NULL, 512, 5, 20},//��ʾ����ʾ��λ��Ϣ��ʾ�߳�
{"exit_thread", exit_thread_entry, RT_NULL, 3072, 4, 80},//�����г�Ҫʻ�����߳�
{"entr_thread", entr_thread_entry, RT_NULL, 3072, 4, 80},//����г�Ҫʻ����߳�
{"WIFI_thread", WIFI_thread_entry, RT_NULL, 768, 3, 20},//����WIFI�����ط�����

{"", RT_NULL, RT_NULL, RT_NULL, RT_NULL, RT_NULL},
};

/*************************************************************************
*                             �����ʱ���ص���������
*************************************************************************/
static void Inspection_ProcessSoftTimer_callback(void* parameter)
{ 
  Parking_management();//��λ����Լ���Ϣ���� 
}

/*************************************************************************
*                            ���Ź��߳���ں���
*************************************************************************/
static void IWDG_thread_entry(void *parameter)
{
  while(1)
  {
    rt_thread_mdelay(5000);//ι����ʱ����
    IWDG_Feed();//ι��
    rt_kprintf("ι��\n");
  }
}

/*************************************************************************
*                     �л��ɳ�λ��Ϣ��ʾ���߳���ں���
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
      Parking_information();//��λ��Ϣ��ʾ
      Update_P=1;//Ϊ��ˢ�³�λ��Ϣ
      Parking_management();//��ʾ��λ��Ϣ
       /* ������ʱ�� */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
        rt_timer_start(Inspection_ProcessSoftTimer);
    }
  }
}

/*************************************************************************
*                       �г�Ҫʻ�����߳���ں���
*************************************************************************/
static void exit_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while(1)
	{    
    uwRet =rt_sem_take(exit_recv_sem, RT_WAITING_FOREVER);//��ȡ�г�ʻ�����ź���
    if(RT_EOK == uwRet )
    {
      /* ��ͣ��ʱ�� */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
        rt_timer_stop(Inspection_ProcessSoftTimer);
      while(!Number_recognition_LP(exit_LP) && Key3In == 0 );//����ʶ����ʾ
      Exit_LP_communication(); //���ƺŷ�����λ��
      rt_thread_mdelay(80);
      TIM_SetCompare3(TIM3,600);//�������
      rt_thread_mdelay(20);
      exit_num++;//�ۼ�ʻ������
      exit_num%=100;//��ϵͳ��ֵ��ʾ���Ϊ99     
      Vehicle_count();
    }
	}
}

/*************************************************************************
*                       �г�Ҫʻ����߳���ں���
*************************************************************************/
static void entr_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	while(1)
	{
		uwRet =rt_sem_take(entr_recv_sem, RT_WAITING_FOREVER);//��ȡ�г�ʻ���ź���
    if(RT_EOK == uwRet )
    {
      /* ��ͣ��ʱ�� */
      if (Inspection_ProcessSoftTimer != RT_NULL) 
      rt_timer_stop(Inspection_ProcessSoftTimer);
      while(!Number_recognition_LP(entrance_LP) && Key1In == 0 );//����ʶ����ʾ 
      Show_Str(76,320,BLACK,WHITE,"��ǰ��",24,1);
      Show_Str(196,320,BLACK,WHITE,"��λ",24,1);
      Entrance_LP_communication();//���ƺŷ�����λ��
      Parking_recommendation();//Һ������ʾ�Ƽ���λ
      rt_thread_mdelay(80);
      TIM_SetCompare4(TIM3,600);//�������
      rt_thread_mdelay(20);
      entr_num++;//�ۼ�ʻ�복����Ŀ
      entr_num%=100;//��ϵͳ��ֵ��ʾ���Ϊ99
      Vehicle_count();
    }
	}
}

/*************************************************************************
*                      ����WIFI�����ط��������߳���ں���
*************************************************************************/
static void WIFI_thread_entry(void *parameter)
{
 	macESP8266_CH_ENABLE();//����ESP8266
	
	ESP8266_AT_Test();//ATָ�����
	
	ESP8266_Net_Mode_Choose ( STA );//ģʽѡ��

	while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	//����WIFI
	
	ESP8266_Enable_MultipleId ( DISABLE );//�Ƿ������
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );//���ӷ�����
	
	while ( ! ESP8266_UnvarnishSend () );//����͸��
  
  Interface_init();//Һ����ϵͳ������ʾ
  Show_Str(112,58,BLACK,WHITE,"��",16,0);
  Vehicle_count();
      /* ������ʱ�� */
  if (Inspection_ProcessSoftTimer != RT_NULL) 
      rt_timer_start(Inspection_ProcessSoftTimer);
      /* ������ʱ�� */
  if (In_ProcessSoftTimer != RT_NULL) 
      rt_timer_start(In_ProcessSoftTimer);
}

/*************************************************************************
*                  �̴߳����������߳�ͨѶ��ʽ�Ĵ�������
*************************************************************************/
void TaskInit(void)
{
	uint8_t TaskThreadTndex = 0;
  
	Inspection_event = rt_event_create("Inspection_event", RT_IPC_FLAG_FIFO);
  
	exit_recv_sem = rt_sem_create(	"exit_recv_sem",  //�ź�������
									0,                  //�ź�����ʼֵ
									RT_IPC_FLAG_FIFO);    //�ź���ģʽ FIFO(0x00)
	
	entr_recv_sem = rt_sem_create(	"entr_recv_sem",  //�ź�������
									0,                  //�ź�����ʼֵ
									RT_IPC_FLAG_FIFO);   //�ź���ģʽ FIFO(0x00)
	                          
	Inspection_ProcessSoftTimer = rt_timer_create(	"Inspection_ProcessSoftTimer", /* �����ʱ�������� */
													Inspection_ProcessSoftTimer_callback,/* �����ʱ���Ļص����� */
													0,			/* ��ʱ����ʱ��������ڲ��� */
													500,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��)��λ���� */
													RT_TIMER_FLAG_PERIODIC );
	while(1)
	{
		if(strcmp(TaskThread[TaskThreadTndex].name,"") != 0 )
		{
			message_thread = rt_thread_create(	TaskThread[TaskThreadTndex].name,   /*�߳�����*/
												TaskThread[TaskThreadTndex].entry, /*�߳���ں�����*/
												TaskThread[TaskThreadTndex].parameter,      /*�߳���ں�������*/
												TaskThread[TaskThreadTndex].stack_size,      /*�߳�ջ��С*/
												TaskThread[TaskThreadTndex].priority,        /*�̵߳����ȼ�*/
												TaskThread[TaskThreadTndex].tick);      /*�߳�ʱ��Ƭ*/
			if(message_thread != RT_NULL){rt_thread_startup(message_thread);} 
			TaskThreadTndex ++; 
		}else{break;}
	}
}

/*************************************************************************
*                       ����ͣ���������ּ����ĺ���
*************************************************************************/
static void Vehicle_count(void)
{
  char counternum[2]={0};//�洢���ֵ�ÿλ��ֵ
  
  counternum[0]=entr_num/10+'0';//ʮλ����ֵ
  counternum[1]=entr_num%10+'0';//��λ����ֵ
  Show_Str(180,80,RED,WHITE,counternum,24,0);//����ʻ�복����ֵ��ʾ

  counternum[0]=exit_num/10+'0';//ʮλ����ֵ
  counternum[1]=exit_num%10+'0';//��λ����ֵ
  Show_Str(180,107,RED,WHITE,counternum,24,0); //����ʻ��������ֵ��ʾ

  Detained_vehicle=entr_num-exit_num;//��������������Ŀ
  counternum[0]=((Detained_vehicle+Stranded_Init)%100)/10+'0';////ʮλ����ֵ
  counternum[1]=(Detained_vehicle+Stranded_Init)%10+'0';//��λ����ֵ
  Show_Str(180,134,RED,WHITE,counternum,24,0);//��������������ֵ
  
  counternum[0]=((Total_P-Detained_vehicle)%100)/10+'0';//����ʣ�೵λ����ʮλ����ֵ
  counternum[1]=(Total_P-Detained_vehicle)%10+'0';//����ʣ�೵λ���ĸ�λ����ֵ
  Show_Str(180,161,RED,WHITE,counternum,24,0);//����ʣ�೵λ��ֵ
}
