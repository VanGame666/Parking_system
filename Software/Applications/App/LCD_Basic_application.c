#include "config.h"
#include "font.h" 
#include "LCD_Basic_application.h"

extern const uint8_t ZF[];
extern const uint8_t ZM[];
/********************************************************************
 * 函数名:DrawPoint
 * 描述  :描绘一个点
 * 输入  :x:光标位置x坐标
           y:光标位置y坐标
           color:要填充的颜色
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD->LCD_RAM=color;
}

/********************************************************************
 * 函数名:DrawLine
 * 描述  :画线
 * 输入  :xStar,yStar:起点坐标
           xEnd,yEnd: 终点坐标 
           color:线的颜色
 * 输出  :无
 * 注意  :无
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
 * 函数名:DrawHollowRectangle
 * 描述  :画方框
 * 输入  :(x1,y1),(x2,y2):方框的对角坐标 
           color:线的颜色
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawHollowRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	DrawLine(x1,y1,x2,y1,color);
	DrawLine(x1,y1,x1,y2,color);
	DrawLine(x1,y2,x2,y2,color);
	DrawLine(x2,y1,x2,y2,color);
}  

/********************************************************************
 * 函数名:DrawSolidRectangle
 * 描述  :画方块
 * 输入  :(x1,y1),(x2,y2):方块的对角坐标 
           color:方块的颜色
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawSolidRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{  	

	uint32_t i;				
	uint16_t width=x2-x1+1; 		//得到填充的宽度
	uint16_t height=y2-y1+1;		//高度
	uint32_t totalpoint=height*width;
  
	LCD_SetWindows(x1,y1,x2-1,y2-1);//设置显示窗口

	for(i=0;i<totalpoint;i++)
	{
		LCD->LCD_RAM=color;	//写入数据 	 
	}
  
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

/********************************************************************
 * 函数名:DrawCircular_SymmetryPoint
 * 描述  :画圆弧上的对称点，由于圆弧的8分圆特性
 * 输入  :(x_centre,y_centre):圆心坐标
           (x_edge,y_edge):圆上相对圆心坐标
           color:点的颜色
 * 输出  :无
 * 注意  :无
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
 * 函数名:DrawHollowCircular
 * 描述  :Bresenham画圆形算法
 * 输入  :(x_centre,y_centre):圆心坐标
           Radius:圆的半径
           color:线的颜色
 * 输出  :无
 * 注意  :无
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
 * 函数名:DrawSolidCircular
 * 描述  :利用Bresenham画圆形算法画实心圆
 * 输入  :(x_centre,y_centre):圆心坐标
           Radius:圆的半径
           color:实心圆的颜色
 * 输出  :无
 * 注意  :无
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
 * 函数名:ShowCP
 * 描述  :显示车牌号
 * 输入  :(x,y):车牌号显示位置的起始坐标
           fc:字体颜色
           bc:背景颜色
           *str:字符串地址
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void ShowCP(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t *str)
{  
  uint16_t i,j,color;							  	  
  uint8_t num,str1,w; 
  
  DrawSolidRectangle(x-2,y-2,x+182,y+52,bc);
  
  LCD_SetWindows(x,y,x+24-1,y+50-1);//设置单个文字显示窗口
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
          LCD->LCD_RAM=color;	//写入数据 	
      }
  }
  LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏   
  x+=26;
  
  for(num=1;num<7;num++)
  {
    LCD_SetWindows(x,y,x+24-1,y+50-1);//设置单个文字显示窗口
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
            LCD->LCD_RAM=color;	//写入数据 	
        }
    }
    LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏   
    x+=26;
  } 
}

/********************************************************************
 * 函数名:ShowYW24
 * 描述  :显示车位信息的区域编号
 * 输入  :(x,y):区域编号显示位置的起始坐标
           fc:字体颜色
           bc:背景颜色
           *str:字符串地址
 * 输出  :无
 * 注意  :无
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
 * 函数名:ShowChar
 * 描述  :显示单个字符（字母和数字）
 * 输入  :(x,y):字符显示位置的起始坐标
           fc:字体颜色
           bc:背景颜色
           str:字符
           size:字符大小(24,16)
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char str,uint8_t size,uint8_t mode)
{  
  uint8_t temp,pos,t,w,num;

  if(size==24)  
  {
    if(str-'0'>9)num=str-'A'+10;
    else num=str-'0';//得到偏移后的值
    LCD_SetWindows(x,y,x+16-1,y+24-1);//设置单个文字显示窗口
    
    if(!mode) //非叠加方式
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

    LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
    if(!mode) //非叠加方式
    {
      for(pos=0;pos<size;pos++)
      {
        temp=asc2_1608[num][pos];		 //调用1608字体
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
    else//叠加方式
    {
      for(pos=0;pos<size;pos++)
      {
        temp=asc2_1608[num][pos];		 //调用1608字体
        for(t=0;t<size/2;t++)
          {   
          POINT_COLOR=fc;              
              if(temp&0x01)
                DrawPoint(x+t,y+pos,fc);//画一个点    
              temp>>=1; 
          }
      }
    }
  }	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏    	   	 	  
} 
 
/********************************************************************
 * 函数名:ShowString
 * 描述  :显示字符串
 * 输入  :(x,y):字符串显示位置的起始坐标
           fc:字体颜色
           bc:背景颜色
           *s:字符串的起始地址
           size:字符大小(24,16)
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void ShowString(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t size,uint8_t mode)
{         
    while((*s<='~')&&(*s>=' '))//判断是不是非法字符!
    {   
      if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
      return; 
      
      ShowChar(x,y,fc,bc,*s,size,mode);
      x+=size/2;
      s++;
    }  
} 

/********************************************************************
 * 函数名:DrawFont16
 * 描述  :显示单个16X16中文字体
 * 输入  :(x,y):起始坐标
           fc:字体颜色
           bc:背景颜色
           *s:字符串地址
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawFont16(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
  
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	
      LCD_SetWindows(x,y,x+16-1,y+16-1);
      for(i=0;i<16*2;i++)
      {
        for(j=0;j<8;j++)
        {	
          if(!mode) //非叠加方式
          {
            if(tfont16[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont16[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//画一个点
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
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

/********************************************************************
 * 函数名:DrawFont24
 * 描述  :显示单个24X24中文字体
 * 输入  :(x,y):起始坐标
           fc:字体颜色
           bc:背景颜色
           *s:字符串地址
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawFont24(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//自动统计汉字数目

  for (k=0;k<HZnum;k++) 
  {
    if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
    { 	
      LCD_SetWindows(x,y,x+24-1,y+24-1);
      for(i=0;i<24*3;i++)
      {
        for(j=0;j<8;j++)
        {
          if(!mode) //非叠加方式
          {
            if(tfont24[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont24[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//画一个点
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
    continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
  }
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}

/********************************************************************
 * 函数名:DrawFont32
 * 描述  :显示单个32X32中文字体
 * 输入  :(x,y):起始坐标
           fc:字体颜色
           bc:背景颜色
           *s:字符串地址
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void DrawFont32(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode)
{
	uint8_t i,j;
	uint8_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//自动统计汉字数目
	for (k=0;k<HZnum;k++) 
  {
    if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
    { 	
      LCD_SetWindows(x,y,x+32-1,y+32-1);
      for(i=0;i<32*4;i++)
      {
        for(j=0;j<8;j++)
        {
          if(!mode) //非叠加方式
          {
            if(tfont32[k].Msk[i]&(0x80>>j))	LCD_WR_DATA(fc);
            else LCD_WR_DATA(bc);
          }
          else
          {
            if(tfont32[k].Msk[i]&(0x80>>j))	DrawPoint(x,y,fc);//画一个点
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
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

/********************************************************************
 * 函数名:DrawFont16
 * 描述  :显示一个字符串,包含中英文显示
 * 输入  :(x,y):起始坐标
           fc:字体颜色
           bc:背景颜色
           *str:字符串地址
           size:字体大小
           mode:模式  0,填充模式;1,叠加模式
 * 输出  :无
 * 注意  :无
 ********************************************************************/   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char *str,uint8_t size,uint8_t mode)
{					
	uint16_t x0=x;							  	  
  uint8_t bHz=0;     //字符或者中文 
  while(*str!=0)//数据未结束
  { 
    if(!bHz)
    {
      if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
      return; 
      
      if(*str>0x80)bHz=1;//中文 
      else              //字符
      {          
        if(*str==0x0D)//换行符号
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
            x+=16; //字符,为全字的一半 
          }
          else
          {
            ShowChar(x,y,fc,bc,*str,size,mode);
            x+=size/2; //字符,为全字的一半 
          }
        } 
        str++;       
      }
    }
    else//中文 
    {   
      if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
      return;  
            
      bHz=0;//有汉字库    
      if(size==32)
      DrawFont32(x,y,fc,bc,str,mode);	 	
      else if(size==24)
      DrawFont24(x,y,fc,bc,str,mode);	
      else
      DrawFont16(x,y,fc,bc,str,mode);
      
      str+=2; 
      x+=size;//下一个汉字偏移	    
    }						 
  }   
}

  

 
