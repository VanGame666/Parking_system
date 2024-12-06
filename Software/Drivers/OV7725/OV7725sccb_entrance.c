
#include "OV7725sccb_entrance.h"

#define OV7725_DEV_ADR  ADDR_OV7725 			 /*�豸��ַ����*/

/********************************************************************
 * ������:SCCB_Configuration
 * ����  :SCCB�ܽ�����
 * ����  :��
 * ���  :��
 * ע��  :��        
 ********************************************************************/
void OV7725_SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	
  /* SCL(PC6)��SDA(PC7)�ܽ����� */
	OV7725_SIO_C_SCK_APBxClock_FUN ( OV7725_SIO_C_GPIO_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin =  OV7725_SIO_C_GPIO_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(OV7725_SIO_C_GPIO_PORT, &GPIO_InitStructure);
	
	OV7725_SIO_D_SCK_APBxClock_FUN ( OV7725_SIO_D_GPIO_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin =  OV7725_SIO_D_GPIO_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(OV7725_SIO_D_GPIO_PORT, &GPIO_InitStructure);
	
}

/********************************************************************
 * ������:SCCB_delay
 * ����  :�ӳ�ʱ��
 * ����  :��
 * ���  :��
 * ע��  :�ڲ�����        
 ********************************************************************/
static void OV7725_SCCB_delay(void)
{	
   uint16_t i = 400; 
   while(i) 
   { 
     i--; 
   } 
}

/********************************************************************
 * ������:SCCB_Start
 * ����  :SCCB��ʼ�ź�
 * ����  :��
 * ���  :��
 * ע��  :�ڲ�����        
 ********************************************************************/
static int OV7725_SCCB_Start(void)
{
  
  OV7725_SCCB_SDA_IN;
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	if(!OV7725_SDA_read)
	return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	if(OV7725_SDA_read) 
	return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	return ENABLE;
}



/********************************************************************
 * ������:SCCB_Stop
 * ����  :SCCBֹͣ�ź�
 * ����  :��
 * ���  :��
 * ע��  :�ڲ�����        
 ********************************************************************/
static void OV7725_SCCB_Stop(void)
{
	OV7725_SCL_L;
	OV7725_SCCB_delay();
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	OV7725_SDA_H;
	OV7725_SCCB_delay();
}



/********************************************************************
 * ������:SCCB_Ack
 * ����  :SCCBӦ��ʽ
 * ����  :��
 * ���  :��
 * ע��  :�ڲ�����        
 ********************************************************************/
static void OV7725_SCCB_Ack(void)
{	
	OV7725_SCL_L;
	OV7725_SCCB_delay();
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	OV7725_SCL_L;
	OV7725_SCCB_delay();
}



/********************************************************************
 * ������:SCCB_NoAck
 * ����  :SCCB ��Ӧ��ʽ
 * ����  :��
 * ���  :��
 * ע��  :�ڲ�����        
 ********************************************************************/
static void OV7725_SCCB_NoAck(void)
{	
	OV7725_SCL_L;
	OV7725_SCCB_delay();
	OV7725_SDA_H;
	OV7725_SCCB_delay();
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	OV7725_SCL_L;
	OV7725_SCCB_delay();
}

/********************************************************************
 * ������:SCCB_WaitAck
 * ����  :SCCB �ȴ�Ӧ��
 * ����  :��
 * ���  :����Ϊ:=1��ACK,=0��ACK
 * ע��  :�ڲ�����        
 ********************************************************************/
static int OV7725_SCCB_WaitAck(void) 	
{
	OV7725_SCL_L;
	OV7725_SCCB_delay();
	OV7725_SDA_H;			
	OV7725_SCCB_delay();
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	if(OV7725_SDA_read)
	{
      OV7725_SCL_L;
      return DISABLE;
	}
	OV7725_SCL_L;
	return ENABLE;
}



 /*******************************************************************
 * ������:SCCB_SendByte
 * ����  :���ݴӸ�λ����λ
 * ����  :SendByte: ���͵�����
 * ���  :��
 * ע��  :�ڲ�����        
 *********************************************************************/
static void OV7725_SCCB_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
        OV7725_SCL_L;
        OV7725_SCCB_delay();
      if(SendByte&0x80)
        OV7725_SDA_H;  
      else 
        OV7725_SDA_L;   
        SendByte<<=1;
        OV7725_SCCB_delay();
        OV7725_SCL_H;
        OV7725_SCCB_delay();
    }
    OV7725_SCL_L;
}




 /******************************************************************
 * ������:SCCB_ReceiveByte
 * ����  :���ݴӸ�λ����λ
 * ����  :��
 * ���  :SCCB���߷��ص�����
 * ע��  :�ڲ�����        
 *******************************************************************/
static int OV7725_SCCB_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    OV7725_SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      OV7725_SCL_L;
      OV7725_SCCB_delay();
      OV7725_SCL_H;
      OV7725_SCCB_delay();	
      if(OV7725_SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    OV7725_SCL_L;
    return ReceiveByte;
}





 /*****************************************************************************************
 * ������:SCCB_WriteByte
 * ����  :дһ�ֽ�����
 * ����  :- WriteAddress: ��д���ַ 	- SendByte: ��д������	- DeviceAddress: ��������
 * ���  :����Ϊ:=1�ɹ�д��,=0ʧ��
 * ע��  :��        
 *****************************************************************************************/           
int OV7725_SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
    if(!OV7725_SCCB_Start())
	{
	    return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR );                    /* ������ַ */
    if( !OV7725_SCCB_WaitAck() )
	{
		OV7725_SCCB_Stop(); 
		return DISABLE;
	}
    OV7725_SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    OV7725_SCCB_WaitAck();	
    OV7725_SCCB_SendByte(SendByte);
    OV7725_SCCB_WaitAck();   
    OV7725_SCCB_Stop(); 
    return ENABLE;
}

/******************************************************************************************************************
 * ������:SCCB_ReadByte
 * ����  :��ȡһ������
 * ����  :- pBuffer: ��Ŷ������� 	- length: ����������	- ReadAddress: ��������ַ		 - DeviceAddress: ��������
 * ���  :����Ϊ:=1�ɹ�����,=0ʧ��
 * ע��  :��        
 **********************************************************************************************************************/           
int OV7725_SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!OV7725_SCCB_Start())
	{
	    return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR );         /* ������ַ */
    if( !OV7725_SCCB_WaitAck() )
	{
		OV7725_SCCB_Stop(); 
		return DISABLE;
	}
    OV7725_SCCB_SendByte( ReadAddress );     /* ���õ���ʼ��ַ */      
    OV7725_SCCB_WaitAck();	
    OV7725_SCCB_Stop(); 
	
    if(!OV7725_SCCB_Start())
	{
		return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR + 1 );     /* ������ַ */ 
    if(!OV7725_SCCB_WaitAck())
	{
		OV7725_SCCB_Stop(); 
		return DISABLE;
	}
    while(length)
    {
      *pBuffer = OV7725_SCCB_ReceiveByte();
      if(length == 1)
	  {
		  OV7725_SCCB_NoAck();
	  }
      else
	  {
      OV7725_SCCB_Ack(); 
	  }
      pBuffer++;
      length--;
    }
    OV7725_SCCB_Stop();
    return ENABLE;
}

