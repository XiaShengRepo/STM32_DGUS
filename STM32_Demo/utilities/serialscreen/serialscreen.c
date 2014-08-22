 /**
  ******************************************************************************
  * �ļ�: serialscreen.c
  * ����: jackey
  * �汾: V1.0.0
  * ����: DWIN ������ͨ�ų���
  *  
  ******************************************************************************
	**/
#include <string.h>
#include "stm32f4xx.h"
#include "serialscreen.h"
#include "uart3.h"
#include "bsp.h"
#include "MsgHandle.h"

#define Null 0x00
typedef struct DispMeal
{
	char meal_id;
	char meal_cnt;
	char meal_cost;
}DispMealStuct;

DispMealStuct DispMeal[4]={0};

char price=0;
const char price_1st= 16;
const char price_2nd= 16;
const char price_3rd= 16;
const char price_4th= 16;
const char price_5th= 18;
const char price_6th= 18;
const char price_7th= 16;
const char price_8th= 16;

const char    mealname_1st[12]= {"�������ַ�"}; 
const char    mealname_2nd[14]= {"����±���ȷ�"};
const char    mealname_3rd[14]= {"��ɫ�����ⷹ"};
const char    mealname_4th[12]= {"�ڽ���Ƿ�"};
const char    mealname_5th[12]= {"�������㷹"};
const char    mealname_6th[14]= {"�����ﵶ�㷹"};
const char    mealname_7th[12]= {"��଼��Ƿ�"};
const char    mealname_8th[12]= {"÷�˿��ⷹ"};

uint8_t Menu_interface= 0x00;
uint8_t MealSet_interface= 0x00;
uint8_t sell_type[4]={0};//�洢�ĸ���Ʒ�ĵ�ID,��ǩ����ʱ����Ҫ��ȡ���е�ǰ��Ҫ��ʾ��������һ��������Ǽ�����Ʒ
uint8_t sell_type_1st[4]={0x01,0x02,0x03,0x04}; //��һ���۲Ͳ˵�
uint8_t sell_type_2nd[4]={0x05,0x06,0x07,0x08}; //�ڶ����۲Ͳ˵�
uint8_t sell_type_3rd[4]={0x03,0x04,0x05,0x06}; //�������۲Ͳ˵�
uint8_t sell_type_4th[4]={0x05,0x06,0x01,0x02}; //���ĸ��۲Ͳ˵�
uint8_t sell_type_5th[4]={0x05,0x06,0x01,0x02}; //������۲Ͳ˵�
/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRam
{
		int16_t	 	adress ;
		int8_t  	adr[2] ;
}myunion;

/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRamLong
{
		uint32_t	 	adress ;
		int8_t  	adr[4] ;
}myunion_long;


/*дָ���Ĵ���֡*/
const unsigned char RegisterWrite[7]={ 
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x04 , /*���� �������������*/
              0x80 , /*ָ����ַд�Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							0x00 ,0x00 /*����*/
							//0x00 , 0x00	/*CRCУ����β*/
							};

/*��ָ���Ĵ���֡*/
const unsigned char RegisterRead[6]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x03 , /*���� �������������*/
              0x81 ,  /*ָ����ַ���Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							RD_LEN , /*����*/
							//0x00 , 0x00	/*CRCУ����β*/	
							};
	
/*д�����洢��֡*/							
const unsigned char VariableWrite[8]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x00 , /*���� �������������*/	
							0x82 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							0x00 , 0x00   /*����*/
							//0x00 , 0x00  	/*CRCУ����β*/
							}; 

/*�������洢��֡*/	
const unsigned char VariableRead[7]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x04 , /*���� �������������*/
							0x83 , /*�ӱ����洢��ָ����ַ��ʼ����RD_LEN����������*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							RD_LEN , /*����*/
							//0x00 , 0x00  	/*CRCУ����β*/
							};	

const char ABC[20]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x05 , /*���� �������������*/	
							0x82 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x40 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							0xBA , 0xA0   /*����*/
              };	
const unsigned char rtc_write[13]={
  						FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x0A , /*���� �������������*/	
							0x80 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x1F , 0x5A ,	/*�����洢�ռ�2�ֽ�*/
							0x00 , 0x00 ,0X00 ,   /*����*/
              0X00 ,/*����*/
              0x00 , 0x00 ,0x00 /*ʱ��*/
              };
#include "stdio.h"

char *mystrcat(char *dest, const char *src, char length)
{
  int i=0;
	for(i=0;i<length;i++)
	{
		dest[6+i] = src[i];
	}
  return dest;
}


 /*******************************************************************************
 * ��������:VariableChage                                                                    
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void VariableChage(uint16_t Variable,uint16_t Value)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 5;
		myunion.adress= Variable; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		myunion.adress= Value; 
		temp[6]= myunion.adr[1];
		temp[7]= myunion.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}							

 /*******************************************************************************
 * ��������:VariableChagelong                                                                    
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void VariableChagelong (uint16_t Variable,uint32_t Value)							
{
	  unsigned char temp[10]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 7;
		myunion.adress= Variable; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		myunion_long.adress= Value;
		temp[6]= myunion_long.adr[3];
		temp[7]= myunion_long.adr[2];					
		temp[8]= myunion_long.adr[1];
		temp[9]= myunion_long.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:RegisterChage                                                                   
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void RegisterChage(uint8_t Register,uint16_t Value)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));
		temp[3]= Register;
		myunion.adress=Value;
		temp[5]= myunion.adr[1];
		temp[6]= myunion.adr[0];		
		Uart3_Send(temp,sizeof(temp));	
}	

 /*******************************************************************************
 * ��������:RegisterChage                                                                   
 * ��    ��:�ı�ָ��������ֵ                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void ScreenControl(char cmd)							
{
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));
		temp[3]= TPC_Enable;
		temp[5]= cmd ;	//0x00 �ر�
		Uart3_Send(temp,sizeof(temp));	
}	
 /*******************************************************************************
 * ��������:MealNameDisp                                                                  
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 17; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_name+floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,mealname_1st,12);break;	
			case 0x02:mystrcat(temp,mealname_2nd,14);break;		
			case 0x03:mystrcat(temp,mealname_3rd,14);break;	
			case 0x04:mystrcat(temp,mealname_4th,12);break;
			case 0x05:mystrcat(temp,mealname_5th,12);break;
			case 0x06:mystrcat(temp,mealname_6th,14);break;      
			case 0x07:mystrcat(temp,mealname_7th,12);break;
			case 0x08:mystrcat(temp,mealname_8th,12);break;     
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:MealCntDisp                                                                 
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_count+floor*3; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= meal_cnt;
		Uart3_Sent(temp,sizeof(temp));	
}

 /*******************************************************************************
 * ��������:MealCostDisplay                                                                
 * ��    ��:��ʾ��Ʒ����                                                          
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void MealCostDisplay(uint8_t meal_cost,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_cost+floor*3; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= meal_cost;
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:DisplayPassWord                                                                 
 * ��    ��:��ʾ���볤����*��ʾ                                                     
 *                                                                               
 * ��    ��:PassWordLen                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void DisplayPassWord(char PassWordLen)
{
	char i=0;
	char temp[20]={0};  //��Ŵ������ݵ���ʱ����
	memcpy(temp,VariableWrite,sizeof(VariableWrite));
	temp[2]= PassWordLen+ 4; //  0x83 00 00 ****** �����������ʾ���������
	myunion.adress= password_show; 
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
	for(i=0;i<PassWordLen;i++) temp[i+6]='*';
	Uart3_Sent(temp,sizeof(temp)); 
}	
 /*******************************************************************************
 * ��������:DisplayAbnormal                                                               
 * ��    ��:��ʾ������                                                   
 *                                                                               
 * ��    ��:abnomal_code �ַ���                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void DisplayAbnormal(char *abnomal_code)
{
	char i=0;
	char temp[20]={0};  //��Ŵ������ݵ���ʱ����
	memcpy(temp,VariableWrite,sizeof(VariableWrite));
	temp[2]= 7; //  ���ݳ���Ϊ4λ
	myunion.adress= erro_num; 
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
	for(i=0;i<4;i++) temp[i+6]=abnomal_code[i];
	Uart3_Sent(temp,sizeof(temp)); 	
}


 /*******************************************************************************
 * ��������:PageChange                                                                     
 * ��    ��:�����л�ͼƬ����                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	
char pageunitil= 0;
void PageChange(char page)
{
		unsigned char temp[7]={0};
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));	
		temp[4]=	PIC_ID;	
	  temp[6]=  page;
		pageunitil = page;
		Uart3_Send(temp,sizeof(temp));
}
 /*******************************************************************************
 * ��������:ReadPage                                                                     
 * ��    ��:��ȡ��ǰҳ,���ݴ�����DealSeriAceptData�д���                                                           
 *                                                                               
 * ��    ��:��                                                                    
 * ��    ��:                                                                     
 * ��    ��:                                                             
 * �޸�����:2014��6��24��                                                                    
 *******************************************************************************/ 	
void ReadPage(void)
{
		unsigned char temp[6]={0};
		memcpy(temp,RegisterRead,sizeof(RegisterRead));	
		temp[4]=	PIC_ID;	
		Uart3_Send(temp,sizeof(temp));
}

 /*******************************************************************************
 * ��������:MenuChange                                                                     
 * ��    ��:���ݷ��ص����ݸ�����ѡ�Ĳ˵�                                                         
 *                                                                               
 * ��    ��:��                                                                    
 * ��    ��:                                                                     
 * ��    ��:                                                             
 * �޸�����:2014��6��24��                                                                    
 *******************************************************************************/ 	
void MenuChange(uint8_t MenuNO)
{
  switch(MenuNO)
  {
    case 0x00:
    {
      PageChange(Logo_interface);
      while(1);
    }break;
    case 0x01:
    {
      Menu_interface= Menu1st_interface;
      MealSet_interface= MealSet1st_interface;
      memcpy(sell_type,sell_type_1st,4);
    }break;
    case 0x02:
    {
      Menu_interface= Menu2nd_interface;
      MealSet_interface= MealSet2nd_interface;  
      memcpy(sell_type,sell_type_2nd,4);      
    }break;
    case 0x03:
    {
      Menu_interface= Menu3rd_interface;
      MealSet_interface= MealSet3rd_interface;   
      memcpy(sell_type,sell_type_3rd,4);        
    }break;
    case 0x04:
    {
      Menu_interface= Menu4th_interface;
      MealSet_interface= MealSet4th_interface;  
      memcpy(sell_type,sell_type_4th,4);         
    }break;
    case 0x05:
    {
      Menu_interface= Menu5th_interface;
      MealSet_interface= MealSet5th_interface;   
      memcpy(sell_type,sell_type_5th,4);     
    }break; 
    default:break;    
  }
}
 /*******************************************************************************
 * ��������:SetScreenRtc                                                                    
 * ��    ��:��ȡ��ǰҳ,���ݴ�����DealSeriAceptData�д���                                                           
 *                                                                               
 * ��    ��:��                                                                    
 * ��    ��:                                                                     
 * ��    ��:                                                             
 * �޸�����:2014��6��24��                                                                    
 *******************************************************************************/ 	
void SetScreenRtc(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  unsigned char temp[13]={0};
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
  memcpy(temp,rtc_write,13);
  temp[6]=  RTC_DateStructure.RTC_Year;
  temp[7]=  RTC_DateStructure.RTC_Month;
  temp[8]=  RTC_DateStructure.RTC_Date;
  temp[9]=  RTC_DateStructure.RTC_WeekDay;
  temp[10]= RTC_TimeStructure.RTC_Hours;
  temp[11]= RTC_TimeStructure.RTC_Minutes;
  temp[12]= RTC_TimeStructure.RTC_Seconds;
  Uart3_Send(temp,sizeof(temp));
}
 /*******************************************************************************
 * ��������:DispLeftMeal                                                                     
 * ��    ��:�ڲ˵�ѡ�������ʾ����Ʒ��ʣ���                                                           
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	

void DispLeftMeal(void)
{
		uint8_t i;
	  unsigned char temp[8]={0};  //��Ŵ������ݵ���ʱָ��
		for(i=0;i<MealKindTotoal;i++)
    {		
      memcpy(temp,VariableWrite,sizeof(VariableWrite));
      temp[2]= 5;
      myunion.adress= meat+i; //�ڻ���ַ����λ��
      temp[4]= myunion.adr[1];
      temp[5]= myunion.adr[0];
      temp[7]= DefineMeal[i].MealCount;//sell_type[i]�洢�����ĸ���Ʒ��ID
      Uart3_Send(temp,sizeof(temp));	
		}
}
 /*******************************************************************************
 * ��������:CutDownDisp                                                                     
 * ��    ��:��ʾ����ʱ60-0                                                          
 *                                                                               
 * ��    ��:time                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	
void CutDownDisp(char time)
{
	  unsigned char temp[8]={0};
	  memcpy(temp,VariableWrite,sizeof(VariableWrite));
	  temp[2]= 5;
		myunion.adress= count_dowm; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
    temp[7]= time;	
		Uart3_Send(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:MealCostDisp                                                                     
 * ��    ��:��ʾ��ǰ��ƷӦ���Ľ��                                                          
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
unsigned char GetMealPrice(char meal_type,char count)
{
	char price= 0;
  switch(meal_type)
	{
		case 0x01:
		{
			price= price_1st*count;
		}break;
		case 0x02:
		{
      price= price_2nd*count;
		}break;
		case 0x03:
		{
      price= price_3rd*count;
		}break;
		case 0x04:
		{
      price= price_4th*count;
		}break;
		case 0x05:
		{
      price= price_5th*count;
		}break;
		case 0x06:
		{
      price= price_6th*count;
		}break;
		case 0x07:
		{
      price= price_7th*count;
		}break;
		case 0x08:
		{
      price= price_8th*count;
		}break;
    
		default:break;
	}
	return price;
}

 /*******************************************************************************
 * ��������:MealCostDisp                                                                     
 * ��    ��:��ʾ��ǰ��ƷӦ���Ľ��                                                          
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void MealCostDisp(char meal_id,char meal_count)
{
	  unsigned char temp[8]={0};
	  memcpy(temp,VariableWrite,sizeof(VariableWrite));
	  temp[2]= 5;
		temp[5]= (0x31+(meal_id-1)*2); //Ǯ�ұ�����ַ
    temp[7]= GetMealPrice(meal_id,meal_count);	
		Uart3_Send(temp,sizeof(temp));		
}
 /*******************************************************************************
 * ��������:ClearUserBuffer                                                                     
 * ��    ��:����û�ѡ������                                                          
 *                                                                               
 * ��    ��:��                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void ClearUserBuffer(void)
{
	UserActMessageWriteToFlash.UserAct.MealCnt_1st=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_1st_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_4th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_4th_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_5th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_5th_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_6th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_6th_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_7th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_7th_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_8th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_8th_t=0;   
	UserActMessageWriteToFlash.UserAct.MealCost_1st=0;
	UserActMessageWriteToFlash.UserAct.MealCost_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealCost_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealCost_4th=0;
	UserActMessageWriteToFlash.UserAct.MealCost_5th=0;
	UserActMessageWriteToFlash.UserAct.MealCost_6th=0;  
  UserActMessageWriteToFlash.UserAct.MealCost_7th=0;  
  UserActMessageWriteToFlash.UserAct.MealCost_8th=0; 
	UserActMessageWriteToFlash.UserAct.MealID=0;
	UserActMessageWriteToFlash.UserAct.Meal_totoal=0;
	UserActMessageWriteToFlash.UserAct.Meal_takeout=0;
	UserActMessageWriteToFlash.UserAct.PayShould=0;
	UserActMessageWriteToFlash.UserAct.PayType=0;
  UserActMessageWriteToFlash.UserAct.PayForCoins=0;           //�û�Ͷ���Ӳ����	
	UserActMessageWriteToFlash.UserAct.PayForBills=0;           //�û�Ͷ���ֽ����
	UserActMessageWriteToFlash.UserAct.PayForCards=0;           //�û�Ӧ��ˢ������
  UserActMessageWriteToFlash.UserAct.PayAlready=0;
  UserActMessageWriteToFlash.UserAct.MoneyBackShould=0;
  UserActMessageWriteToFlash.UserAct.MoneyBackAlready=0;
  UserActMessageWriteToFlash.UserAct.MoneyBack=0;
  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already=0;
  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st=0;
  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd=0;  
}
 /*******************************************************************************
 * ��������:PutIntoShopCart                                                                     
 * ��    ��:��ѡ�����潫������빺�ﳵ֮��ĳ�����Ӧ                                                        
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
void PutIntoShopCart(void)
{
	switch(UserActMessageWriteToFlash.UserAct.MealID)
	{
		case 0x01:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_1st= UserActMessageWriteToFlash.UserAct.MealCnt_1st_t;
			UserActMessageWriteToFlash.UserAct.MealCost_1st = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_1st);
		}break;
		case 0x02:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_2nd=UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t;
			UserActMessageWriteToFlash.UserAct.MealCost_2nd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_2nd);
		}break;
		case 0x03:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_3rd=UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t;
			UserActMessageWriteToFlash.UserAct.MealCost_3rd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_3rd);
		}break;
		case 0x04:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_4th=UserActMessageWriteToFlash.UserAct.MealCnt_4th_t;
			UserActMessageWriteToFlash.UserAct.MealCost_4th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_4th);
		}break;
		case 0x05:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_5th=UserActMessageWriteToFlash.UserAct.MealCnt_5th_t;
			UserActMessageWriteToFlash.UserAct.MealCost_5th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_5th);
		}break;
		case 0x06:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_6th=UserActMessageWriteToFlash.UserAct.MealCnt_6th_t;
			UserActMessageWriteToFlash.UserAct.MealCost_6th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_6th);
		}break;
		case 0x07:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_7th=UserActMessageWriteToFlash.UserAct.MealCnt_7th_t;
			UserActMessageWriteToFlash.UserAct.MealCost_7th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_7th);
		}break;
		case 0x08:
		{
			UserActMessageWriteToFlash.UserAct.MealCnt_8th=UserActMessageWriteToFlash.UserAct.MealCnt_8th_t;
			UserActMessageWriteToFlash.UserAct.MealCost_8th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_8th);
		}break;
		default:break;	
	}
	//PageChange((UserActMessageWriteToFlash.UserAct.MealID-1)*3+6); //��֪������Ƿ������???
	UserActMessageWriteToFlash.UserAct.Meal_totoal= UserActMessageWriteToFlash.UserAct.MealCost_8th+UserActMessageWriteToFlash.UserAct.MealCnt_7th+UserActMessageWriteToFlash.UserAct.MealCost_6th+UserActMessageWriteToFlash.UserAct.MealCnt_5th+
                                                  +UserActMessageWriteToFlash.UserAct.MealCost_4th+UserActMessageWriteToFlash.UserAct.MealCnt_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_2nd+UserActMessageWriteToFlash.UserAct.MealCnt_1st;
}	

 /*******************************************************************************
 * ��������:SettleAccounts                                                                 
 * ��    ��:�����½��㰴ť��ĳ�������                                                                                                                        
 * ��    ��:��                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 
char Floor= 0; //��Ϊ����Ʒ����Ĳ���
void SettleAccounts(void)
{
	uint8_t tempcnt=0;
	//�����Ļ��ʾ����
  for(tempcnt=0;tempcnt<4;tempcnt++)
	{
		MealNameDisp(Null,tempcnt);
		MealCntDisp(Null,tempcnt);//��ʾ����
		MealCostDisplay(Null,tempcnt);//��ʾ���ܼ� �������ظ�		
	}
  for(Floor=0;Floor<4;Floor++)
	{
		DispMeal[Floor].meal_id= 0x00; //0x01Ϊ��һ����Ʒ��ID
		DispMeal[Floor].meal_cnt= 0x00; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= 0x00; //��Ʒ���ܼ۵ĸ�ֵ
	}
	Floor= 0;
	tempcnt=0; /*��������*/
  PageChange(Acount_interface);//�������
	//����������� current = payformoney
  //��ʾӲ����
	VariableChage(payment_coin,UserActMessageWriteToFlash.UserAct.PayForCoins);
  //��ʾֽ����
	VariableChage(payment_bill,UserActMessageWriteToFlash.UserAct.PayForBills);
  //ˢ����
	VariableChage(payment_card,UserActMessageWriteToFlash.UserAct.PayForCards);
  //��ʾ�ȴ�ʱ��
	VariableChage(wait_payfor,60);
	OpenTIM7();
	//������ʾ����
	Floor= 0; //ÿ�ν�һ�ν��˽������Ҫ����д������
	
	if(UserActMessageWriteToFlash.UserAct.MealCnt_1st>0)
	{
		DispMeal[Floor].meal_id= 0x01; //0x01Ϊ��һ����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_1st; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_1st; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd>0)
	{
		DispMeal[Floor].meal_id= 0x02; //0x02Ϊ�ڶ�����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_2nd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_2nd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}		
  if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd>0)
	{
		DispMeal[Floor].meal_id= 0x03; //0x03Ϊ��������Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_3rd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_3rd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;		
	}		
	if(UserActMessageWriteToFlash.UserAct.MealCnt_4th>0)
	{
		DispMeal[Floor].meal_id= 0x04; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_4th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_4th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_5th>0)
	{
		DispMeal[Floor].meal_id= 0x05; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_5th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_5th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_6th>0)
	{
		DispMeal[Floor].meal_id= 0x06; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_6th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_6th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_7th>0)
	{
		DispMeal[Floor].meal_id= 0x07; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_7th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_7th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_8th>0)
	{
		DispMeal[Floor].meal_id= 0x08; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserActMessageWriteToFlash.UserAct.MealCnt_8th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserActMessageWriteToFlash.UserAct.MealCost_8th; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;	
	}
	//������ʾ�Ĳ��֣��ӵ�һ����ʼ��������
	for(tempcnt=0;tempcnt<Floor;tempcnt++)
	{
		MealNameDisp(DispMeal[tempcnt].meal_id,tempcnt);//��ʾ����  
		MealCntDisp(DispMeal[tempcnt].meal_cnt,tempcnt);//��ʾ����
		MealCostDisplay(DispMeal[tempcnt].meal_cost,tempcnt);//��ʾ���ܼ� �������ظ�
	}
  if(Floor>4) while(1);/*����û�ѡ��Ĳ�Ʒ����4��ֱ�ӱ���*/
	UserActMessageWriteToFlash.UserAct.PayShould= (UserActMessageWriteToFlash.UserAct.MealCost_1st+UserActMessageWriteToFlash.UserAct.MealCost_2nd+UserActMessageWriteToFlash.UserAct.MealCost_3rd+UserActMessageWriteToFlash.UserAct.MealCost_4th
                                                +UserActMessageWriteToFlash.UserAct.MealCost_5th+UserActMessageWriteToFlash.UserAct.MealCost_6th+UserActMessageWriteToFlash.UserAct.MealCost_7th+UserActMessageWriteToFlash.UserAct.MealCost_8th);
	VariableChage(mealtotoal_cost,UserActMessageWriteToFlash.UserAct.PayShould);
}

 /*******************************************************************************
 * ��������:SyncMealNameDisp                                                                  
 * ��    ��:����ͬ��ʱ��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void SyncMealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 17; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= sync_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,mealname_1st,12);break;	
			case 0x02:mystrcat(temp,mealname_2nd,14);break;		
			case 0x03:mystrcat(temp,mealname_3rd,14);break;	
			case 0x04:mystrcat(temp,mealname_4th,12);break;
			case 0x05:mystrcat(temp,mealname_5th,12);break;
			case 0x06:mystrcat(temp,mealname_6th,14);break;
			case 0x07:mystrcat(temp,mealname_7th,12);break;
			case 0x08:mystrcat(temp,mealname_8th,12);break;      
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:SyncMealCntDisp                                                                 
 * ��    ��:����ͬ��ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void SyncMealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= sync_column1st_number+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}

 /*******************************************************************************
 * ��������:AbnomalMealNameDisp                                                                  
 * ��    ��:�����¼ʱ��ʾ��Ʒ����                                                          
 *           ����ID ��ʾ���� ������floor �ı���ʵ�����ĵ�ַ                                                                    
 * ��    ��:meal_id ,floor                                                               
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 							
void AbnomalMealNameDisp(uint8_t meal_id,uint8_t floor)							
{
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 17; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= record_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,mealname_1st,12);break;	
			case 0x02:mystrcat(temp,mealname_2nd,14);break;		
			case 0x03:mystrcat(temp,mealname_3rd,14);break;	
			case 0x04:mystrcat(temp,mealname_4th,12);break;
			case 0x05:mystrcat(temp,mealname_5th,12);break;
			case 0x06:mystrcat(temp,mealname_6th,14);break;
			case 0x07:mystrcat(temp,mealname_7th,12);break;
			case 0x08:mystrcat(temp,mealname_8th,12);break;      
			default:break;			
		}
		Uart3_Sent(temp,sizeof(temp));	
}

/*******************************************************************************
 * ��������:AbnomalMealCnttDisp                                                                 
 * ��    ��:�����¼ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void AbnomalMealCnttDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= record_column1st_cnt_t+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}

/*******************************************************************************
 * ��������:AbnomalMealCntDisp                                                                 
 * ��    ��:�����¼ʱ��ʾ��Ʒ����  �����ı���ʽ��ʾ                                                        
 *           ����floor �ı���� ��meal_cnt ��Ҫ��ʵ������                                                                    
 * ��    ��:meal_id                                                                
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void AbnomalMealCntDisp(uint8_t meal_cnt,uint8_t floor)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		char buffer[5]={0};
		int cnt_t=0,i=0;
		cnt_t= sprintf(buffer,"%d",meal_cnt);
		if(meal_cnt==0xff) cnt_t=0;
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4+cnt_t; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 
		myunion.adress= record_column1st_cnt+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		for(i=0;i<cnt_t;i++) temp[i+6]=buffer[i]; 
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:DisplayTimeCutDown                                                                    
 * ��    ��:��ʾ���۲�ʱ��ĵ���ʱ                                                            
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��7��22��                                                                    
 *******************************************************************************/ 
uint32_t sellsecond_remain=0;
void DisplayTimeCutDown(void)
{
  uint32_t sellsecond_totoal=0,realsecond_totoal=0;
	RTC_TimeShow();	//��ȡ��ǰ��ʱ��
  sellsecond_totoal= selltime_hour*3600+selltime_minute*60; //�����۲͵�ʱ�任λ��
  realsecond_totoal= TimeDate.Hours*3600+TimeDate.Minutes*60+TimeDate.Senconds; //��ǰʱ��ת��Ϊ��
  if(sellsecond_totoal>realsecond_totoal)  
  {
    sellmeal_flag= false;
    sellsecond_remain = sellsecond_totoal-realsecond_totoal;
    selltime_hour_r= sellsecond_remain/3600;
    selltime_minute_r= (sellsecond_remain%3600)/60;
    selltime_second_r= (sellsecond_remain%3600)%60;
    VariableChage(wait_sellmeal_hour,selltime_hour_r);
    VariableChage(wait_sellmeal_minute,selltime_minute_r);
    VariableChage(wait_sellmeal_second,selltime_second_r);
    PageChange(SellMeal_TimeWait_interface); 
    OpenTIM4(); 
  }
  else
  {
    sellsecond_remain=0;
    CloseTIM4();
    sellmeal_flag= true;
    PageChange(Menu_interface); 
  }
}
 /*******************************************************************************
 * ��������:DisplayRecordTime                                                                     
 * ��    ��:��ʾ����ʱ�Ĵ����¼                                                              
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��23��                                                                    
 *******************************************************************************/  
char record_time[20]={"20yy-mm-dd hh:mm:ss"};
void DisplayRecordTime(void)
{ 
	  char temp[30]={0};  //��Ŵ������ݵ���ʱ����  
		RTC_TimeShow();     /*ʱ����Ϊ�����¼ʱ��*/
		record_time[0] =    20/10+'0'; 
		record_time[1] =    20%10 +'0'; 
		record_time[2] =    TimeDate.Year/10+ '0'; 
		record_time[3] =    TimeDate.Year%10+ '0' ;
		record_time[5] =    TimeDate.Month/10+ '0';
		record_time[6] =    TimeDate.Month%10+ '0';
		record_time[8] =    TimeDate.Date/10+ '0'; 
		record_time[9] =    TimeDate.Date%10+ '0';
		record_time[11] =   TimeDate.Hours/10+ '0';
		record_time[12] =   TimeDate.Hours%10+ '0';
		record_time[14] =   TimeDate.Minutes/10+ '0';
		record_time[15] =   TimeDate.Minutes%10+ '0' ;
		record_time[17] =   TimeDate.Senconds/10+ '0'; 
		record_time[18] =   TimeDate.Senconds%10+ '0';	
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 23; //22 0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= record_UserDataTime;
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		mystrcat(temp,record_time,20);
		Uart3_Sent(temp,sizeof(temp));	
}
 /*******************************************************************************
 * ��������:DisplayUserRecord                                                                     
 * ��    ��:��ʾ����ʱ�Ĵ����¼                                                              
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/  
void DisplayUserRecord(void)
{
	uint8_t cnt_t=0,floor=0;
	for(floor=0;floor<4;floor++)		//�����Ļ����
	{
		AbnomalMealNameDisp(0,floor);
		AbnomalMealCnttDisp(0xff,floor);
		AbnomalMealCntDisp(0xff,floor);		
	}
	floor=0; //��ԭ
	if(UserActMessageWriteToFlash.UserAct.MealCnt_1st_t>0)  
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_1st_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_1st,floor);		
		floor++;
	}
	cnt_t++;
	if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_2nd,floor);	
		floor++;
	}
	cnt_t++;
	if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_3rd,floor);	
		floor++;
	}
	cnt_t++;
	if(UserActMessageWriteToFlash.UserAct.MealCnt_4th_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_4th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_4th,floor);	
		floor++;
	}
	cnt_t++;
	if(UserActMessageWriteToFlash.UserAct.MealCnt_5th_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_5th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_5th,floor);	
		floor++;
	}
	cnt_t++;
	if(UserActMessageWriteToFlash.UserAct.MealCnt_6th_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_6th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_6th,floor);	
		floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_7th_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_7th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_7th,floor);	
		floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_8th_t>0)
	{
		AbnomalMealNameDisp(sell_type[cnt_t],floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_8th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_8th,floor);	
		floor++;
	}
  cnt_t=0;  
		//��ʾ�û��Ѹ� ��  Ӧ��  �� ����
	VariableChage(record_UserActPayAlready,UserActMessageWriteToFlash.UserAct.PayAlready);
		//Ӧ�˵�Ǯ = �ܵ�Ӧ���˱ҵ�Ǯ
	VariableChage(record_UserActPayBack,UserActMessageWriteToFlash.UserAct.MoneyBackShould);
	  //���˵�Ǯ = �ܵ�Ӧ���˱ҵ�Ǯ- ��δ�˵�Ǯ
	VariableChage(record_UserActPayBackAlready,UserActMessageWriteToFlash.UserAct.MoneyBackShould-UserActMessageWriteToFlash.UserAct.MoneyBack);
}
	
 /*******************************************************************************
 * ��������:GetPassWord                                                                     
 * ��    ��:�Ų�����                                                            
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                    
 *******************************************************************************/  
void GetPassWord(unsigned char *PassWord)
{
  PassWord[0] = 5;
	PassWord[1] = 1;
	PassWord[2] = 8;
	PassWord[3] = 5;
	PassWord[4] = 1;
	PassWord[5] = 8;
}
 /*******************************************************************************
 * ��������:GetAdminPassWord                                                                     
 * ��    ��:Ӳ���˱���װ������                                                           
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��8��13��                                                                    
 *******************************************************************************/  
void GetAdminPassWord(unsigned char *PassWord)
{
  PassWord[0] = 1;
	PassWord[1] = 2;
	PassWord[2] = 3;
	PassWord[3] = 4;
	PassWord[4] = 5;
	PassWord[5] = 6;
}
 /*******************************************************************************
 * ��������:GetUserPassword                                                                     
 * ��    ��:�����¼�������                                                         
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��8��13��                                                                    
 *******************************************************************************/  
void GetUserPassWord(unsigned char *PassWord)
{
  PassWord[0] = 6;
	PassWord[1] = 6;
	PassWord[2] = 6;
	PassWord[3] = 6;
	PassWord[4] = 6;
	PassWord[5] = 6;  
}
 /*******************************************************************************
 * ��������:VerifyPassword                                                                   
 * ��    ��:����Ƚ�                                                                 
 *                                                                               
 * ��    ��:����ָ��1������ָ��2����Ƚϵĳ���                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                     
 *******************************************************************************/  
uint8_t VerifyPassword( uint8_t *Src1, uint8_t *Src2, uint8_t Length)
{
  uint8_t i = 0; 
	for(i = 0; i < Length; i++)
	{
	  if(*(Src1+i) != *(Src2+i))
		{
		    return 0;
		}
	}
	return 1;
}

 /*******************************************************************************
 * ��������:RecRegisterValues                                                                   
 * ��    ��:��ȡ�Ĵ�����ֵ                                                                                                                          
 * ��    ��:�Ĵ�����ַ,�Ĵ�������,�Ĵ������ݳ���                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��6��24��                                                                   
 *******************************************************************************/ 
char current_page =0;//��ǰҳ��
void RecRegisterValues(char VariableAdress,char *VariableData,char length)
{
	if(VariableAdress==PIC_ID)//��ȡ�жϵ�ǰ��ҳ���ID
	{
		current_page =VariableData[length-1];
		if(current_page!=pageunitil)
		{
			//PageChange(cmd_page);
    }
  }
}

 /*******************************************************************************
 * ��������:ChangeVariableValues                                                                   
 * ��    ��:�ı����ݱ�����ֵ                                                                                                                          
 * ��    ��:������ַ,��������,��������                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 
char meal1st_cnt_t= 0,chicken_cnt_t= 0,duck_cnt_t= 0,fish_cnt_t= 0; /*��ʱ����*/
uint8_t  PassWordLen=0;	//����ĳ���Ϊ0
uint8_t  PassWord[6]={0};
uint8_t  InputPassWord[6]={0};
bool cardbalence_cancel_flag= false;
bool mealneed_sync = false;  //��Ʒͬ�����
int16_t CoinTotoal_t=0; //��Ϊ���� 		
int8_t	selltime_hour= 11,selltime_hour_t=11,selltime_hour_r=0;
int8_t	selltime_minute=30, selltime_minute_t=30,selltime_minute_r=0;
int8_t  selltime_second_r=0;
bool sellmeal_flag = true;
void ChangeVariableValues(int16_t VariableAdress,char *VariableData,char length)
{
	char MealID =0;
	uint8_t i= 0,j=0;
	  switch (VariableAdress)
		{
			case meal_choose: /*�������û�ѡ��*/
			{
				switch(VariableData[1])
				{
					case 0x01:
						if(DefineMeal[0].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1]; //��ǰ�û�ѡ�͵�ID
							UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= 1;//����Ĭ�Ϸ���Ϊ 1							
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();
							PageChange(Meal1st_interface);//��ʾ��Ӧ����
              VariableChage(meal1st_cnt,0x01);
 							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_1st_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x02:
						if(DefineMeal[1].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();							
							PageChange(Meal2nd_interface);//��ʾ��Ӧ����	
							VariableChage(meal2nd_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;							
					case 0x03:
						if(DefineMeal[2].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();							
							PageChange(Meal3rd_interface);//��ʾ��Ӧ����	
							VariableChage(meal3rd_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x04:
					{					
						if(DefineMeal[3].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);							
							OpenTIM3();	
						  VariableChage(count_dowm,WaitTime);
							PageChange(Meal4th_interface);//��ʾ��Ӧ����	
							VariableChage(meal4th_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_4th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;
					case 0x05:
						if(DefineMeal[4].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_5th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);
							VariableChage(count_dowm,WaitTime);
							OpenTIM3();							
							PageChange(Meal5th_interface);//��ʾ��Ӧ����	
							VariableChage(meal5th_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_5th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x06:
					{					
						if(DefineMeal[5].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_6th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);							
							OpenTIM3();	
						  VariableChage(count_dowm,WaitTime);
							PageChange(Meal6th_interface);//��ʾ��Ӧ����	
							VariableChage(meal6th_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_6th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;   
					case 0x07:
					{					
						if(DefineMeal[6].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_7th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);							
							OpenTIM3();	
						  VariableChage(count_dowm,WaitTime);
							PageChange(Meal7th_interface);//��ʾ��Ӧ����	
							VariableChage(meal7th_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_7th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;  
					case 0x08:
					{					
						if(DefineMeal[7].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							PlayMusic(VOICE_1);
							UserActMessageWriteToFlash.UserAct.MealID= VariableData[1];
							UserActMessageWriteToFlash.UserAct.MealCnt_8th_t= 1;//����Ĭ�Ϸ���Ϊ 1
							WaitTimeInit(&WaitTime);							
							OpenTIM3();	
						  VariableChage(count_dowm,WaitTime);
							PageChange(Meal8th_interface);//��ʾ��Ӧ����	
							VariableChage(meal8th_cnt,0x01);
							MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_8th_t);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;            
					case 0x0F:  /*�����û���*/
					{
						PageChange(Password_interface);
					}break;
					default:break;
				}
			}break;      				
			case meal1st_cnt: /*���ܲ��������*/
			{
				if(VariableData[1]<= DefineMeal[0].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_1st = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_1st_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_1st_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= DefineMeal[0].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_1st_t);
					VariableChage(meal1st_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_1st_t);	//�ı������ַ����
				}	 						 
			}break;
			case meal2nd_cnt:/*�㹽��������*/
			{
				if(VariableData[1]<= DefineMeal[1].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_2nd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= DefineMeal[1].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t);
					VariableChage(meal2nd_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t);	//�ı������ַ����
				}	 						 
			}break;
			case meal3rd_cnt:/*��Ƥ��Ѽ*/
			{
				if(VariableData[1]<= DefineMeal[2].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_3rd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= DefineMeal[2].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t);
					VariableChage(meal3rd_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t);	//�ı������ַ����			
				}	 
			}break;	
			case meal4th_cnt:/*�������*/
			{
				if(VariableData[1]<= DefineMeal[3].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_4th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_4th_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_4th_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= DefineMeal[3].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_4th_t);
					VariableChage(meal4th_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_4th_t);	//�ı������ַ����
				}
			}break;	
 			case meal5th_cnt:/*÷�˿���*/
			{
				if(VariableData[1]<= DefineMeal[4].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_5th_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_5th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_5th_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_5th_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_5th_t= DefineMeal[4].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_5th_t);
					VariableChage(meal5th_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_5th_t);	//�ı������ַ����
				}
			}break;	 
 			case meal6th_cnt:/*����ţ��*/
			{
				if(VariableData[1]<= DefineMeal[5].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_6th_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_6th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_6th_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_6th_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_6th_t= DefineMeal[5].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_6th_t);
					VariableChage(meal6th_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_6th_t);	//�ı������ַ����
				}
			}break;	   
 			case meal7th_cnt:/*����ţ��*/
			{
				if(VariableData[1]<= DefineMeal[6].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_7th_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_7th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_7th_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_7th_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_7th_t= DefineMeal[6].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_7th_t);
					VariableChage(meal7th_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_7th_t);	//�ı������ַ����
				}
			}break;	 
 			case meal8th_cnt:/*����ţ��*/
			{
				if(VariableData[1]<= DefineMeal[7].MealCount)	//���ò�Ʒѡ�������
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_8th_t= VariableData[1];
					UserActMessageWriteToFlash.UserAct.MealCost_8th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_8th_t);//��һЩ�ظ�����
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_8th_t);
				}
				else
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_8th_t= DefineMeal[7].MealCount;
					MealCostDisp(UserActMessageWriteToFlash.UserAct.MealID,UserActMessageWriteToFlash.UserAct.MealCnt_8th_t);
					VariableChage(meal8th_cnt,UserActMessageWriteToFlash.UserAct.MealCnt_8th_t);	//�ı������ַ����
				}
			}break;	       
      case mealcnt_choose:/*��ҳѡ��ť*/
      {
			  switch(VariableData[1])
				{
					case 0x01:  /*���빺�ﳵ*/
					{
								
					}break;
					case 0x02:  /*����ѡ��*/
					{
            PutIntoShopCart();
						PageChange(Menu_interface);
						WaitTimeInit(&WaitTime);
					}break;
 					case 0x03:  /*���빺�ﳵ*/
					{
            if(sellmeal_flag)
            {
              PutIntoShopCart();
              SettleAccounts();
              if(UserActMessageWriteToFlash.UserAct.Meal_totoal>0)
              {  
                CloseTIM3();
                WaitTimeInit(&WaitTime);
                OpenTIM7();
                Current= waitfor_money;//�����Ǯ����
                CurrentPoint= 1;       //����ʱ60s�����ֽ����
                PlayMusic(VOICE_2);
              }
            }
            else
            {
              PageChange(SellMeal_TimeWait_interface);
            }
					}break;
					case 0x04:  /*ȡ��*/
					{
						//������е��û�����???
						ClearUserBuffer();
            SaveUserData();
						PageChange(Menu_interface);
						CloseTIM3();
						Current= current_temperature;//ȡ��������г���
						
					}break;
					default:break;
				}
			}break;
			/*�ı������в�Ʒ������*/
			/*��Ҫ��Ʒ��ID���к�:column1st_count, MealID*/
			case column1st_count:MealID= DispMeal[0].meal_id;goto loop1;
			case column2nd_count:MealID= DispMeal[1].meal_id;goto loop1;
			case column3rd_count:MealID= DispMeal[2].meal_id;goto loop1;
			case column4th_count:MealID= DispMeal[3].meal_id;goto loop1;
			{
loop1:	switch(MealID)
				{
					case 0x00:
					{
						VariableChage(VariableAdress,0); //ά��ԭ����ѡ������
					}break;
					case 0x01:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//���û�ѡ������С�ڻ���ڴ��ʱ
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_1st =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_1st =UserActMessageWriteToFlash.UserAct.MealCnt_1st *price_1st; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_1st); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_1st); //�ı䵥�ܼۣ���ַƫ��1
				    }
				    else
				    {	
							VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_1st); //ά��ԭ����ѡ������
				    }					
					}break;
					case 0x02:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_2nd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_2nd =UserActMessageWriteToFlash.UserAct.MealCnt_2nd *price_2nd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_2nd); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_2nd); //�ı䵥�ܼ�
				    }
				    else
				    {
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_2nd); //ά��ԭ����ѡ������
				    }									
					}break;
					case 0x03:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_3rd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_3rd =UserActMessageWriteToFlash.UserAct.MealCnt_3rd *price_3rd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_3rd); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_3rd); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_3rd); //ά��ԭ����ѡ������
				    }							
					}break;
					case 0x04:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_4th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_4th =UserActMessageWriteToFlash.UserAct.MealCnt_4th *price_4th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_4th); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_4th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_4th); //ά��ԭ����ѡ������
				    }							
					}break;
					case 0x05:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_5th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_5th =UserActMessageWriteToFlash.UserAct.MealCnt_5th *price_5th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_5th); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_5th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_5th); //ά��ԭ����ѡ������
				    }							
					}break;
					case 0x06:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_6th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_6th =UserActMessageWriteToFlash.UserAct.MealCnt_6th *price_6th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_6th); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_6th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_6th); //ά��ԭ����ѡ������
				    }							
					}break; 
					case 0x07:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_7th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_7th =UserActMessageWriteToFlash.UserAct.MealCnt_7th *price_7th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_7th); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_7th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_7th); //ά��ԭ����ѡ������
				    }							
					}break;  
					case 0x08:
					{
					  if(VariableData[1]<=DefineMeal[MealID-1].MealCount)//����Ҫ�ж�����
				    {
					    UserActMessageWriteToFlash.UserAct.MealCnt_8th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserActMessageWriteToFlash.UserAct.MealCost_8th =UserActMessageWriteToFlash.UserAct.MealCnt_8th *price_8th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_8th); //�ı�����
					    VariableChage(VariableAdress+1,UserActMessageWriteToFlash.UserAct.MealCost_8th); //�ı䵥�ܼ�
				    }
				    else
				    {
					     VariableChage(VariableAdress,UserActMessageWriteToFlash.UserAct.MealCnt_8th); //ά��ԭ����ѡ������
				    }							
					}break;            
					default:break;						
				}
				UserActMessageWriteToFlash.UserAct.PayShould= (UserActMessageWriteToFlash.UserAct.MealCost_1st+UserActMessageWriteToFlash.UserAct.MealCost_2nd+UserActMessageWriteToFlash.UserAct.MealCost_3rd+UserActMessageWriteToFlash.UserAct.MealCost_4th
                                                      +UserActMessageWriteToFlash.UserAct.MealCost_5th+UserActMessageWriteToFlash.UserAct.MealCost_6th+UserActMessageWriteToFlash.UserAct.MealCost_7th+UserActMessageWriteToFlash.UserAct.MealCost_8th);
	      VariableChage(mealtotoal_cost,UserActMessageWriteToFlash.UserAct.PayShould);
			}break;
			case payment_method: /*���ʽ*/ 
			{
        uint32_t temp1= 0,temp2= 0;
				AcountCopy();
				if(UserActMessageWriteToFlash.UserAct.PayShould==0) goto loop7;
				PageChange(Acount_interface+2); //�����¸������ת����һ��ҳ���ֹ�����Ӽ�
				//MoveToFisrtMeal();  //��ѡ�񸶿ʽ����Բ�ѯ��ǰ���û�ѡ�͵�ID,ֱ�ӷ�������,��Ҫ����������Ȼ����
				switch(VariableData[1])
				{
					case 0x01:   /*�ֽ�֧��*/
					{
            if(cash_limit_flag==true) break; //�����Ǯ���꣬�޷�����
						CurrentPoint =2;
            PlayMusic(VOICE_3);
						if(!OpenCashSystem()){OpenCashSystem();};// printf("cash system is erro2");  //�ر��ֽ����
					}break;
					case 0x02:   /*����Ԥ����*/
					{
						CurrentPoint =7;
						PlayMusic(VOICE_5);
						if(!CloseCashSystem()){CloseCashSystem();};// printf("cash system is erro3");  //�ر��ֽ����
					}break;
					case 0x03:   /*����֧ͨ��*/
					{
						CurrentPoint =8;
						PlayMusic(VOICE_5);
						if(!CloseCashSystem()){CloseCashSystem();};//printf("cash system is erro4");  //�ر��ֽ����			
					}break;
					case 0x04:   /*ȡ��*/
					{
	 loop7:		temp1= UserActMessageWriteToFlash.UserAct.MoneyBack= UserActMessageWriteToFlash.UserAct.PayAlready; //��ʱ���յ���Ǯ��Ӳ�ҵ���ʽ����
	          temp2= UserActMessageWriteToFlash.UserAct.MoneyBackShould = UserActMessageWriteToFlash.UserAct.PayAlready; //�ܵ�Ӧ���˵�Ǯ
            ClearUserBuffer();
            UserActMessageWriteToFlash.UserAct.MoneyBack= temp1;
            UserActMessageWriteToFlash.UserAct.MoneyBackShould= temp2;
            SaveUserData();
			      if(!CloseCashSystem()){CloseCashSystem();};//printf("cash system is erro5");  //�ر��ֽ����
            PageChange(Menu_interface);
						CloseTIM3();
						CloseTIM7();
						CurrentPoint = 0 ;
            if(UserActMessageWriteToFlash.UserAct.MoneyBack>0) //����Ǯ��Ҫ�˵�ʱ�򣬱���û�ȡ�����������˱ҵ�ʱ��ֱ�ӽ����¶���ʾ״̬
            {
              UserActMessageWriteToFlash.UserAct.Cancle= 0x01;
						  Current= hpper_out; 
            }
            else
            {              
              Current= current_temperature; 
            }
					}break;
					default:break;		
				}					
			}break;
			case caedbalence_cancel:/*ˢ��ȡ��*/
			{
				PageChange(Acount_interface+2);
				cardbalence_cancel_flag=true;
			}break;			
			case bill_print:
			{
				switch(VariableData[1])
				{
					case 0x01:   /*��ӡСƱ*/
          {
            UserActMessageWriteToFlash.UserAct.PrintTick= 0x00000001;
            PrintTickFun(&UserActMessageWriteToFlash.UserAct.PrintTick);
            CloseTIM4();
            if(!erro_record) //���д����ʱ�򲻽�����ͽ���						
						PageChange(Mealout_interface);            
          }break;
					case 0x02:   /*����ӡСƱ*/
					{
						UserActMessageWriteToFlash.UserAct.PrintTick= 0x00000002;
							 /*�ж��Ƿ��ӡСƱ*/ 			
            PrintTickFun(&UserActMessageWriteToFlash.UserAct.PrintTick);
            CloseTIM4();
            if(!erro_record) //���д����ʱ�򲻽�����ͽ���						
						PageChange(Mealout_interface);
					}break;  
					default:break;
				}
			}break;
			case password:
			{
				switch(VariableData[1])
				{
					case 0x00:
					case 0x01:
					case 0x02:
					case 0x03:
					case 0x04:
					case 0x05:
					case 0x06:
					case 0x07:
					case 0x08:
					case 0x09:
					case 0x0A:						
					case 0x0B:
					case 0x0C:
					case 0x0D:
					case 0x0E:
					case 0x0F:
					{
	          if(PassWordLen < 6)
		        {
		          InputPassWord[PassWordLen] = (VariableData[1]);
		          PassWordLen++;
			        DisplayPassWord(PassWordLen);//��ʾһ���ַ���******
		        }
	        }break;					
					case 0x10:/*�˸��*/
					{
	          if(PassWordLen > 0)
		        {
	            InputPassWord[PassWordLen] = 0;
	            PassWordLen--;
		          DisplayPassWord(PassWordLen);
		        }
					}break;
					case 0x11:/*ȷ�ϼ�*/ //��Ҫ���볬������
					{
	          GetPassWord(PassWord);
    	      if(VerifyPassword(PassWord, InputPassWord,6) == 1) //�Ų�������ȷ
		        {
							PassWordLen = 0;		
							DisplayPassWord(0);//���������ʾ
							memset(InputPassWord,0,6);
						  if(Current== 0x00) 
						  {
							  break;
						  }              
						  if(erro_record!=0) 
						  {
							  break;
						  }							
							//����д�������break;
		           /*�����Ʒ���ý���*/
		           PageChange(MealSet_interface);					 
							 InitSetting();//��յ�һ�����е����� //�ԷŲ͵����ݽ��г�ʼ��
			         break;
		        }
						GetAdminPassWord(PassWord);//Ӳ����������
    	      if(VerifyPassword(PassWord, InputPassWord,6) == 1) 
		        {
							//�������Ա�������ã�����Ӳ�����ý���
		           /*�������Ա����*/
							memset(InputPassWord,0,6);	
              DisplayPassWord(0);//���������ʾ
              PassWordLen = 0;
						  if(Current== 0x00) 
						  {
							  break;
						  }
						  if(erro_record!=0) 
						  {
							  break;
						  }              
               VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ������
               VariableChage(coins_input,0);//��ʾ�ű���
               VariableChage(coins_retain,coins_remain);//����Ӳ����
               VariableChage(coins_back,0); //��ʾ�˱���
               PageChange(Coinset_interface);
               //��ǰ�˱���
               break;
		        }
            GetUserPassWord(PassWord);   //����������ʾ����
            if(VerifyPassword(PassWord, InputPassWord,6) == 1) 
            {
               memset(InputPassWord,0,6);
               DisplayPassWord(0);//���������ʾ
               PassWordLen = 0;	
               if(erro_record!=0)
               {
                 if(!((erro_record&1<<arm_limit)||(erro_record&1<<signin_erro)||erro_record&1<<network_erro))
                   DisplayUserRecord();
                 PageChange(UserAbonamalRecord_interface);
               }                 
               break;              
            }
		        else
		        {
		          /*������֤���󣬽��������������*/   
							 DisplayPassWord(0);	
               PageChange(Password_interface);						
			         PassWordLen = 0;
		        }						
	        }break;						
					case 0x12:/*����*/
					{
						if(erro_record!=0) //�д����ʱ��ֻ�ܷ��ص��������
						{
						  PageChange(Err_interface);
							break;
						}								
		        PageChange(Menu_interface);
						DisplayPassWord(0);//���������ʾ
		        PassWordLen = 0;	
					}break;
          default:break;					
        } 					
			}break;
			case record_clear:
			{
				switch(VariableData[1])
			  {
					case 0x01: //�������
					{
						erro_flag=0; //������ݱ��,���������AbnormalHandle������
						if(Current==current_temperature)//�޶��ڳ�ʼ��״̬
						{
							UserActMessageWriteToFlash.UserAct.MoneyBack=0;
							ClearUserBuffer();
					    SaveUserData();
							PageChange(Logo_interface);
						}
   				}break;
					case 0x02: //����
					{
						PageChange(Err_interface);
					}break;
          default:break;
				}	
			}break;
			case admin_set:
			{
				switch(VariableData[1])
				{ 
					case 0x01:  /*�¶����ð�ť*/
					{
						PageChange(TemperatureSet_interface);
					}break;
					case 0x02:  /*��Ʒ���ð�ť Ӧ�ø�Ϊһ������������� */
					{
							for(i=0;i<90;i++)
	            {
	              FloorMealMessageWriteToFlash.FlashBuffer[i] = 0 ;
	            }
	            WriteMeal();  //д���Ʒ����
							StatisticsTotal(); 
							DispLeftMeal();
					}break;
					case 0x03:	/*ȡ����*/
					{
						if(mealneed_sync == false)
						{
						  PageChange(Menu_interface);
						}
					}
          default:break;
				}					
			}break;
			case meal_num: //���ı��Ʒ�ı�ŵ�ʱ����Ҫ���ҵ�ǰ��ID
			{
				uint8_t cnt_t=0;
        uint8_t meal_id_t=0,find_flag=0;
        for(meal_id_t=0;meal_id_t<4;meal_id_t++)
        {
          if(sell_type[meal_id_t]==VariableData[1])
          {
            find_flag=1;
            break;
          }
        }
        if(find_flag==1)
			    CurFloor.MealID= VariableData[1];	//��ǰ�ľ��ǲ�Ʒ��ID
        else
        {
          CurFloor.MealID= sell_type[0]; //���鲻�������ID�뱾�ص�id��ƥ����ѡ��С�ŵ�ID
        }
        VariableChage(meal_num,CurFloor.MealID);		
				InitSetting();
				for(cnt_t = 0; cnt_t < FloorMealNum; cnt_t++)  //������һ�����ǿյ�
				{
					if(FloorMealMessageWriteToFlash.FloorMeal[cnt_t].MealCount ==0)
					{
						 CurFloor.FloorNum = cnt_t+1;
						 break;
					}			
				}
				VariableChage(floor_num,CurFloor.FloorNum);				
			}break;
      case floor_num:
			{
			  CurFloor.FloorNum= VariableData[1];
				InitSetting();
			}break;
      case row_1st:
      {
				CurFloor.FCount= VariableData[1];
			}break;	
      case row_2nd:
      {
				CurFloor.SCount= VariableData[1];
			}break;	
      case row_3rd:
      {
				CurFloor.TCount= VariableData[1];
			}break;	
      case mealinput_choose:
			{
				switch(VariableData[1])
				{
					case 0x01:  /*����*/
					{
						CurFloor.FCount = 3;
		        CurFloor.SCount = 3;
		        CurFloor.TCount = 3; 
		        CurFloor.MealCount = 9;
					  VariableChage(row_1st,CurFloor.FCount);
		        VariableChage(row_2nd,CurFloor.SCount);
		        VariableChage(row_3rd,CurFloor.TCount);	
					}break;
					case 0x02:  /*���*/
					{
						CurFloor.FCount = 0;
		        CurFloor.SCount = 0;
		        CurFloor.TCount = 0; 
		        CurFloor.MealCount = 0;
					  VariableChage(row_1st,CurFloor.FCount);
		        VariableChage(row_2nd,CurFloor.SCount);
		        VariableChage(row_3rd,CurFloor.TCount);							
					}break;
					case 0x03:  /*ȷ��*/
					{
						/*�����ܹ��Ĳ�Ʒ��*/
						CurFloor.MealCount=  CurFloor.FCount + CurFloor.SCount + CurFloor.TCount;
						/*�ѵ�ǰ��һ�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FCount    = CurFloor.FCount;
						/*�ѵ�ǰ��2�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].SCount    = CurFloor.SCount;
						/*�ѵ�ǰ��3�����õĲ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].TCount    = CurFloor.TCount;
						/*�ѵ�ǰ���ò͵�ID�ű�������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealID    = CurFloor.MealID;
						/*�ѵ�ǰ���õĲ͵�������������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealCount = CurFloor.MealCount;
						/*�����ò͵Ĳ�����������*/
						FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FloorNum  = CurFloor.FloorNum;
						/*���ֲ�Ʒ*/
						for(j = 0; j < MealKindTotoal; j++)	 //ͳ��ÿ����Ʒ������
						{
							/*�����͵���Ŀ����*/
							DefineMeal[j].MealCount = 0;
							/*�����͵��ж�Ӧ����Ŀ����*/
							for(i = 0; i < FloorMealNum; i++)
							{
								DefineMeal[j].Position[i][0] = 0;
								DefineMeal[j].Position[i][1] = 0;
								DefineMeal[j].Position[i][2] = 0;
							}
							 /*ͳ�Ƹ����͵���Ŀ���ܺ�*/
							for(i = 0; i < FloorMealNum; i++)
							{														 /*�͵�ID�Ǵ�1-6*/
								if(FloorMealMessageWriteToFlash.FloorMeal[i].MealID == j + 1)
								{
									DefineMeal[j].MealCount      = DefineMeal[j].MealCount + FloorMealMessageWriteToFlash.FloorMeal[i].MealCount;
									DefineMeal[j].Position[i][0] = FloorMealMessageWriteToFlash.FloorMeal[i].FCount;
									DefineMeal[j].Position[i][1] = FloorMealMessageWriteToFlash.FloorMeal[i].SCount;
									DefineMeal[j].Position[i][2] = FloorMealMessageWriteToFlash.FloorMeal[i].TCount;
									//printf("DefineMeal[%d].Position[%d][0]=%d\r\n",j,i,DefineMeal[j].Position[i][0]);
								}
							}
						}
						StatisticsTotal(); //��ͳ����Ŀ��д��
						WriteMeal(); //д���ݵ�flash
						DispLeftMeal(); //���²�Ʒ����
						mealneed_sync = true;
						//printf("DefineMeal[%d].MealCount=%d\r\n",i,DefineMeal[i].MealCount);
					}break;
					case 0x04:  /*����ͬ��*/
					{
						uint8_t cnt_t=0,floor=0;
						for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
						{
							/*��ʾ��Ʒ,����*/									
              SyncMealNameDisp(0,cnt_t);
              SyncMealCntDisp(0xFF,cnt_t); 									 
						}	
						//��ֹ��Ļ���*/
            ScreenControl(ScreenDisable);
						/*��Ʒͬ��ʱ����������������Ͽ������*/
						/*���ܼ���ǩ��������ֱ������*/
						//if(!SignInFunction())  AbnormalHandle(signin_erro);
						/*�ϴ�����������ݣ�����̨ʵʱ��������*/
            // SendtoServce(); //��Ҫ����Ϊû���ã� 
						//����ͬ���ӳ���
						MealDataCompareFun();
						if(MealCompareData.MealCompareTotoal==0xFFFFFFFF) //��ȷ
						 {
							 mealneed_sync = false;
							 PageChange(Data_synchronization);
							 for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
							 {
									 /*��ʾ��Ʒ,����*/									
                  SyncMealNameDisp(sell_type[cnt_t],floor);
                  SyncMealCntDisp(DefineMeal[sell_type[cnt_t]-1].MealCount,floor); 
                  floor++;									 
							 }							 
						 }
						 /*���Լ���Աȵķ���ֵ�����ж�*/
             else
             {
							 mealneed_sync = true;
							 PageChange(Data_synchronization+2);
							 for(cnt_t=0x00;cnt_t<0x04;cnt_t++)
							 {
								 if(MealCompareData.MealComparePart[cnt_t]==0xff)
								 {
									//��Ʒͬ���ɹ������κ����� 
								 }
								 else
								 {
									 /*��ʾ��Ʒ,����*/									
                  SyncMealNameDisp(sell_type[cnt_t],floor);
                  SyncMealCntDisp(MealCompareData.MealComparePart[cnt_t],floor); 
                  floor++;									 
								 }
               }						 
						 }
						//��ʱʱ�˳��������˳�
						ScreenControl(ScreenEnable);
						DispLeftMeal();//�ı�ʣ���Ʒ������ʾ
					}break;
					case 0x05:  /*����*/
					{
						CurFloor.FCount    = 0;
						CurFloor.SCount    = 0;
						CurFloor.TCount    = 0;
						CurFloor.MealID    = 0;
						CurFloor.MealCount = 0;
						CurFloor.FloorNum  = 0;	 
	          PageChange(MealSet_interface);
					}break;
					case 0x06: /*�۲�����*/
					{
            VariableChage(set_sellmeal_hour,selltime_hour);
            VariableChage(set_sellmeal_minute,selltime_minute);  
						PageChange(SellMeal_TimeSet_interface); 
					}break;
          case 0x07: /*��Ʒ�Աȳɹ�����*/
          {
            PageChange(MealInput_interface);
          }break;
					default:break;	
				}
			}break;
			case temprature_set:
			{
        if(VariableData[1]==0xFF)
        {
          PageChange(MealSet_interface);
          VariableChage(temprature_set,0);
        }
        else
        {
          SetTemper(VariableData[1]);
        }
				
			}break;
			case coins_key:  //��һ����һ��
      {
				int16_t cnt_t=0,i=0;
			  int16_t coins_time=0;
				int16_t MoneyBack_cnt=0;
				/*����������һ��������ʱ����Ҫ��Ӳ�����ݽ��д���*/
				CoinsTotoalMessageWriteToFlash.CoinTotoal+= CoinTotoal_t;
				CoinTotoal_t=0;
				WriteCoins();
				if(VariableData[1]==0x01) /*�����˱�*/
				{
					if(CoinsTotoalMessageWriteToFlash.CoinTotoal-coins_remain>0)
					{
						Coins_cnt=0;
						coins_time= ((CoinsTotoalMessageWriteToFlash.CoinTotoal-coins_remain)/10);
						cnt_t= ((CoinsTotoalMessageWriteToFlash.CoinTotoal-coins_remain)%10);
						VariableChage(coins_back,Coins_cnt);
						for(i=0;i<coins_time+1;i++) //һ���˱�10����
						{
							if(i!=coins_time)
							{
								UserActMessageWriteToFlash.UserAct.MoneyBack+=SendOutN_Coin(10);		
							}
							else
							{
								if(cnt_t>0)
									UserActMessageWriteToFlash.UserAct.MoneyBack+=SendOutN_Coin(cnt_t);	
								else
									break;
							}
							delay_ms(1000); 
							VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ����
							VariableChage(coins_back,Coins_cnt);						
							if(ErrorType ==1)  //�˱һ��ޱҴ���,ֱ�ӽ������״̬
							{
								break;
							}														
						}
					}	
					//�����ܷ��ͼ����Ǽ����ķ����ǲ����еģ�//CoinsTotoalMessageWriteToFlash.CoinTotoal = SendOutN_Coin(CoinsTotoalMessageWriteToFlash.CoinTotoal);			
				}
				else if(VariableData[1] == 0x02) /*ȫ���˱�*/
				{
					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)!=0)//�����޶�
					{
						Coins_cnt=0;
						coins_time= (CoinsTotoalMessageWriteToFlash.CoinTotoal/10);
						cnt_t= (CoinsTotoalMessageWriteToFlash.CoinTotoal%10);
						VariableChage(coins_back,Coins_cnt);
						for(i=0;i<coins_time+1;i++) //һ���˱�10����
						{
							if(i!=coins_time)
							{
								MoneyBack_cnt+=SendOutN_Coin(10);		
							}
							else
							{
								if(cnt_t>0)
									MoneyBack_cnt+=SendOutN_Coin(cnt_t);	
								else
									break;
							}
							delay_ms(1500); 
							VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal);//��ʾ����Ӳ����
							VariableChage(coins_back,Coins_cnt);												
						}							
						if(ErrorType ==1)  //�˱һ��ޱҴ���,�ٴη����˱�
						{
							MoneyBack_cnt= SendOutN_Coin(MoneyBack_cnt);
							//RefundButton(UserAct.MoneyBack);���ʹ�����������˱һ�����
							//����ȫ�˱ң���ʾ�˱ҳ�����棬������
						}	
					}						
				}
				else if(VariableData[1] == 0x03) /*����*/
				{
					StopRefond();
					PageChange(Menu_interface);
				}
			}break;	
      case 	coins_input: /*��Ҫ�����õ�Ӳ����������������*/
      {
        myunion.adr[0] =	VariableData[1];
				myunion.adr[1] =	VariableData[0];
				CoinTotoal_t   =  myunion.adress;
				VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal+ CoinTotoal_t);//��ʾ����Ӳ����			
			}break;
      case set_sellmeal_hour: /*�۲����õ�Сʱ*/	
			{
				selltime_hour_t= VariableData[1];		
			}break;				
			case set_sellmeal_minute:/*�۲����õķ�*/	
			{
				selltime_minute_t= VariableData[1];				
      }break;				
			case set_sellmeal_key: /*�۲����õİ���*/
			{
				if(VariableData[1]==0x01) /*ȷ��*/
				{
					selltime_hour= selltime_hour_t;
					selltime_minute= selltime_minute_t;
					DisplayTimeCutDown();
        }
				else if(VariableData[1]==0x02) /*����*/
				{
          PageChange(Data_synchronization);				
        }
      }break;
			  default:break;		
		}
}
 /*******************************************************************************
 * ��������:DealSeriAceptData                                                                   
 * ��    ��:����ӵ�����Ļ���ص�����                                                        
 *          ���ʵ���������������ݣ�0xA5 0XA5 0X5A                                                                           
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 	
void DealSeriAceptData(void)
 {
	unsigned char i;
	unsigned char temp,temp1,length,checkout;
	char RegisterAdress=0;
	int16_t VariableAdress=0;
	
	unsigned char Rx3DataBuff[10]={0};/*����һ�������С��?����Խ��(out of bounds),?�ɱ䳤������*/
	char RegisterData[5]={0};  //�Ĵ�����������
	char VariableData[5]={0};  //������������
	char RegisterLength= 0;   //�Ĵ������ݵĳ���
	char VariableLength= 0;   //�������ݵĳ���
	while(UART3_GetCharsInRxBuf()>=9) //��ȡ��������С��ֱ��������������
	{	
		if(USART3_GetChar(&temp)==1)
		{	
			if(temp==FH0)	//ƥ��֡ͷ��һ���ֽ�
			{ 
loop:	  if(USART3_GetChar(&temp1)==1)  
				{
					if(temp1==FH1)  //ƥ��֡ͷ�ڶ����ֽڣ�
					{
						if(USART3_GetChar(&length)==1)  //��ȡ�����ֽڳ���
						{
							checkout =1;//��λ��ʼ��ȡ���������ݱ�־	
						}	
					}
					else if(temp1==FH0)	goto loop; //�����������0xA5������ж���һ���ֽ��Ƿ�Ϊ0x5A
				}
			}
		}
	  if(checkout==1) //ֱ����ȡ���ݳ��ȣ�Ȼ�������
	  {
	 	  checkout =0;  //��λ��־
		  for(i=0;i<length;i++) USART3_GetChar(Rx3DataBuff+i);
		  if(Rx3DataBuff[0]==0x81)  //���Ĵ�����������
		  {
			  RegisterAdress =Rx3DataBuff[1];
			  for(i=0;i<(length-2);i++)
			  RegisterData[i]=Rx3DataBuff[2+i];
				//�����޸�������ݵĹ���
				RecRegisterValues(RegisterAdress,RegisterData,RegisterLength);
		  }
		  else if(Rx3DataBuff[0]==0x83) //�����ݴ洢����������
		  {
			  myunion.adr[1] =Rx3DataBuff[1]; //�п�����С��ģʽ�����
			  myunion.adr[0] =Rx3DataBuff[2];
			  VariableAdress =myunion.adress;
			  VariableLength =Rx3DataBuff[3];
			  for(i=0;i<(length-4);i++)
			  VariableData[i]=Rx3DataBuff[4+i];
			  //�����޸ı�����ַ���ݵĹ���
			  ChangeVariableValues(VariableAdress,VariableData,VariableLength);
		   }
	   } 
	 }		
}

