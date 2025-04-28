#include "config.h"
#include "USart.h"

#if USART4_EN == 1
	uint8   g_USART4_RxBuf[USART4_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART4_RecPos = 0;//存放当前串口接收数据存放的位置
#endif 

#if USART2_EN == 1
	uint8   g_USART2_RxBuf[USART2_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART2_RecPos = 0;//存放当前串口接收数据存放的位置
#endif 

#if USART3_EN == 1
	uint8   g_USART3_RxBuf[USART3_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART3_RecPos = 0;//存放当前串口接收数据存放的位置
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
	USART_Init(USART4, &USART_InitStructure);//初始化串口1
 
	//串口1中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART4, USART_IT_IDLE, ENABLE);//添加串口空闲中断使能，请不要使用USART_IT_RXNE|USART_IT_IDLE，记住分开写两条语句
  
	USART_Cmd(USART4, ENABLE);//使能串口1
	
  USART_ClearFlag(USART4, USART_FLAG_TC  );//清发送完成标志位
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
	USART_Init(USART2, &USART_InitStructure);//初始化串口2
#if 0	  //由于FinSH中rt_hw_console_getchar使用查询方式实现，故串口1中断初始化需注释掉
	//串口2中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//添加串口空闲中断使能，请不要使用USART_IT_RXNE|USART_IT_IDLE，记住分开写两条语句
#endif
	USART_Cmd(USART2, ENABLE);//使能串口2
	
  USART_ClearFlag(USART2, USART_FLAG_TC  );//清发送完成标志位
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
	USART_Init(USART3, &USART_InitStructure);//初始化串口3
	
	//串口3中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART3, ENABLE);//使能串口3
	
  USART_ClearFlag(USART3, USART_FLAG_TC  );//清发送完成标志位
#endif

}


//为能够调用printf函数从选定的串口打印输出，重定义fputc函数，本例子使用串口2
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
 * 函数名:itoa
 * 描述  :将整形数据转换成字符串
 * 输入  :-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  :无
 * 返回  :无
 * 调用  :被USART2_printf()调用
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
 * 函数名:USART_printf
 * 描述  :格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  :-USARTx 串口通道，
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  :无
 * 返回  :无 
 * 调用  :外部调用
 *         典型应用USART3_printf( USART3, "\r\n this is a demo \r\n" );
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

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
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
				case 's':										  //字符串
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//十进制
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



#if 0 //由于FinSH中rt_hw_console_getchar使用查询方式实现，故串口1中断回调需注释掉
//USART1_IRQHandler,串口1中断回调函数
void USART4_IRQHandler(void)
{
	#if USART4_EN == 1 
		//用户代码
	 uint8 RecCh;
	#endif
	if( USART_GetFlagStatus(USART4,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
		#if USART2_EN == 1 
					 //用户代码
		RecCh = (uint8)USART_ReceiveData(USART4);
		g_USART4_RxBuf[g_USART4_RecPos++] = RecCh;
		
		#endif
		USART_ClearFlag(USART4, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART4,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART4, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART4,USART_FLAG_IDLE)==SET ) 				// 串口溢出错误
	{
		#if USART4_EN == 1 
					 //用户代码
		
		g_USART4_RxBuf[g_USART4_RecPos] = '\0';
		rt_sem_release(usart4_recv_sem);//释放一个信号量，表示数据已接收;给出二值信号量 ，发送接收到新数据帧标志，供前台线程查询
		
		#endif
		USART_ReceiveData(USART4);//使用该语句清除空闲中断标志位，请不要使用USART_ClearITPendingBit(USART2, USART_IT_IDLE);该语句无法达到效果
	}
}
#endif



#if 0
//USART2_IRQHandler,串口2中断回调函数
void USART2_IRQHandler(void)
{
	#if USART2_EN == 1 
		//用户代码
	 uint8 RecCh;
	#endif
	if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
		#if USART2_EN == 1 
					 //用户代码
		RecCh = (uint8)USART_ReceiveData(USART2);
		g_USART2_RxBuf[g_USART2_RecPos++] = RecCh;
		
		#endif
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_IDLE)==SET ) 				// 串口空闲中断
	{
		#if USART2_EN == 1 
					 //用户代码
		
		g_USART2_RxBuf[g_USART2_RecPos] = '\0';
		
		#endif
		USART_ReceiveData(USART2);//使用该语句清除空闲中断标志位，请不要使用USART_ClearITPendingBit(USART2, USART_IT_IDLE);该语句无法达到效果
	}
}
#endif


#if 0
//USART3_IRQHandler,串口3中断回调函数
void USART3_IRQHandler(void)
{
	#if USART3_EN == 1 
		//用户代码
	uint8 RecCh;
	#endif

	if( USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
		#if USART3_EN == 1 
			//用户代码
		RecCh = (uint8)USART_ReceiveData(USART3);
		g_USART3_RxBuf[g_USART3_RecPos++] = RecCh;

		#endif
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
  
	if( USART_GetFlagStatus(USART3,USART_FLAG_IDLE)==SET ) 				// 串口空闲中断
	{
		#if USART3_EN == 1 
					 //用户代码
		
		g_USART3_RxBuf[g_USART3_RecPos] = '\0';
    rt_sem_release(usart3_recv_sem);//释放一个信号量，表示数据已接收;给出二值信号量 ，发送接收到新数据帧标志，供前台线程查询
		
		#endif
		USART_ReceiveData(USART3);//使用该语句清除空闲中断标志位，请不要使用USART_ClearITPendingBit(USART3, USART_IT_IDLE);该语句无法达到效果
	}
}
#endif

void rt_hw_console_output(const char *str)  //实现该函数，才能使用rt_kprintf
{

	 /* 进入临界段 */
  rt_enter_critical();
	
	while(*str!='\0')
	{
		 /* 换行 */
    if (*str == '\n')//RT-Thread 系统中已有的打印均以 \n 结尾，而并非 \r\n，所以在字符输出时，需要在输出 \n 之前输出 \r，完成回车与换行，否则系统打印出来的信息将只有换行
    {
       USART_SendData(USART2, '\r');
	
	     while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
    }

		USART_SendData(USART2, *(str++));
	
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)== RESET);
		
	}
	 /* 退出临界段 */
  rt_exit_critical();  //注意:使用进入临界段语句rt_enter_critical(); 一定要使用退出临界段语句 rt_exit_critical();否则调度器锁住，无法进行调度
	
}

char rt_hw_console_getchar(void)//使用Finsh组件三步骤:1.实现该函数及rt_hw_console_output函数；2.rtconfig.h中开启RT_USING_FINSH宏;3.添加Finsh组件(cmd.c、msh.c、shell.c)，
{  //查询方式实现，记得将Usart1初始化中的中断接收配置相关代码注释掉
	   int ch = -1;
	   /*等待串口1输入数据*/
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
