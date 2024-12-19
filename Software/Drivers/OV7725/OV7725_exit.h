#ifndef __OV7725_exit_H
#define __OV7725_exit_H 
	   
#include "stm32f10x.h"



/* 寄存器宏定义 */
#define REG_GAIN      0x00
#define REG_BLUE      0x01
#define REG_RED       0x02
#define REG_GREEN     0x03
#define REG_BAVG      0x05
#define REG_GAVG      0x06
#define REG_RAVG      0x07
#define REG_AECH      0x08
#define REG_COM2      0x09
#define REG_PID       0x0A
#define REG_VER       0x0B
#define REG_COM3      0x0C
#define REG_COM4      0x0D
#define REG_COM5      0x0E
#define REG_COM6      0x0F
#define REG_AEC       0x10
#define REG_CLKRC     0x11
#define REG_COM7      0x12
#define REG_COM8      0x13
#define REG_COM9      0x14
#define REG_COM10     0x15
#define REG_REG16     0x16
#define REG_HSTART    0x17
#define REG_HSIZE     0x18
#define REG_VSTRT     0x19
#define REG_VSIZE     0x1A
#define REG_PSHFT     0x1B
#define REG_MIDH      0x1C
#define REG_MIDL      0x1D
#define REG_LAEC      0x1F
#define REG_COM11     0x20
#define REG_BDBase    0x22
#define REG_BDMStep   0x23
#define REG_AEW       0x24
#define REG_AEB       0x25
#define REG_VPT       0x26
#define REG_REG28     0x28
#define REG_HOutSize  0x29
#define REG_EXHCH     0x2A
#define REG_EXHCL     0x2B
#define REG_VOutSize  0x2C
#define REG_ADVFL     0x2D
#define REG_ADVFH     0x2E
#define REG_YAVE      0x2F
#define REG_LumHTh    0x30
#define REG_LumLTh    0x31
#define REG_HREF      0x32
#define REG_DM_LNL    0x33
#define REG_DM_LNH    0x34
#define REG_ADoff_B   0x35
#define REG_ADoff_R   0x36
#define REG_ADoff_Gb  0x37
#define REG_ADoff_Gr  0x38
#define REG_Off_B     0x39
#define REG_Off_R     0x3A
#define REG_Off_Gb    0x3B
#define REG_Off_Gr    0x3C
#define REG_COM12     0x3D
#define REG_COM13     0x3E
#define REG_COM14     0x3F
#define REG_COM16     0x41
#define REG_TGT_B     0x42
#define REG_TGT_R     0x43
#define REG_TGT_Gb    0x44
#define REG_TGT_Gr    0x45
#define REG_LC_CTR    0x46
#define REG_LC_XC     0x47
#define REG_LC_YC     0x48
#define REG_LC_COEF   0x49
#define REG_LC_RADI   0x4A
#define REG_LC_COEFB  0x4B 
#define REG_LC_COEFR  0x4C
#define REG_FixGain   0x4D
#define REG_AREF1     0x4F
#define REG_AREF6     0x54
#define REG_UFix      0x60
#define REG_VFix      0x61
#define REG_AWBb_blk  0x62
#define REG_AWB_Ctrl0 0x63
#define REG_DSP_Ctrl1 0x64
#define REG_DSP_Ctrl2 0x65
#define REG_DSP_Ctrl3 0x66
#define REG_DSP_Ctrl4 0x67
#define REG_AWB_bias  0x68
#define REG_AWBCtrl1  0x69
#define REG_AWBCtrl2  0x6A
#define REG_AWBCtrl3  0x6B
#define REG_AWBCtrl4  0x6C
#define REG_AWBCtrl5  0x6D
#define REG_AWBCtrl6  0x6E
#define REG_AWBCtrl7  0x6F
#define REG_AWBCtrl8  0x70
#define REG_AWBCtrl9  0x71
#define REG_AWBCtrl10 0x72
#define REG_AWBCtrl11 0x73
#define REG_AWBCtrl12 0x74
#define REG_AWBCtrl13 0x75
#define REG_AWBCtrl14 0x76
#define REG_AWBCtrl15 0x77
#define REG_AWBCtrl16 0x78
#define REG_AWBCtrl17 0x79
#define REG_AWBCtrl18 0x7A
#define REG_AWBCtrl19 0x7B
#define REG_AWBCtrl20 0x7C
#define REG_AWBCtrl21 0x7D 
#define REG_GAM1      0x7E
#define REG_GAM2      0x7F
#define REG_GAM3      0x80
#define REG_GAM4      0x81
#define REG_GAM5      0x82
#define REG_GAM6      0x83
#define REG_GAM7      0x84
#define REG_GAM8      0x85
#define REG_GAM9      0x86
#define REG_GAM10     0x87
#define REG_GAM11     0x88
#define REG_GAM12     0x89
#define REG_GAM13     0x8A
#define REG_GAM14     0x8B
#define REG_GAM15     0x8C
#define REG_SLOP      0x8D
#define REG_DNSTh     0x8E
#define REG_EDGE0     0x8F
#define REG_EDGE1     0x90
#define REG_DNSOff    0x91
#define REG_EDGE2     0x92
#define REG_EDGE3     0x93
#define REG_MTX1      0x94
#define REG_MTX2      0x95
#define REG_MTX3      0x96
#define REG_MTX4      0x97
#define REG_MTX5      0x98
#define REG_MTX6      0x99
#define REG_MTX_Ctrl  0x9A
#define REG_BRIGHT    0x9B
#define REG_CNST      0x9C
#define REG_UVADJ0    0x9E
#define REG_UVADJ1    0x9F
#define REG_SCAL0     0xA0
#define REG_SCAL1     0xA1
#define REG_SCAL2     0xA2
#define REG_SDE       0xA6
#define REG_USAT      0xA7
#define REG_VSAT      0xA8
#define REG_HUECOS    0xA9
#define REG_HUESIN    0xAA
#define REG_SIGN      0xAB
#define REG_DSPAuto   0xAC



/************************** OV7725_exit 连接引脚定义********************************/
// FIFO 输出使能，即模块中的OE
#define      OV7725_exit_OE_GPIO_CLK                       RCC_APB2Periph_GPIOC
#define      OV7725_exit_OE_GPIO_PORT                      GPIOC
#define      OV7725_exit_OE_GPIO_PIN                       GPIO_Pin_6

// FIFO 写复位
#define      OV7725_exit_WRST_GPIO_CLK                     RCC_APB2Periph_GPIOC
#define      OV7725_exit_WRST_GPIO_PORT                    GPIOC
#define      OV7725_exit_WRST_GPIO_PIN                     GPIO_Pin_7

// FIFO 读复位
#define      OV7725_exit_RRST_GPIO_CLK                     RCC_APB2Periph_GPIOC
#define      OV7725_exit_RRST_GPIO_PORT                    GPIOC
#define      OV7725_exit_RRST_GPIO_PIN                     GPIO_Pin_8

// FIFO 读时钟
#define      OV7725_exit_RCLK_GPIO_CLK                     RCC_APB2Periph_GPIOG
#define      OV7725_exit_RCLK_GPIO_PORT                    GPIOG
#define      OV7725_exit_RCLK_GPIO_PIN                     GPIO_Pin_14

// FIFO 写使能
#define      OV7725_exit_WE_GPIO_CLK                       RCC_APB2Periph_GPIOG
#define      OV7725_exit_WE_GPIO_PORT                      GPIOG
#define      OV7725_exit_WE_GPIO_PIN                       GPIO_Pin_13


// 8位数据口
#define      OV7725_exit_DATA_GPIO_CLK                     RCC_APB2Periph_GPIOG
#define      OV7725_exit_DATA_GPIO_PORT                    GPIOG
#define      OV7725_exit_DATA_0_GPIO_PIN                   GPIO_Pin_2
#define      OV7725_exit_DATA_1_GPIO_PIN                   GPIO_Pin_3
#define      OV7725_exit_DATA_2_GPIO_PIN                   GPIO_Pin_4
#define      OV7725_exit_DATA_3_GPIO_PIN                   GPIO_Pin_5
#define      OV7725_exit_DATA_4_GPIO_PIN                   GPIO_Pin_6
#define      OV7725_exit_DATA_5_GPIO_PIN                   GPIO_Pin_7
#define      OV7725_exit_DATA_6_GPIO_PIN                   GPIO_Pin_8
#define      OV7725_exit_DATA_7_GPIO_PIN                   GPIO_Pin_9

// OV7725_exit场中断
#define      OV7725_exit_VSYNC_GPIO_CLK                    RCC_APB2Periph_GPIOG
#define      OV7725_exit_VSYNC_GPIO_PORT                   GPIOG
#define      OV7725_exit_VSYNC_GPIO_PIN                    GPIO_Pin_15

#define      OV7725_exit_VSYNC_EXTI_SOURCE_PORT            GPIO_PortSourceGPIOG
#define      OV7725_exit_VSYNC_EXTI_SOURCE_PIN             GPIO_PinSource15
#define      OV7725_exit_VSYNC_EXTI_LINE                   EXTI_Line15
#define      OV7725_exit_VSYNC_EXTI_IRQ                    EXTI15_10_IRQn
#define      VSYNC_exit_EXTI_INT_FUNCTION           	   EXTI15_10_IRQHandler



#define OV7725_exit_FIFO_OE_H()     OV7725_exit_OE_GPIO_PORT->BSRR =OV7725_exit_OE_GPIO_PIN	  
#define OV7725_exit_FIFO_OE_L()     OV7725_exit_OE_GPIO_PORT->BRR  =OV7725_exit_OE_GPIO_PIN	  /*拉低使FIFO输出使能*/

#define OV7725_exit_FIFO_WRST_H()   OV7725_exit_WRST_GPIO_PORT->BSRR =OV7725_exit_WRST_GPIO_PIN	  /*拉高允许FIFO写(数据from摄像头)指针运动 */
#define OV7725_exit_FIFO_WRST_L()   OV7725_exit_WRST_GPIO_PORT->BRR  =OV7725_exit_WRST_GPIO_PIN	  /*拉低使FIFO写(数据from摄像头)指针复位*/

#define OV7725_exit_FIFO_RRST_H()   OV7725_exit_RRST_GPIO_PORT->BSRR =OV7725_exit_RRST_GPIO_PIN	  /*拉高允许FIFO读(数据从FIFO输出)指针运动 */
#define OV7725_exit_FIFO_RRST_L()   OV7725_exit_RRST_GPIO_PORT->BRR  =OV7725_exit_RRST_GPIO_PIN	  /*拉低使FIFO读(数据从FIFO输出)指针复位 */

#define OV7725_exit_FIFO_RCLK_H()   OV7725_exit_RCLK_GPIO_PORT->BSRR =OV7725_exit_RCLK_GPIO_PIN	  
#define OV7725_exit_FIFO_RCLK_L()   OV7725_exit_RCLK_GPIO_PORT->BRR  =OV7725_exit_RCLK_GPIO_PIN	  /*FIFO输出数据时钟*/

#define OV7725_exit_FIFO_WE_H()     OV7725_exit_WE_GPIO_PORT->BSRR =OV7725_exit_WE_GPIO_PIN	  /*拉高使FIFO写允许*/
#define OV7725_exit_FIFO_WE_L()     OV7725_exit_WE_GPIO_PORT->BRR  =OV7725_exit_WE_GPIO_PIN


 
#define OV7725_READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  OV7725_FIFO_RCLK_L();\
	                                  RGB565 = (OV7725_DATA_GPIO_PORT->IDR<<8) & 0xff00;\
	                                  OV7725_FIFO_RCLK_H();\
                                    OV7725_FIFO_RCLK_L();\
	                                  RGB565 |= (OV7725_DATA_GPIO_PORT->IDR) & 0x00ff;\
	                                  OV7725_FIFO_RCLK_H();\
                                    }while(0)                                    
                                    

#define OV7725_exit_FIFO_PREPARE                do{\
	                                  OV7725_exit_FIFO_RRST_L();\
	                                  OV7725_exit_FIFO_RCLK_L();\
	                                  OV7725_exit_FIFO_RCLK_H();\
	                                  OV7725_exit_FIFO_RRST_H();\
	                                  OV7725_exit_FIFO_RCLK_L();\
	                                  OV7725_exit_FIFO_RCLK_H();\
                                    }while(0)

#define OV7725_exit_ID       0x21
																		
																		
																		

/*信息输出*/
#define OV7725_exit_DEBUG_ON         0

#define OV7725_exit_INFO(fmt,arg...)           printf("<<-OV7725_exit-INFO->> "fmt"\n",##arg)
#define OV7725_exit_ERROR(fmt,arg...)          printf("<<-OV7725_exit-ERROR->> "fmt"\n",##arg)
#define OV7725_exit_DEBUG(fmt,arg...)          do{\
                                          if(OV7725_exit_DEBUG_ON)\
                                          printf("<<-OV7725_exit-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

extern int OV7725_exit_SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
extern int OV7725_exit_SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);
extern void OV7725_exit_SCCB_GPIO_Config(void);
								  
										  
										  
										  
void OV7725_exit_GPIO_Config(void);
ErrorStatus OV7725_exit_Init(void);
void OV7725_exit_Light_Mode(uint8_t mode);
void OV7725_exit_Color_Saturation(int8_t sat);
void OV7725_exit_Brightness(int8_t bri);
void OV7725_exit_Contrast(int8_t cnst);
void OV7725_exit_Special_Effect(uint8_t eff);			
void OV7725_exit_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,uint8_t QVGA_VGA);
                                          
										  
										  
#endif























