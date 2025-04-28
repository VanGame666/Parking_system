#include "config.h"
#include "zimo.h"
#include "License_plate_recognition.h"

const char CPZM[]={'0','1','2','3','4','5','6','7','8','9','A','B','E','F','G','H','K','Q','U','N','R'};//�����ַ������ַ����������
const unsigned char CPHZ[][3]={"��","��","��"};//�����ֿ��к��ֵ��������
static uint16_t boundary[4]={0};//��������ı߽�
static uint8_t cp_xp[15000]={0};//��������Ķ�ֵ������
static uint8_t k[150]={0};//�����ַ���һ���������
uint8_t number_LP[7]={0};//�����ַ���Ӧ�ֿ�����

extern uint8_t OV7725_exit_vsync;
extern uint8_t OV7725_entr_vsync;

/********************************************************************
 * ������:Exit_LP_communication
 * ����  :ʻ���ĳ�����Ϣ������λ��
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Exit_LP_communication(void)
{
  char cStr [10] = {0}; //�洢�����ַ�   
  sprintf ( cStr, "%s%c.%c%c%c%c%c",CPHZ[number_LP[0]],CPZM[number_LP[1]],CPZM[number_LP[2]],CPZM[number_LP[3]],CPZM[number_LP[4]],CPZM[number_LP[5]],CPZM[number_LP[6]]);   
  strEsp8266_Fram_Record .InfBit .FramLength = 0;
  macESP8266_Usart ("C%s\r\n",cStr); //�����ƺŷ��͸���λ��
  rt_thread_mdelay(500);
  while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag ); //����Ƿ��������
  strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';//��������
  Show_Str(78,312,BLACK,WHITE,"Parking Time:",16,1);
  Show_Str(182,308,RED,WHITE,strEsp8266_Fram_Record.Data_RX_BUF,24,0);
  Show_Str(182+(strEsp8266_Fram_Record .InfBit .FramLength)*16,312,RED,WHITE,"S",16,1);//Һ������ʾͣ��ʱ��
}

/********************************************************************
 * ������:Entrance_LP_communication
 * ����  :ʻ��ĳ�����Ϣ������λ��
 * ����  :��
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Entrance_LP_communication(void)
{
  char cStr [10] = {0};   //�洢�����ַ�     
  sprintf ( cStr, "%s%c.%c%c%c%c%c",CPHZ[number_LP[0]],CPZM[number_LP[1]],CPZM[number_LP[2]],CPZM[number_LP[3]],CPZM[number_LP[4]],CPZM[number_LP[5]],CPZM[number_LP[6]]);
  macESP8266_Usart ("L%s\r\n",cStr);//�����ƺŷ��͸���λ��
}

/********************************************************************
 * ������:Number_recognition_LP
 * ����  :����ʶ����ʾ
 * ����  :1Ϊ���ڵ�����ͷ�ɼ���ͼ��0Ϊ��ڵ�����ͷ�ɼ���ͼ��
 * ���  :ʶ����0Ϊʶ��ʧ�ܡ�1Ϊʶ��ɹ�
 * ע��  :����ɼ�����ͼ�񣬽��г���ʶ��   
 ********************************************************************/
uint8_t Number_recognition_LP(uint8_t Doorway)
{
  uint8_t Recognition_result=0;//ʶ����
  
  LP_display();//��ʾ����ʶ��״̬
  if(Doorway==0) //��ڵ�����ͷ�ɼ���ͼ��
  {      
      OV7725_entr_FIFO_PREPARE; //׼����FIFO�ж�ȡ����
      Areap_LP(entrance_LP);//��������ʶ��
      OV7725_entr_FIFO_PREPARE; //׼����FIFO�ж�ȡ����   
      Area_bin(entrance_LP);//���������ֵ���������ݴ洢
  }
  else //���ڵ�����ͷ�ɼ���ͼ��
  {
      OV7725_exit_FIFO_PREPARE; //׼����FIFO�ж�ȡ����
      Areap_LP(exit_LP);//��������ʶ��
      OV7725_exit_FIFO_PREPARE; //׼����FIFO�ж�ȡ����      
      Area_bin(exit_LP); //���������ֵ���������ݴ洢 
  } 
  
  Recognition_result=Character_segmentation();//�����ַ��ָ���š�ƥ��
  if(Recognition_result==0)
  {
    Show_Str(112,261,RED ,BLUE ,"ʶ��ʧ��",24,0);//��ʾʶ��ʧ��
  }
  else
  {
    ShowCP(75,250,WHITE,BLUE,number_LP);//��ʾ���ƺ�
  }
  return Recognition_result;//����ʶ����
}

/********************************************************************
 * ������:Areap_LP
 * ����  :��������ʶ��
 * ����  :1Ϊ���ڵ�����ͷ�ɼ���ͼ��0Ϊ��ڵ�����ͷ�ɼ���ͼ��
 * ���  :��
 * ע��  :��        
 ********************************************************************/
void Areap_LP(uint8_t Doorway)
{
  const uint16_t width=320,height=240;//�ɼ�ͼ��Ĵ�С
	uint16_t i,j;//ѭ��
  uint16_t r,g,b;//rgb����ֵ
  uint16_t lr=0,num=0,max_num=0,threshold_column;//ǰһ��ĻҶȻ�ֵ���ۼƵ�������������ֵ����ֵ����ֵ
	uint16_t Camera_Data,Q_regional_datum_column=0,Regional_datum_column,Regional_datum_line;//ͼ������ֵ����24�����λ�ã���������ĵ�24����λ�ã������������λ��
  uint8_t Jump_point[240]={0},Datum_line_Data[320]={0},Data_Grayscale,sign=0;//ÿ�е��������Ŀ�������������һ����������ֵ����Ȩƽ��ֵ�����
  
  if(Doorway==1) //���ڵĳ��Ʋɼ�����
  {
    threshold_column=exit_threshold_column;
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
          exit_READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */

          r=(Camera_Data>>11);   //ȡ��rgb565 �е�rֵ
          g=(Camera_Data&0x07e0)>>6;//ȡ��rgb565 �е�gֵ����1λ
          b=Camera_Data&0x001f;//ȡ��rgb565 �е�bֵ

          Data_Grayscale=0.30*r+0.59*g+0.11*b;//��Ȩƽ��ֵ����
        
        if(sign==1)//��һ�������������һ��
          Datum_line_Data[j]=Data_Grayscale;//�洢�������Ŀ������һ�е�ÿ���Ȩƽ��ֵ
    /**********************************************************/
       if(lr>Data_Grayscale)//�ۼ��������Ŀ
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
        
        if(num==24)//������24��
          Q_regional_datum_column=j;//��¼������24�����λ��
        lr=Data_Grayscale;//��һ��ļ�Ȩƽ��ֵ
        
      }
      Jump_point[i]=num;//�洢ÿ�е��������Ŀ
      sign=0;//����Ƿ������������
      if(num>max_num)//�ж���������
      {
        Regional_datum_line=i;//��¼�����������λ��
        Regional_datum_column=Q_regional_datum_column;//��¼������������е�24�����λ��
        sign=1;//��ǳ�������������
        max_num=num;//��¼Ŀǰ�����������Ŀ
      }
      
      num=0;lr=0; 
    }
  }
  else  //��ڵĳ��Ʋɼ�����
  {
  threshold_column=entrance_threshold_column;
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        entrance_READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
                
        r=(Camera_Data>>11);   //ȡ��RGB565 �е�Rֵ
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
    
  for(i = Regional_datum_line; i < height; i++) //ʶ����������±߽�
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
  for(i = Regional_datum_line; i >0; i--) //ʶ����������ϱ߽�
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
  for(i=Regional_datum_column;i>0;i--) //ʶ�����������߽�
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
  for(i=Regional_datum_column;i<320;i++) //ʶ����������ұ߽�
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
  
  if(boundary[3]-boundary[2]>230)//�ж����ұ߽��Ƿ����
  {
    if((boundary[3]-Regional_datum_column) < (Regional_datum_column-boundary[2]))//��߽����
    {
      boundary[2]=230+Regional_datum_column*2-boundary[3];
    }
    else if((boundary[3]-Regional_datum_column) > (Regional_datum_column-boundary[2]))//�ұ߽����
    {
      boundary[3]=230+boundary[2];
    }
    else//���ұ߽綼����
    {
      boundary[3]=Regional_datum_column+100;
      boundary[2]=Regional_datum_column+100;
    }
  
  }
  
}

/********************************************************************
 * ������:Area_bin
 * ����  :���������ֵ���������ݴ洢
 * ����  :1Ϊ���ڵ�����ͷ�ɼ���ͼ��0Ϊ��ڵ�����ͷ�ɼ���ͼ��
 * ���  :��
 * ע��  :��        
 ********************************************************************/
void Area_bin(uint8_t Doorway)
{
  const uint16_t width=320,height=240;//ͼ���С
  uint16_t i,j,r,g,b,Camera_Data,num;


  num=boundary[3]-boundary[2];//��λ���ĳ�������Ŀ��
 
  if(Doorway==1) //���ڵĳ���ͼ���ֵ��
  {
   for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        exit_READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
        if(i>=boundary[0]&&i<boundary[1]&&j>=boundary[2]&&j<boundary[3])
        {
          r=(Camera_Data>>11);   //ȡ��RGB565 �е�Rֵ
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
  else //��ڵĳ���ͼ���ֵ��
  {
    for(i = 0; i < height; i++)
    {
      for(j = 0; j < width; j++)
      {
        entrance_READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
        if(i>=boundary[0]&&i<boundary[1]&&j>=boundary[2]&&j<boundary[3])
        {
          r=(Camera_Data>>11);   //ȡ��RGB565 �е�Rֵ
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
 * ������:Character_segmentation
 * ����  :�����ַ��ָ�
 * ����  :��
 * ���  :0:�ָ����
           1:�ָ�����
 * ע��  :������Ҳ�������ַ��������һ���ĺ���
 ********************************************************************/
uint8_t Character_segmentation(void)
{
	uint16_t i,j,height,width,num=0,sign=0; 
	uint16_t n,split_line_column[16]={0},split_line_level[16]={0};//��ţ������ַ���������λ�ã������ַ���������λ��

  height=boundary[1]-boundary[0];//��λ���ĳ�������ĸ߶�
  width=boundary[3]-boundary[2];//��λ���ĳ�������Ŀ��
  
   
  for(i = height/2; i < height; i++) //�����ַ�������±߽�
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
	for(i = height/2; i > 0; i--)//�����ַ�������ϱ߽�
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
  
  for(j = (width/2); j>0; j--)//���Ƶ��ַ������ұ߽�
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
  for(j = split_line_column[15]+1; j<width; j++)//���Ƶ��ַ����ұ�����ַ������ұ߽�
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
  if(num<13)//�����ַ������������²ɼ�
  {
    return 0;
  }
  /***********************************************************************/
  
  num=3;
  sign=0;
  for(j = split_line_column[14]-1; j>0; j--)//���Ƶ��ַ�����������ַ������ұ߽�
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
  if(num>0)//�����ַ������������²ɼ�
  {
    return 0;
  }
  
  if(num==0)//�����к����ַ�����߽�
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
    for(i= boundary[0];i<boundary[1];i++)//��ȷ���Ƹ��ַ����ϱ߽�
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
    for(i = boundary[1]; i>=boundary[0];i--)//��ȷ���Ƹ��ַ����±߽�
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
  
 /*   ȥ���������ĸ���ĸ�ĵط�     */ 
  {
    split_line_level[2]=split_line_level[4];
    split_line_level[3]=split_line_level[5];
    
    split_line_level[10]=split_line_level[12];
    split_line_level[11]=split_line_level[13]; 
  }
 
  for(n=0;n<14;n+=2)//���Ƹ��ַ������������һ��
  {
    Scaling_Normalization(n/2,split_line_column[n],split_line_column[n+1],split_line_level[n],split_line_level[n+1]);//�����ַ������������һ��
    number_LP[n/2]=Character_matching(n/2); //�����ַ�ƥ�䣬�洢ƥ����
  }
 
  return 1;//�����ַ�����
}
  
/********************************************************************
 * ������:Scaling_Normalization
 * ����  :�����ַ��������һ��
 * ����  :�ַ�����š��ַ�����߽硢�ַ����ұ߽硢�ַ����ϱ߽硢�ַ����±߽�
 * ���  :��
 * ע��  :��
 ********************************************************************/
void Scaling_Normalization(uint8_t n,uint16_t Left_split_line,uint16_t Right_split_line,uint16_t Upper_split_line,uint16_t Lower_split_line)
{
  uint16_t i,j,num=0,height,width; 
	uint16_t Character_seg_value[1200]={0};
  uint8_t Date_normalization,Displacement;
  
  num=boundary[3]-boundary[2];//��λ���ĳ�������Ŀ��  
  width=Right_split_line - Left_split_line;//�ַ��Ŀ��
  height=Lower_split_line - Upper_split_line;//�ַ��ĸ߶�

  for(i=0;i<50;i++)//�����ַ�����
  {
    for(j=0;j<24;j++)
    {     /* �洢�ַ����ź�Ķ�ֵ������ */
      Character_seg_value[i*24+j]=cp_xp[(i*height/49+Upper_split_line)*num + (j*width/23+Left_split_line)];     
    }
  }
  
  for(i=0;i<150;i++)//�����ַ���һ��
  {
    Date_normalization=0;
    for(j=0;j<8;j++)//ÿ8�����ص�����Ϊ1���ֽ�
    {
      Displacement=7-j;
      if(Character_seg_value[i*8+j]==1)
      {
        Date_normalization |= 1<<Displacement;             
      }
    }
    k[i]=Date_normalization;//�洢��һ���������  
  }
}

/********************************************************************
 * ������:Character_matching
 * ����  :�����ַ�ƥ��
 * ����  :�����ַ������
 * ���  :ƥ�䵽���ƶ���ߵ��ֿ��ж�Ӧ�����
 * ע��  :��
 ********************************************************************/
uint8_t Character_matching(uint8_t num)
{
  uint16_t Max_similarity=0;
  uint8_t i,j,m,Distinguish,Data_matching,Matching;
  int Similarity;
  
  switch(num)//ƥ������Ǹ�λ�õ��ַ�
  {
    case 0:      //�����ַ��ĵ�һλ��������
      for(m=0;m<3;m++)
      {
        Similarity=0;
        for(i=0;i<150;i++)//�ַ����ֿ�ıȽ�
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
        
        if(Max_similarity<Similarity)//ȡ�����Ƶ��ֿ����
        {
          Max_similarity=Similarity;
          Distinguish=m;//ƥ�䵽���ƶ���ߵ��ֿ��ж�Ӧ�����
        }
      }     
      break;
      
    case 1:      //�����ַ��ĵڶ�λ������ĸ      
      for(m=10;m<21;m++)
        {
          Similarity=0;
          for(i=0;i<150;i++)//�ַ����ֿ�ıȽ�
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
          
          if(Max_similarity<Similarity)//ȡ�����Ƶ��ֿ����
          {
            Max_similarity=Similarity;
            Distinguish=m;
          }
        }
        break;
        
    default:       //�����ַ��ĺ���λ������������ĸ
       for(m=0;m<21;m++)
        {
          Similarity=0;
          for(i=0;i<150;i++)//�ַ����ֿ�ıȽ�
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
          
          if(Max_similarity<Similarity)//ȡ�����Ƶ��ֿ����
          {
            Max_similarity=Similarity;
            Distinguish=m;
          }
        }
        break;
  }
  return Distinguish;//�����ַ����ֿ������ƶ���ߵ��ַ����
}



