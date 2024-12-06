#include "config.h"
#include "font.h" 
#include "LCD_Basic_application.h"

extern const uint8_t ZF[];
extern const uint8_t ZM[];
/********************************************************************
 * ������:DrawPoint
 * ����  :���һ����
 * ����  :x:���λ��x����
           y:���λ��y����
           color:Ҫ������ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD->LCD_RAM=color;
}

/********************************************************************
 * ������:DrawLine
 * ����  :����
 * ����  :xStar,yStar:�������
           xEnd,yEnd: �յ����� 
           color:�ߵ���ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawLine(uint16_t xStar,uint16_t yStar,uint16_t xEnd,uint16_t yEnd,uint16_t color)
{
	uint16_t i,max_distance; 
	int x_distance,y_distance,x_increment,y_increment; 
 
  x_distance=xEnd-xStar;
  y_distance=yEnd-yStar;
  
  if(x_distance>0)x_increment=1;
  else if(x_distance==0)x_increment=0;
  else {x_increment=-1;x_distance=-x_distance;}
  
  if(y_distance>0)y_increment=1;
  else if(y_distance==0)y_increment=0;
  else {y_increment=-1;y_distance=-y_distance;}
  
  if(x_distance==0)
  {
    max_distance=y_distance+1;
    
    for(i=0;i<max_distance;i++)
      DrawPoint(xStar,yStar+i*y_increment,color);  
    return ;
  }
  
  if(y_distance==0)
  {
    max_distance=x_distance+1;
    for(i=0;i<max_distance;i++)
      DrawPoint(xStar+i*x_increment,yStar,color);  
    return ;
  }
  
  if(x_distance>y_distance)
  {
    max_distance=x_distance+1;
    for(i=0;i<max_distance;i++)
      DrawPoint(xStar+i*x_increment,yStar+i*y_distance/x_distance*y_increment, color);
  }
  else 
  {
    max_distance=y_distance+1;
    for(i=0;i<max_distance;i++)
      DrawPoint(xStar+i*x_distance/y_distance*x_distance,yStar+i*y_increment, color);    
  }
}

/********************************************************************
 * ������:DrawHollowRectangle
 * ����  :������
 * ����  :(x1,y1),(x2,y2):����ĶԽ����� 
           color:�ߵ���ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawHollowRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	DrawLine(x1,y1,x2,y1,color);
	DrawLine(x1,y1,x1,y2,color);
	DrawLine(x1,y2,x2,y2,color);
	DrawLine(x2,y1,x2,y2,color);
}  

/********************************************************************
 * ������:DrawSolidRectangle
 * ����  :������
 * ����  :(x1,y1),(x2,y2):����ĶԽ����� 
           color:�������ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawSolidRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{  	

	uint32_t i;				
	uint16_t width=x2-x1+1; 		//�õ����Ŀ��
	uint16_t height=y2-y1+1;		//�߶�
	uint32_t totalpoint=height*width;
  
	LCD_SetWindows(x1,y1,x2-1,y2-1);//������ʾ����

	for(i=0;i<totalpoint;i++)
	{
		LCD->LCD_RAM=color;	//д������ 	 
	}
  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��
}

/********************************************************************
 * ������:DrawCircular_SymmetryPoint
 * ����  :��Բ���ϵĶԳƵ㣬����Բ����8��Բ����
 * ����  :(x_centre,y_centre):Բ������
           (x_edge,y_edge):Բ�����Բ������
           color:�����ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawCircular_SymmetryPoint(uint16_t x_centre,uint16_t y_centre,uint16_t x_edge,uint16_t y_edge,uint16_t color)
{
	DrawPoint(x_centre + x_edge, y_centre + y_edge, color);

	DrawPoint(x_centre - x_edge, y_centre + y_edge, color);

	DrawPoint(x_centre + x_edge, y_centre - y_edge, color);

	DrawPoint(x_centre - x_edge, y_centre - y_edge, color);

	DrawPoint(x_centre + y_edge, y_centre + x_edge, color);

	DrawPoint(x_centre - y_edge, y_centre + x_edge, color);

	DrawPoint(x_centre + y_edge, y_centre - x_edge, color);

	DrawPoint(x_centre - y_edge, y_centre - x_edge, color);  
}

/********************************************************************
 * ������:DrawHollowCircular
 * ����  :Bresenham��Բ���㷨
 * ����  :(x_centre,y_centre):Բ������
           Radius:Բ�İ뾶
           color:�ߵ���ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawHollowCircular(uint16_t x_centre,uint16_t y_centre,uint16_t Radius,uint16_t color)
{
  int x = 0, y = Radius, p;
  
  p = 3 - 2 * Radius ;
  
  while (x <= y) 
  {
    DrawCircular_SymmetryPoint(x_centre, y_centre, x, y, color);
    if (p < 0)
    {
      p = p + 4 * x + 6;
    } 
    else
    {
      p = p + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

/********************************************************************
 * ������:DrawSolidCircular
 * ����  :����Bresenham��Բ���㷨��ʵ��Բ
 * ����  :(x_centre,y_centre):Բ������
           Radius:Բ�İ뾶
           color:ʵ��Բ����ɫ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawSolidCircular(uint16_t x_centre,uint16_t y_centre,uint16_t Radius,uint16_t color)
{
  int x = 0, y = Radius, p,yi;
  
  p = 3 + 2 * Radius;
  
  while (x <= y) 
  {
    for(yi=x;yi<=y;yi++)
      DrawCircular_SymmetryPoint(x_centre, y_centre, x, yi, color);
    if (p < 0)
    {
      p = p + 4 * x + 6;
    } 
    else
    {
      p = p + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

/********************************************************************
 * ������:ShowCP
 * ����  :��ʾ���ƺ�
 * ����  :(x,y):���ƺ���ʾλ�õ���ʼ����
           fc:������ɫ
           bc:������ɫ
           *str:�ַ�����ַ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void ShowCP(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t *str)
{  
  uint16_t i,j,color;							  	  
  uint8_t num,str1,w; 
  
  DrawSolidRectangle(x-2,y-2,x+182,y+52,bc);
  
  LCD_SetWindows(x,y,x+24-1,y+50-1);//���õ���������ʾ����
  for(i=0;i<150;i++)
  {      
    str1=ZF[str[0]*150+i]; 
      for(j=0;j<8;j++)
      {
        w=7-j;
        
        if((str1&(1<<w))==(1<<w))
            color=fc;
          else
            color=bc;
          LCD->LCD_RAM=color;	//д������ 	
      }
  }
  LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��   
  x+=26;
  
  for(num=1;num<7;num++)
  {
    LCD_SetWindows(x,y,x+24-1,y+50-1);//���õ���������ʾ����
    for(i=0;i<150;i++)
    {      
      str1=ZM[str[num]*150+i]; 
        for(j=0;j<8;j++)
        {
          w=7-j;
          
          if((str1&(1<<w))==(1<<w))
              color=fc;
            else
              color=bc;
            LCD->LCD_RAM=color;	//д������ 	
        }
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��   
    x+=26;
  } 
}

/********************************************************************
 * ������:ShowYW24
 * ����  :��ʾ��λ��Ϣ��������
 * ����  :(x,y):��������ʾλ�õ���ʼ����
           fc:������ɫ
           bc:������ɫ
           *str:�ַ�����ַ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void ShowYW24(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char *str)
{  
	uint16_t i,j,color;							  	  
  uint8_t num,str1,w;     
  while(*str!=0)
  { 
    if(x>(lcddev.width-24)||y>(lcddev.height-50)) 
    return; 
    else
    {
      num=*str-'A';
    }
    LCD_SetWindows(x,y,x+24-1,y+50-1);
    for(i=0;i<150;i++)
    {                 
     str1=YW[num*150+i]; 
      for(j=0;j<8;j++)
      {
        w=7-j;
        
        if((str1&(1<<w))==(1<<w))
            color=fc;
          else
            color=bc;
          LCD->LCD_RAM=color;	
      }
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1); 
    x+=24;  
    str++; 
  }
}      

/********************************************************************
 * ������:ShowChar
 * ����  :��ʾ�����ַ�����ĸ�����֣�
 * ����  :(x,y):�ַ���ʾλ�õ���ʼ����
           fc:������ɫ
           bc:������ɫ
           str:�ַ�
           size:�ַ���С(24,16)
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char str,uint8_t size,uint8_t mode)
{  
  uint8_t temp,pos,t,w,num;

  if(size==24)  
  {
    if(str-'0'>9)num=str-'A'+10;
    else num=str-'0';//�õ�ƫ�ƺ��ֵ
    LCD_SetWindows(x,y,x+16-1,y+24-1);//���õ���������ʾ����
    
    if(!mode) //�ǵ��ӷ�ʽ
    {     
      for(pos=0;pos<48;pos++)
      {
        temp=number24[num][pos];
        for(t=0;t<8;t++)
        {    
          w=7-t;
          if((temp&(1<<w))==(1<<w))
            LCD->LCD_RAM=fc; 
          else 
            LCD->LCD_RAM=bc;            
        }
      }
    }
    else
    {
      for(pos=0;pos<48;pos++)
      {
        temp=number24[num][pos];
        for(t=0;t<8;t++)
        {    
            w=7-t;
            if((temp&(1<<w))==(1<<w))
            DrawPoint(x+t+8*(pos%2),y+pos/2,fc);                
        }
      }
    }
  } 
  else
  {    
    num=str-' ';

    LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
    if(!mode) //�ǵ��ӷ�ʽ
    {
      for(pos=0;pos<size;pos++)
      {
        temp=asc2_1608[num][pos];		 //����1608����
        for(t=0;t<size/2;t++)
        {                 
          if(temp&0x01)
            LCD->LCD_RAM=fc; 
          else 
            LCD->LCD_RAM=bc; 
          temp>>=1;           
        }       
      }	
    }
    else//���ӷ�ʽ
    {
      for(pos=0;pos<size;pos++)
      {
        temp=asc2_1608[num][pos];		 //����1608����
        for(t=0;t<size/2;t++)
          {   
          POINT_COLOR=fc;              
              if(temp&0x01)
                DrawPoint(x+t,y+pos,fc);//��һ����    
              temp>>=1; 
          }
      }
    }
  }	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��    	   	 	  
} 
 
/********************************************************************
 * ������:ShowString
 * ����  :��ʾ�ַ���
 * ����  :(x,y):�ַ�����ʾλ�õ���ʼ����
           fc:������ɫ
           bc:������ɫ
           *s:�ַ�������ʼ��ַ
           size:�ַ���С(24,16)
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void ShowString(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t size,uint8_t mode)
{         
    while((*s<='~')&&(*s>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {   
      if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
      return; 
      
      ShowChar(x,y,fc,bc,*s,size,mode);
      x+=size/2;
      s++;
    }  
} 

/********************************************************************
 * ������:DrawFont16
 * ����  :��ʾ����16X16��������
 * ����  :(x,y):��ʼ����
           fc:������ɫ
           bc:������ɫ
           *s:�ַ�����ַ
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawFont16(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
  
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//�Զ�ͳ�ƺ�����Ŀ
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
      LCD_SetWindows(x,y,x+16-1,y+16-1);
      for(i=0;i<16*2;i++)
      {
        for(j=0;j<8;j++)
        {	
          if(!mode) //�ǵ��ӷ�ʽ
          {
            if(tfont16[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont16[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//��һ����
            x++;
            if((x-x0)==16)
            {
              x=x0;
              y++;
              break;
            }
          }
        }
      }			
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��  
} 

/********************************************************************
 * ������:DrawFont24
 * ����  :��ʾ����24X24��������
 * ����  :(x,y):��ʼ����
           fc:������ɫ
           bc:������ɫ
           *s:�ַ�����ַ
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawFont24(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//�Զ�ͳ�ƺ�����Ŀ

  for (k=0;k<HZnum;k++) 
  {
    if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
    { 	
      LCD_SetWindows(x,y,x+24-1,y+24-1);
      for(i=0;i<24*3;i++)
      {
        for(j=0;j<8;j++)
        {
          if(!mode) //�ǵ��ӷ�ʽ
          {
            if(tfont24[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont24[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//��һ����
            x++;
            if((x-x0)==24)
            {
              x=x0;
              y++;
              break;
            }
          }
        }
      }			
    }				  	
    continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
  }
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��  
}

/********************************************************************
 * ������:DrawFont32
 * ����  :��ʾ����32X32��������
 * ����  :(x,y):��ʼ����
           fc:������ɫ
           bc:������ɫ
           *s:�ַ�����ַ
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/
void DrawFont32(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//�Զ�ͳ�ƺ�����Ŀ
	for (k=0;k<HZnum;k++) 
  {
    if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
    { 	
      LCD_SetWindows(x,y,x+32-1,y+32-1);
      for(i=0;i<32*4;i++)
      {
        for(j=0;j<8;j++)
        {
          if(!mode) //�ǵ��ӷ�ʽ
          {
            if(tfont32[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont32[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//��һ����
            x++;
            if((x-x0)==32)
            {
              x=x0;
              y++;
              break;
            }
          }
        }
      }     
    }				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��  
} 

/********************************************************************
 * ������:DrawFont16
 * ����  :��ʾһ���ַ���,������Ӣ����ʾ
 * ����  :(x,y):��ʼ����
           fc:������ɫ
           bc:������ɫ
           *str:�ַ�����ַ
           size:�����С
           mode:ģʽ  0,���ģʽ;1,����ģʽ
 * ���  :��
 * ע��  :��
 ********************************************************************/   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char *str,uint8_t size,uint8_t mode)
{					
	uint16_t x0=x;							  	  
  uint8_t bHz=0;     //�ַ��������� 
  while(*str!=0)//����δ����
  { 
    if(!bHz)
    {
      if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
      return; 
      
      if(*str>0x80)bHz=1;//���� 
      else              //�ַ�
      {          
        if(*str==0x0D)//���з���
        {         
          y+=size;
          x=x0;
          str++; 
        }  
        else
        {
          POINT_COLOR=fc;
          if(size>16)//
          {  
            ShowChar(x,y,fc,bc,*str,24,mode);
            x+=16; //�ַ�,Ϊȫ�ֵ�һ�� 
          }
          else
          {
            ShowChar(x,y,fc,bc,*str,size,mode);
            x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
          }
        } 
        str++;       
      }
    }
    else//���� 
    {   
      if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
      return;  
            
      bHz=0;//�к��ֿ�    
      if(size==32)
      DrawFont32(x,y,fc,bc,str,mode);	 	
      else if(size==24)
      DrawFont24(x,y,fc,bc,str,mode);	
      else
      DrawFont16(x,y,fc,bc,str,mode);
      
      str+=2; 
      x+=size;//��һ������ƫ��	    
    }						 
  }   
}

  

 
