
#include "OV7725sccb_entrance.h"

#define OV7725_DEV_ADR  ADDR_OV7725 			 /*设备地址定义*/

/********************************************************************
 * 函数名:SCCB_Configuration
 * 描述  :SCCB管脚配置
 * 输入  :无
 * 输出  :无
 * 注意  :无        
 ********************************************************************/
void OV7725_SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
	
  /* SCL(PC6)、SDA(PC7)管脚配置 */
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
 * 函数名:SCCB_delay
 * 描述  :延迟时间
 * 输入  :无
 * 输出  :无
 * 注意  :内部调用        
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
 * 函数名:SCCB_Start
 * 描述  :SCCB起始信号
 * 输入  :无
 * 输出  :无
 * 注意  :内部调用        
 ********************************************************************/
static int OV7725_SCCB_Start(void)
{
  
  OV7725_SCCB_SDA_IN;
	OV7725_SCL_H;
	OV7725_SCCB_delay();
	if(!OV7725_SDA_read)
	return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	if(OV7725_SDA_read) 
	return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	OV7725_SDA_L;
	OV7725_SCCB_delay();
	return ENABLE;
}



/********************************************************************
 * 函数名:SCCB_Stop
 * 描述  :SCCB停止信号
 * 输入  :无
 * 输出  :无
 * 注意  :内部调用        
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
 * 函数名:SCCB_Ack
 * 描述  :SCCB应答方式
 * 输入  :无
 * 输出  :无
 * 注意  :内部调用        
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
 * 函数名:SCCB_NoAck
 * 描述  :SCCB 无应答方式
 * 输入  :无
 * 输出  :无
 * 注意  :内部调用        
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
 * 函数名:SCCB_WaitAck
 * 描述  :SCCB 等待应答
 * 输入  :无
 * 输出  :返回为:=1有ACK,=0无ACK
 * 注意  :内部调用        
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
 * 函数名:SCCB_SendByte
 * 描述  :数据从高位到低位
 * 输入  :SendByte: 发送的数据
 * 输出  :无
 * 注意  :内部调用        
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
 * 函数名:SCCB_ReceiveByte
 * 描述  :数据从高位到低位
 * 输入  :无
 * 输出  :SCCB总线返回的数据
 * 注意  :内部调用        
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
 * 函数名:SCCB_WriteByte
 * 描述  :写一字节数据
 * 输入  :- WriteAddress: 待写入地址 	- SendByte: 待写入数据	- DeviceAddress: 器件类型
 * 输出  :返回为:=1成功写入,=0失败
 * 注意  :无        
 *****************************************************************************************/           
int OV7725_SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{		
    if(!OV7725_SCCB_Start())
	{
	    return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR );                    /* 器件地址 */
    if( !OV7725_SCCB_WaitAck() )
	{
		OV7725_SCCB_Stop(); 
		return DISABLE;
	}
    OV7725_SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* 设置低起始地址 */      
    OV7725_SCCB_WaitAck();	
    OV7725_SCCB_SendByte(SendByte);
    OV7725_SCCB_WaitAck();   
    OV7725_SCCB_Stop(); 
    return ENABLE;
}

/******************************************************************************************************************
 * 函数名:SCCB_ReadByte
 * 描述  :读取一串数据
 * 输入  :- pBuffer: 存放读出数据 	- length: 待读出长度	- ReadAddress: 待读出地址		 - DeviceAddress: 器件类型
 * 输出  :返回为:=1成功读入,=0失败
 * 注意  :无        
 **********************************************************************************************************************/           
int OV7725_SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!OV7725_SCCB_Start())
	{
	    return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR );         /* 器件地址 */
    if( !OV7725_SCCB_WaitAck() )
	{
		OV7725_SCCB_Stop(); 
		return DISABLE;
	}
    OV7725_SCCB_SendByte( ReadAddress );     /* 设置低起始地址 */      
    OV7725_SCCB_WaitAck();	
    OV7725_SCCB_Stop(); 
	
    if(!OV7725_SCCB_Start())
	{
		return DISABLE;
	}
    OV7725_SCCB_SendByte( OV7725_DEV_ADR + 1 );     /* 器件地址 */ 
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

