#ifndef __License_plate_recognition_H
#define __License_plate_recognition_H 
	   
#include "stm32f10x.h"

#include "OV7725_exit.h"
#include "OV7725_entrance.h"


#define  exit_LP   1  //驶出图像
#define  entrance_LP   0   //驶入图像

#define  threshold_line  3            //跳变点数设置的阈值
#define  exit_threshold_column   11    //出口定位车牌区域的左右边界的二值化阈值
#define  entrance_threshold_column 11  //入口定位车牌区域的左右边界的二值化阈值
#define  exit_threshold  23          //出口的车牌区域二值化
#define  entrance_threshold   25      //入口的车牌区域二值化


/*****************从出口的摄像头的FIFO读出一个rgb565像素***************************/
#define exit_READ_FIFO_PIXEL(RGB565)   	do{\
	                                  RGB565=0;\
	                                  OV7725_exit_FIFO_RCLK_L();\
	                                  RGB565 = (OV7725_exit_DATA_GPIO_PORT->IDR<<6) & 0xff00;\
	                                  OV7725_exit_FIFO_RCLK_H();\
                                    OV7725_exit_FIFO_RCLK_L();\
	                                  RGB565 |= (OV7725_exit_DATA_GPIO_PORT->IDR>>2) & 0x00ff;\
	                                  OV7725_exit_FIFO_RCLK_H();\
                                    }while(0)   

                                    
/*****************从入口的摄像头的FIFO读出一个rgb565像素***************************/
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
