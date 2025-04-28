#include "config.h"
#include "LCD_Interface_design.h"

/********************************************************************
 * ������:Interface_init
 * ����  :��Һ������ϵͳ������ʾ
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Interface_init(void)
{
  uint8_t i;

  LCD_Clear(WHITE);//����
  Show_Str(32,15,BLUE,WHITE,"���ܳ�λ����ϵͳ",32,1);
  DrawLine(0,52,320,52,BLUE);
  Show_Str(112,58,BLACK,WHITE,"δ���ӷ�����",16,1);
  /*��ʾͣ������ؼ�����Ϣ*/
  Show_Str(84,80,BLACK,WHITE,"���복��00��",24,1);
  Show_Str(84,107,BLACK,WHITE,"ʻ������00��",24,1);
  Show_Str(84,134,BLACK,WHITE,"��������00��",24,1);
  Show_Str(84,161,BLACK,WHITE,"ʣ�೵λ00��",24,1);

  for(i=0;i<4;i++)
  {
    DrawHollowRectangle(84,78+i*27,236,105+i*27,BLACK);//������
    Show_Str(180,80+i*27,RED,WHITE,"00",24,0);//��������
  } 
  Parking_information();//��λ��Ϣ��ʾ
  alert();//��������Ϣ��ʾ
  
}
//INIT_APP_EXPORT(Interface_init);
/********************************************************************
 * ������:LP_display
 * ����  :����ʶ��״̬
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void LP_display(void)
{
  DrawSolidRectangle(0,190,319,380,WHITE);
  DrawLine(110,200,210,200,BLUE);
  DrawLine(110,206,210,206,BLUE);
  
  Show_Str(124,220,BLACK,WHITE,"���ƺ�",24,1);
  DrawSolidRectangle(73,248,257,302,BLUE);
  Show_Str(124,261,RED ,BLUE ,"ʶ����",24,1);
}

/********************************************************************
 * ������:Parking_information
 * ����  :��Һ��������ʾ��λ��Ϣ
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Parking_information(void)
{
  uint8_t i=40;
  
  DrawSolidRectangle(0,190,319,380,WHITE);
  DrawHollowRectangle(110,204,210,230,BLUE);
  
  DrawHollowRectangle(0,190+i,319,340+i,BLUE);
  Show_Str(112,205,BLACK,WHITE,"��λ��Ϣ",24,1);
  
  DrawLine(160,190+i,160,340+i,BLUE);
  DrawLine(30,190+i,30,340+i,BLUE);
  DrawLine(190,190+i,190,340+i,BLUE);  
  DrawLine(0,265+i,319,265+i,BLUE);
 
  ShowYW24(3,202+i,BLUE,WHITE,"A");
  ShowYW24(163,202+i,BLUE,WHITE,"B");
  ShowYW24(3,277+i,BLUE,WHITE,"C");
  ShowYW24(163,277+i,BLUE,WHITE,"D");
  
  DrawSolidRectangle(50,200+i,85,255+i,GREEN);//A01 
  DrawSolidRectangle(105,200+i,140,255+i,RED);//A02
  
  DrawSolidRectangle(210,200+i,245,255+i,GREEN);//B01
  DrawSolidRectangle(265,200+i,300,255+i,RED);//B02
  
  DrawSolidRectangle(50,275+i,85,330+i,RED);//C01
  DrawSolidRectangle(105,275+i,140,330+i,RED);//C02
  
  DrawSolidRectangle(210,275+i,245,330+i,RED);//D01
  DrawSolidRectangle(265,275+i,300,330+i,RED);//D02

  Show_Str(51,215+i,BLACK,GREEN,"01",24,1);
  Show_Str(106,215+i,BLACK,GREEN,"02",24,1);
  
  Show_Str(211,215+i,BLACK,GREEN,"01",24,1);
  Show_Str(266,215+i,BLACK,GREEN,"02",24,1);
  
  Show_Str(51,290+i,BLACK,GREEN,"01",24,1);
  Show_Str(106,290+i,BLACK,GREEN,"02",24,1);
  
  Show_Str(211,290+i,BLACK,GREEN,"01",24,1);
  Show_Str(266,290+i,BLACK,GREEN,"02",24,1);  
}

/********************************************************************
 * ������:alert
 * ����  :��Һ��������ʾ��������Ϣ
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void alert(void)
{
  uint8_t i;
 
  Show_Str(4,408,RED,WHITE,"��",24,1);
  Show_Str(76,408,RED,WHITE,"��",24,1);
  Show_Str(148,408,RED,WHITE,"��",24,1);
  Show_Str(220,408,RED,WHITE,"��",24,1);
  Show_Str(292,408,RED,WHITE,"��",24,1);
  
  for(i=0;i<4;i++)
  {
    DrawSolidCircular(52+72*i,420,20,BROWN);
    DrawHollowCircular(52+72*i,420,20,RED);
    DrawSolidRectangle(32+72*i,450,72+72*i,470,LGRAY);
    ShowChar(49+72*i,452,RED,LGRAY,'A'+i,16,1);
  }
}
