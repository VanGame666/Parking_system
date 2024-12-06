#ifndef __License_plate_recognition_H
#define __License_plate_recognition_H 
	   
#include "stm32f10x.h"

#include "OV7725_exit.h"
#include "OV7725_entrance.h"


#define  exit_LP   1  //ʻ��ͼ��
#define  entrance_LP   0   //ʻ��ͼ��

#define  threshold_line  3            //����������õ���ֵ
#define  exit_threshold_column   11    //���ڶ�λ������������ұ߽�Ķ�ֵ����ֵ
#define  entrance_threshold_column 11  //��ڶ�λ������������ұ߽�Ķ�ֵ����ֵ
#define  exit_threshold  23          //���ڵĳ��������ֵ��
#define  entrance_threshold   25      //��ڵĳ��������ֵ��


/*****************�ӳ��ڵ�����ͷ��FIFO����һ��rgb565����***************************/
#define exit_READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  OV7725_exit_FIFO_RCLK_L();\
	                                  RGB565 = (OV7725_exit_DATA_GPIO_PORT->IDR<<6) & 0xff00;\
	                                  OV7725_exit_FIFO_RCLK_H();\
                                    OV7725_exit_FIFO_RCLK_L();\
	                                  RGB565 |= (OV7725_exit_DATA_GPIO_PORT->IDR>>2) & 0x00ff;\
	                                  OV7725_exit_FIFO_RCLK_H();\
                                    }while(0)   

                                    
/*****************����ڵ�����ͷ��FIFO����һ��rgb565����***************************/
#define entrance_READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  OV7725_FIFO_RCLK_L();\
	                                  RGB565 = (OV7725_DATA_GPIO_PORT->IDR<<8) & 0xff00;\
	                                  OV7725_FIFO_RCLK_H();\
                                    OV7725_FIFO_RCLK_L();\
	                                  RGB565 |= (OV7725_DATA_GPIO_PORT->IDR) & 0x00ff;\
	                                  OV7725_FIFO_RCLK_H();\
                                    }while(0)                                                                      

void Entrance_LP_communication(void);
void Exit_LP_communication(void);                                  
uint8_t Number_recognition_LP(uint8_t Doorway);
void Areap_LP(uint8_t Doorway);
void Area_bin(uint8_t Doorway);                                   
uint8_t Character_segmentation(void);
void Scaling_Normalization(uint8_t n,uint16_t Left_split_line,uint16_t Right_split_line,uint16_t Upper_split_line,uint16_t Lower_split_line);
uint8_t Character_matching(uint8_t num);    
																		
#endif
