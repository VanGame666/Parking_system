#ifndef __USART_H__
#define __USART_H__



#define  USART1_EN 1
#define  USART2_EN 1
#define  USART3_EN 0

 

#if USART2_EN == 1
	#define USART2_BAUD         115200
	#define USART2_RX_BUF_SIZE  256
#endif 

#if USART3_EN == 1
	#define USART3_BAUD         9600
	#define USART3_RX_BUF_SIZE  256
#endif 


#if USART2_EN == 1
	extern uint8   	g_USART2_RxBuf[USART2_RX_BUF_SIZE];//���ڽ��ջ�����
	extern uint16 	g_USART2_RecPos;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

#if USART3_EN == 1
	extern uint8  	g_USART3_RxBuf[USART3_RX_BUF_SIZE];//���ڽ��ջ�����
	extern uint16 	g_USART3_RecPos;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

void UsartInit(void);
void UsartSendString(USART_TypeDef* USARTx,uint8 *str);//�����ַ���
void UsartSendByte(USART_TypeDef* USARTx,uint8 ch);
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... );
#endif



