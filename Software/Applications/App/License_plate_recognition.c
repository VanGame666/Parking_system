#include "config.h"
#include "zimo.h"
#include "License_plate_recognition.h"

const char CPZM[]={'0','1','2','3','4','5','6','7','8','9','A','B','E','F','G','H','K','Q','U','N','R'};//车牌字符库中字符的相对排序
const unsigned char CPHZ[][3]={"京","苏","甘"};//车牌字库中汉字的相对排序
static uint16_t boundary[4]={0};//车牌区域的边界
static uint8_t cp_xp[15000]={0};//车牌区域的二值化数据
static uint8_t k[150]={0};//车牌字符归一化后的数据
uint8_t number_LP[7]={0};//车牌字符对应字库的序号

extern uint8_t OV7725_exit_vsync;
extern uint8_t OV7725_entr_vsync;

/********************************************************************
 * 函数名:Exit_LP_communication
 * 描述  :驶出的车牌信息发给上位机
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void Exit_LP_communication(void)
{
  char cStr [10] = {0}; //存储车牌字符   
  sprintf ( cStr, "%s%c.%c%c%c%c%c",CPHZ[number_LP[0]],CPZM[number_LP[1]],CPZM[number_LP[2]],CPZM[number_LP[3]],CPZM[number_LP[4]],CPZM[number_LP[5]],CPZM[number_LP[6]]);   
  strEsp8266_Fram_Record .InfBit .FramLength = 0;
  macESP8266_Usart ("C%s\r\n",cStr); //将车牌号发送给上位机
  rt_thread_mdelay(500);
  while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag ); //检测是否接收完了
  strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';//赋结束符
  Show_Str(78,312,BLACK,WHITE,"Parking Time:",16,1);
  Show_Str(182,308,RED,WHITE,strEsp8266_Fram_Record.Data_RX_BUF,24,0);
  Show_Str(182+(strEsp8266_Fram_Record .InfBit .FramLength)*16,312,RED,WHITE,"S",16,1);//液晶屏显示停留时间
}

/********************************************************************
 * 函数名:Entrance_LP_communication
 * 描述  :驶入的车牌信息发给上位机
 * 输入  :无
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void Entrance_LP_communication(void)
{
  char cStr [10] = {0};   //存储车牌字符     
  sprintf ( cStr, "%s%c.%c%c%c%c%c",CPHZ[number_LP[0]],CPZM[number_LP[1]],CPZM[number_LP[2]],CPZM[number_LP[3]],CPZM[number_LP[4]],CPZM[number_LP[5]],CPZM[number_LP[6]]);
  macESP8266_Usart ("L%s\r\n",cStr);//将车牌号发送给上位机
}

/********************************************************************
 * 函数名:Number_recognition_LP
 * 描述  :车牌识别及显示
 * 输入  :1为出口的摄像头采集的图像、0为入口的摄像头采集的图像
 * 输出  :识别结果0为识别失败、1为识别成功
 * 注意  :处理采集到的图像，进行车牌识别   
 ********************************************************************/
uint8_t Number_recognition_LP(uint8_t Doorway)
{
  uint8_t Recognition_result=0;//识别结果
  
  LP_display();//显示车牌识别状态
  if(Doorway==0) //入口的摄像头采集的图像
  {      
      OV7725_entr_FIFO_PREPARE; //准备从FIFO中读取数据
      Areap_LP(entrance_LP);//车牌区域识别
      OV7725_entr_FIFO_PREPARE; //准备从FIFO中读取数据   
      Area_bin(entrance_LP);//车牌区域二值化并将数据存储
  }
  else //出口的摄像头采集的图像
  {
      OV7725_exit_FIFO_PREPARE; //准备从FIFO中读取数据
      Areap_LP(exit_LP);//车牌区域识别
      OV7725_exit_FIFO_PREPARE; //准备从FIFO中读取数据      
      Area_bin(exit_LP); //车牌区域二值化并将数据存储 
  } 
  
  Recognition_result=Character_segmentation();//车牌字符分割、缩放、匹配
  if(Recognition_result==0)
  {
    Show_Str(112,261,RED ,BLUE ,"识别失败",24,0);//显示识别失败
  }
  else
  {
    ShowCP(75,250,WHITE,BLUE,number_LP);//显示车牌号
  }
  return Recognition_result;//返回识别结果
}

/********************************************************************
 * 函数名:Areap_LP
 * 描述  :车牌区域识别
 * 输入  :1为出口的摄像头采集的图像、0为入口的摄像头采集的图像
 * 输出  :无
 * 注意  :无        
 ********************************************************************/
void Areap_LP(uint8_t Doorway)
{
  const uint16_t width=320,height=240;//采集图像的大小
	uint16_t i,j;//循环
  uint16_t r,g,b;//rgb像素值
  uint16_t lr=0,num=0,max_num=0,threshold_column;//前一点的灰度化值，累计点数，最多点数数值，二值化阈值
	uint16_t Camera_Data,Q_regional_datum_column=0,Regional_datum_column,Regional_datum_line;//图像像素值，第24点的列位置，最多跳变点的第24点列位置，最多跳变点的行位置
  uint8_t Jump_point[240]={0},Datum_line_Data[320]={0},Data_Grayscale,sign=0;//每行的跳变点数目，最多跳变点的下一行整行像素值，加权平均值，标记
  
  if(Doorway==1) //出口的车牌采集处理
  {
    threshold_column=exit_threshold_column;
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
          exit_READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */

          r=(Camera_Data>>11);   //取出rgb565 中的r值
          g=(Camera_Data&0x07e0)>>6;//取出rgb565 中的g值右移1位
          b=Camera_Data&0x001f;//取出rgb565 中的b值

          Data_Grayscale=0.30*r+0.59*g+0.11*b;//加权平均值计算
        
        if(sign==1)//上一行是最多跳变点的一行
          Datum_line_Data[j]=Data_Grayscale;//存储跳变点数目最多的下一行的每点加权平均值
    /**********************************************************/
       if(lr>Data_Grayscale)//累计跳变点数目
        {         
          if(lr-Data_Grayscale>threshold_line)
          {
            num++;
          }        
        }
        else
        {          
          if(Data_Grayscale-lr>threshold_line)
          {
            num++;
          } 
        }
        
        if(num==24)//跳变点第24点
          Q_regional_datum_column=j;//记录跳变点第24点的列位置
        lr=Data_Grayscale;//先一点的加权平均值
        
      }
      Jump_point[i]=num;//存储每行的跳变点数目
      sign=0;//标记是否出现最多跳变点
      if(num>max_num)//判断最多跳变点
      {
        Regional_datum_line=i;//记录跳变点最多的行位置
        Regional_datum_column=Q_regional_datum_column;//记录跳变点最多的那行第24点的列位置
        sign=1;//标记出现最多跳变点了
        max_num=num;//记录目前跳变点最多的数目
      }
      
      num=0;lr=0; 
    }
  }
  else  //入口的车牌采集处理
  {
  threshold_column=entrance_threshold_column;
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        entrance_READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
                
        r=(Camera_Data>>11);   //取出RGB565 中的R值
        g=(Camera_Data&0x07e0)>>5;
        b=Camera_Data&0x001f;
        Data_Grayscale=0.30*r+0.59*g+0.11*b;
        if(sign==1)
          Datum_line_Data[j]=Data_Grayscale;
        
    /**********************************************************/
       if(lr>Data_Grayscale)
        {         
          if(lr-Data_Grayscale>threshold_line)
          {
            num++;
          }   
        }
        else
        {       
          if(Data_Grayscale-lr>threshold_line)
          {
            num++;
          } 
        }
        
        if(num==24)
          Q_regional_datum_column=j;
        lr=Data_Grayscale;
      
      }
      Jump_point[i]=num;
      sign=0;
      
      if(num>max_num)
      {
        Regional_datum_line=i;
        Regional_datum_column=Q_regional_datum_column;
        max_num=num;
        sign=1;
      }
      
      num=0;lr=0; 
    }
  }
    
  for(i = Regional_datum_line; i < height; i++) //识别车牌区域的下边界
  {
    if(Jump_point[i]<10)
      num++;

    if(num>3)
    {
      boundary[1]=i;
      break;
    }
  }
  num=0;
  for(i = Regional_datum_line; i >0; i--) //识别车牌区域的上边界
  {
    if(Jump_point[i]<10)
        num++;

    if(num>3)
    {
      boundary[0]=i;
      break;
    }
  } 
/***************************************************************************************/
  num=0;
  for(i=Regional_datum_column;i>0;i--) //识别车牌区域的左边界
  {
    if(Datum_line_Data[i]<threshold_column)
    {
      num++;
    }
    else
    {
      num=0;
    }
    
    if(num>4)
    {
      boundary[2]=i+num;
      break;
    } 
  }
  if(i==0)boundary[2]=0;
  
  num=0;
  for(i=Regional_datum_column;i<320;i++) //识别车牌区域的右边界
  {
    if(Datum_line_Data[i]<threshold_column)
    {
      num++;
    }
    else
    {
      num=0;
    }
    
    if(num>4)
    {
      boundary[3]=i-num;
      break;
    }    
  }
  if(i==320)boundary[3]=319; 
  
  if(boundary[3]-boundary[2]>230)//判断左右边界是否过大
  {
    if((boundary[3]-Regional_datum_column) < (Regional_datum_column-boundary[2]))//左边界过大
    {
      boundary[2]=230+Regional_datum_column*2-boundary[3];
    }
    else if((boundary[3]-Regional_datum_column) > (Regional_datum_column-boundary[2]))//右边界过大
    {
      boundary[3]=230+boundary[2];
    }
    else//左右边界都过大
    {
      boundary[3]=Regional_datum_column+100;
      boundary[2]=Regional_datum_column+100;
    }
  
  }
  
}

/********************************************************************
 * 函数名:Area_bin
 * 描述  :车牌区域二值化并将数据存储
 * 输入  :1为出口的摄像头采集的图像、0为入口的摄像头采集的图像
 * 输出  :无
 * 注意  :无        
 ********************************************************************/
void Area_bin(uint8_t Doorway)
{
  const uint16_t width=320,height=240;//图像大小
  uint16_t i,j,r,g,b,Camera_Data,num;


  num=boundary[3]-boundary[2];//定位到的车牌区域的宽度
 
  if(Doorway==1) //出口的车牌图像二值化
  {
   for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        exit_READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
        if(i>=boundary[0]&&i<boundary[1]&&j>=boundary[2]&&j<boundary[3])
        {
          r=(Camera_Data>>11);   //取出RGB565 中的R值
          g=(Camera_Data&0x07e0)>>6;
          b=Camera_Data&0x001f;

          Camera_Data=0.30*r+0.59*g+0.11*b;
          if(Camera_Data>=exit_threshold)
          {
            Camera_Data=1;
          }
          else
          {
            Camera_Data=0;
          }
          cp_xp[(i-boundary[0])*num+j-boundary[2]]=Camera_Data;
        }
      }
    }
  }
  else //入口的车牌图像二值化
  {
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        entrance_READ_FIFO_PIXEL(Camera_Data);		/* 从FIFO读出一个rgb565像素到Camera_Data变量 */
        if(i>=boundary[0]&&i<boundary[1]&&j>=boundary[2]&&j<boundary[3])
        {
          r=(Camera_Data>>11);   //取出RGB565 中的R值
          g=(Camera_Data&0x07e0)>>6;
          b=Camera_Data&0x001f;

          Camera_Data=0.30*r+0.59*g+0.11*b;
          if(Camera_Data>=entrance_threshold)
          {
            Camera_Data=1;
          }
          else
          {
            Camera_Data=0;
          }
          cp_xp[(i-boundary[0])*num+j-boundary[2]]=Camera_Data;
        }
      }
    } 
  } 
}

/********************************************************************
 * 函数名:Character_segmentation
 * 描述  :车牌字符分割
 * 输入  :无
 * 输出  :0:分割不完整
           1:分割完整
 * 注意  :函数中也调用了字符缩放与归一化的函数
 ********************************************************************/
uint8_t Character_segmentation(void)
{
	uint16_t i,j,height,width,num=0,sign=0; 
	uint16_t n,split_line_column[16]={0},split_line_level[16]={0};//序号，车牌字符的左右列位置，车牌字符的上下行位置

  height=boundary[1]-boundary[0];//定位到的车牌区域的高度
  width=boundary[3]-boundary[2];//定位到的车牌区域的宽度
  
   
  for(i = height/2; i < height; i++) //车牌字符整体的下边界
	{
		for(j = 0; j < width-1; j++)
		{
      n=i*width+j;
      if(cp_xp[n] != cp_xp[n+1])
      {
        num++;
      }
    }
    
    if(num<10)
    {
        boundary[1]=i;break;
    }
    num=0;
  }
  if(num==0)boundary[1]=height;
  
  num=0;
	for(i = height/2; i > 0; i--)//车牌字符整体的上边界
	{
		for(j = 0; j < width-1; j++)
		{
      n=i*width+j;
      if(cp_xp[n] != cp_xp[n+1])
      {
        num++;
      }
    }
    
    if(num<10)
    {
        boundary[0]=i;break;
    }
    num=0;
  }
  if(num==0)boundary[0]=0;
/********************************************************************/  
  
  for(j = (width/2); j>0; j--)//车牌点字符的左右边界
	{
		for(i= boundary[0];i<boundary[1];i++)
		{
        if(cp_xp[i*width+j]==1)
        {
          if(sign==0)
          {
            split_line_column[15]=j;
          }
          sign=1;
          break; 
          
        }
        if(sign==1)
           sign=2;
    }
    if(sign==2)
    {
     split_line_column[14]=j+1;
      sign=0;      
      if((split_line_column[15]-split_line_column[14]<8) && (split_line_column[15]!=(width/2)) && (cp_xp[(boundary[0]+3)*width+split_line_column[15]])!=1)
          break;
    }       
  }
  
  /*********************************************************************/
  
  num=4;
  sign=0;
  for(j = split_line_column[15]+1; j<width; j++)//车牌点字符的右边五个字符的左右边界
	{
		for(i= boundary[0];i<boundary[1];i++)
		{
        if(cp_xp[i*width+j]==1)
        {
          if(sign==0)
          {
            split_line_column[num]=j;
            num++;
          }
          sign=1;
          break; 
          
        }
        if(sign==1)
           sign=2;
    }
    if(sign==2)
    {
      split_line_column[num]=j-1;
      sign=0;
      
      if(num==13)
        break;     
      num++;
    }       
  }
  

  /***********************************************************************/
  if(num<13)//车牌字符不完整，重新采集
  {
    return 0;
  }
  /***********************************************************************/
  
  num=3;
  sign=0;
  for(j = split_line_column[14]-1; j>0; j--)//车牌点字符的左边两个字符的左右边界
	{
		for(i= boundary[0];i<boundary[1];i++)
		{
        if(cp_xp[i*width+j]==1)
        {
          if(sign==0)
          {
            split_line_column[num]=j;
            num--;
          }
          sign=1;
          break; 
          
        }
        if(sign==1)
           sign=2;
    }
    if(sign==2)
    {
      split_line_column[num]=j+1;
      sign=0;      
      num--;
    }  
    if(num==0)
    {     
      break;
    }
      
  }
  



  /**********************************************************************/  
  if(num>0)//车牌字符不完整，重新采集
  {
    return 0;
  }
  
  if(num==0)//车牌中汉字字符的左边界
  {
    sign=0;    
    for(j = (split_line_column[1]-(split_line_column[3]-split_line_column[2])); j<split_line_column[1]; j++)
    {
      for(i= boundary[0];i<boundary[1];i++)
      {
          if(cp_xp[i*width+j]==1)
          {
            split_line_column[0]=j;
            sign=1;
            break; 
          }          
      }
      if(sign==1)
        break;  
    }  
  }
  

  for(n=0;n<14;n+=2)
  { 
    sign=0;
    for(i= boundary[0];i<boundary[1];i++)//精确车牌各字符的上边界
    {
      for(j = split_line_column[n]; j<=split_line_column[n+1]; j++)
      {
          if(cp_xp[i*width+j]==1)
          {
            split_line_level[n]=i;
            sign=1;
            break;   
          }            
      }
      if(sign==1)
        break;
    }
    sign=0;
    for(i = boundary[1]; i>=boundary[0];i--)//精确车牌各字符的下边界
    {
      for(j = split_line_column[n]; j<=split_line_column[n+1]; j++)
      {
          if(cp_xp[i*width+j]==1)
          {
            split_line_level[n+1]=i;
            sign=1;
            break;
          }            
      }
      if(sign==1)
        break;
    }
  }
  
 /*   去除车牌中四个螺母的地方     */ 
  {
    split_line_level[2]=split_line_level[4];
    split_line_level[3]=split_line_level[5];
    
    split_line_level[10]=split_line_level[12];
    split_line_level[11]=split_line_level[13]; 
  }
 
  for(n=0;n<14;n+=2)//车牌各字符进行缩放与归一化
  {
    Scaling_Normalization(n/2,split_line_column[n],split_line_column[n+1],split_line_level[n],split_line_level[n+1]);//车牌字符进行缩放与归一化
    number_LP[n/2]=Character_matching(n/2); //车牌字符匹配，存储匹配结果
  }
 
  return 1;//车牌字符完整
}
  
/********************************************************************
 * 函数名:Scaling_Normalization
 * 描述  :车牌字符缩放与归一化
 * 输入  :字符的序号、字符的左边界、字符的右边界、字符的上边界、字符的下边界
 * 输出  :无
 * 注意  :无
 ********************************************************************/
void Scaling_Normalization(uint8_t n,uint16_t Left_split_line,uint16_t Right_split_line,uint16_t Upper_split_line,uint16_t Lower_split_line)
{
  uint16_t i,j,num=0,height,width; 
	uint16_t Character_seg_value[1200]={0};
  uint8_t Date_normalization,Displacement;
  
  num=boundary[3]-boundary[2];//定位到的车牌区域的宽度  
  width=Right_split_line - Left_split_line;//字符的宽度
  height=Lower_split_line - Upper_split_line;//字符的高度

  for(i=0;i<50;i++)//车牌字符缩放
  {
    for(j=0;j<24;j++)
    {     /* 存储字符缩放后的二值化数据 */
      Character_seg_value[i*24+j]=cp_xp[(i*height/49+Upper_split_line)*num + (j*width/23+Left_split_line)];     
    }
  }
  
  for(i=0;i<150;i++)//车牌字符归一化
  {
    Date_normalization=0;
    for(j=0;j<8;j++)//每8个像素点整合为1个字节
    {
      Displacement=7-j;
      if(Character_seg_value[i*8+j]==1)
      {
        Date_normalization |= 1<<Displacement;             
      }
    }
    k[i]=Date_normalization;//存储归一化后的数据  
  }
}

/********************************************************************
 * 函数名:Character_matching
 * 描述  :车牌字符匹配
 * 输入  :车牌字符的序号
 * 输出  :匹配到相似度最高的字库中对应的序号
 * 注意  :无
 ********************************************************************/
uint8_t Character_matching(uint8_t num)
{
  uint16_t Max_similarity=0;
  uint8_t i,j,m,Distinguish,Data_matching,Matching;
  int Similarity;
  
  switch(num)//匹配的是那个位置的字符
  {
    case 0:      //车牌字符的第一位，即汉字
      for(m=0;m<3;m++)
      {
        Similarity=0;
        for(i=0;i<150;i++)//字符与字库的比较
        {
          Data_matching = ZF[m*150+i] ^ k[i];
          for(j=0;j<8;j++)
          {
            Matching = (Data_matching>>j)&1;
            if(Matching == 1 )
              Similarity--;
            else
              Similarity++;
          }
        }
        if(Similarity<0)
            Similarity=0;
        
        if(Max_similarity<Similarity)//取最相似的字库序号
        {
          Max_similarity=Similarity;
          Distinguish=m;//匹配到相似度最高的字库中对应的序号
        }
      }     
      break;
      
    case 1:      //车牌字符的第二位，即字母      
      for(m=10;m<21;m++)
        {
          Similarity=0;
          for(i=0;i<150;i++)//字符与字库的比较
          {
            Data_matching = ZM[m*150+i] ^ k[i];
            for(j=0;j<8;j++)
            {
              Matching = (Data_matching>>j)&1;
              if(Matching == 1 )
                Similarity--;
              else
                Similarity++;
            }
          }
          if(Similarity<0)
              Similarity=0;
          
          if(Max_similarity<Similarity)//取最相似的字库序号
          {
            Max_similarity=Similarity;
            Distinguish=m;
          }
        }
        break;
        
    default:       //车牌字符的后五位，即数字与字母
       for(m=0;m<21;m++)
        {
          Similarity=0;
          for(i=0;i<150;i++)//字符与字库的比较
          {
            Data_matching = ZM[m*150+i] ^ k[i];
            for(j=0;j<8;j++)
            {
              Matching = (Data_matching>>j)&1;
              if(Matching == 1 )
                Similarity--;
              else
                Similarity++;
            }
          }
          if(Similarity<0)
              Similarity=0;
          
          if(Max_similarity<Similarity)//取最相似的字库序号
          {
            Max_similarity=Similarity;
            Distinguish=m;
          }
        }
        break;
  }
  return Distinguish;//车牌字符与字库中相似度最高的字符序号
}



