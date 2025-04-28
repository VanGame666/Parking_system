#include "config.h"
#include "USart.h"

#if USART4_EN == 1
	uint8   g_USART4_RxBuf[USART4_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART4_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

#if USART2_EN == 1
	uint8   g_USART2_RxBuf[USART2_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART2_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

#if USART3_EN == 1
	uint8   g_USART3_RxBuf[USART3_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART3_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

void UsartInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
#if USART4_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART4, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART4_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART4, &USART_InitStructure);//��ʼ������1
 
	//����1�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART4, USART_IT_IDLE, ENABLE);//��Ӵ��ڿ����ж�ʹ�ܣ��벻Ҫʹ��USART_IT_RXNE|USART_IT_IDLE����ס�ֿ�д�������
  
	USART_Cmd(USART4, ENABLE);//ʹ�ܴ���1
	
  USART_ClearFlag(USART4, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif


#if USART2_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART2_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);//��ʼ������2
#if 0	  //����FinSH��rt_hw_console_getcharʹ�ò�ѯ��ʽʵ�֣��ʴ���1�жϳ�ʼ����ע�͵�
	//����2�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��Ӵ��ڿ����ж�ʹ�ܣ��벻Ҫʹ��USART_IT_RXNE|USART_IT_IDLE����ס�ֿ�д�������
#endif
	USART_Cmd(USART2, ENABLE);//ʹ�ܴ���2
	
  USART_ClearFlag(USART2, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif

#if USART3_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART3_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);//��ʼ������3
	
	//����3�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3
	
  USART_ClearFlag(USART3, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif

}


//Ϊ�ܹ�����printf������ѡ���Ĵ��ڴ�ӡ������ض���fputc������������ʹ�ô���2
int fputc(int ch,FILE *f)
{
	
	USART_SendData(USART2, (uint8)ch);
	
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
	
	return (ch);

}

void UsartSendByte(USART_TypeDef* USARTx,uint8 ch)
{
	
	USART_SendData(USARTx, (uint8)ch);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)== RESET);
}


void UsartSendString(USART_TypeDef* USARTx,uint8 *str)
{
	uint32 pos = 0;
	while(*(str+pos)!='\0')
	{
		UsartSendByte(USARTx,*(str+pos));
		pos ++;	
	}
}

/*
 * ������:itoa
 * ����  :����������ת�����ַ���
 * ����  :-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  :��
 * ����  :��
 * ����  :��USART2_printf()����
 */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */



/*
 * ������:USART_printf
 * ����  :��ʽ�������������C���е�printf��������û���õ�C��
 * ����  :-USARTx ����ͨ����
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  :��
 * ����  :�� 
 * ����  :�ⲿ����
 *         ����Ӧ��USART3_printf( USART3, "\r\n this is a demo \r\n" );
 *            		 USART3_printf( USART3, "\r\n %d \r\n", i );
 *            		 USART3_printf( USART3, "\r\n %s \r\n", j );
 */
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];

	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//ʮ����
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;
				
				default:
				Data++;
				
				break;
				
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
		
	}
}



#if 0 //����FinSH��rt_hw_console_getcharʹ�ò�ѯ��ʽʵ�֣��ʴ���1�жϻص���ע�͵�
//USART1_IRQHandler,����1�жϻص�����
void USART4_IRQHandler(void)
{
	#if USART4_EN == 1 
		//�û�����
	 uint8 RecCh;
	#endif
	if( USART_GetFlagStatus(USART4,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
		#if USART2_EN == 1 
					 //�û�����
		RecCh = (uint8)USART_ReceiveData(USART4);
		g_USART4_RxBuf[g_USART4_RecPos++] = RecCh;
		
		#endif
		USART_ClearFlag(USART4, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART4,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART4, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART4,USART_FLAG_IDLE)==SET ) 				// �����������
	{
		#if USART4_EN == 1 
					 //�û�����
		
		g_USART4_RxBuf[g_USART4_RecPos] = '\0';
		rt_sem_release(usart4_recv_sem);//�ͷ�һ���ź�������ʾ�����ѽ���;������ֵ�ź��� �����ͽ��յ�������֡��־����ǰ̨�̲߳�ѯ
		
		#endif
		USART_ReceiveData(USART4);//ʹ�ø������������жϱ�־λ���벻Ҫʹ��USART_ClearITPendingBit(USART2, USART_IT_IDLE);������޷��ﵽЧ��
	}
}
#endif



#if 0
//USART2_IRQHandler,����2�жϻص�����
void USART2_IRQHandler(void)
{
	#if USART2_EN == 1 
		//�û�����
	 uint8 RecCh;
	#endif
	if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
		#if USART2_EN == 1 
					 //�û�����
		RecCh = (uint8)USART_ReceiveData(USART2);
		g_USART2_RxBuf[g_USART2_RecPos++] = RecCh;
		
		#endif
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_IDLE)==SET ) 				// ���ڿ����ж�
	{
		#if USART2_EN == 1 
					 //�û�����
		
		g_USART2_RxBuf[g_USART2_RecPos] = '\0';
		
		#endif
		USART_ReceiveData(USART2);//ʹ�ø������������жϱ�־λ���벻Ҫʹ��USART_ClearITPendingBit(USART2, USART_IT_IDLE);������޷��ﵽЧ��
	}
}
#endif


#if 0
//USART3_IRQHandler,����3�жϻص�����
void USART3_IRQHandler(void)
{
	#if USART3_EN == 1 
		//�û�����
	uint8 RecCh;
	#endif

	if( USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
		#if USART3_EN == 1 
			//�û�����
		RecCh = (uint8)USART_ReceiveData(USART3);
		g_USART3_RxBuf[g_USART3_RecPos++] = RecCh;

		#endif
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
  
	if( USART_GetFlagStatus(USART3,USART_FLAG_IDLE)==SET ) 				// ���ڿ����ж�
	{
		#if USART3_EN == 1 
					 //�û�����
		
		g_USART3_RxBuf[g_USART3_RecPos] = '\0';
    rt_sem_release(usart3_recv_sem);//�ͷ�һ���ź�������ʾ�����ѽ���;������ֵ�ź��� �����ͽ��յ�������֡��־����ǰ̨�̲߳�ѯ
		
		#endif
		USART_ReceiveData(USART3);//ʹ�ø������������жϱ�־λ���벻Ҫʹ��USART_ClearITPendingBit(USART3, USART_IT_IDLE);������޷��ﵽЧ��
	}
}
#endif

void rt_hw_console_output(const char *str)  //ʵ�ָú���������ʹ��rt_kprintf
{

	 /* �����ٽ�� */
  rt_enter_critical();
	
	while(*str!='\0')
	{
		 /* ���� */
    if (*str == '\n')//RT-Thread ϵͳ�����еĴ�ӡ���� \n ��β�������� \r\n���������ַ����ʱ����Ҫ����� \n ֮ǰ��� \r����ɻس��뻻�У�����ϵͳ��ӡ��������Ϣ��ֻ�л���
    {
       USART_SendData(USART2, '\r');
	
	     while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
    }

		USART_SendData(USART2, *(str++));
	
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
		
	}
	 /* �˳��ٽ�� */
  rt_exit_critical();  //ע��:ʹ�ý����ٽ�����rt_enter_critical(); һ��Ҫʹ���˳��ٽ����� rt_exit_critical();�����������ס���޷����е���
	
}

char rt_hw_console_getchar(void)//ʹ��Finsh���������:1.ʵ�ָú�����rt_hw_console_output������2.rtconfig.h�п���RT_USING_FINSH��;3.���Finsh���(cmd.c��msh.c��shell.c)��
{  //��ѯ��ʽʵ�֣��ǵý�Usart1��ʼ���е��жϽ���������ش���ע�͵�
	   int ch = -1;
	   /*�ȴ�����1��������*/
     if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
		 {
         ch = (int)USART_ReceiveData(USART2);
			   USART_ClearFlag(USART2, USART_FLAG_RXNE);
		 }
		 else
		 {
			 
         if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
         {
             USART_ClearFlag(USART2, USART_FLAG_ORE);
         }
         rt_thread_mdelay(10);
			 
		 }

     return ch;
}
