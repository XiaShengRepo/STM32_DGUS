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
char temprature_old=0;

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

uint8_t *mystrcat(uint8_t *dest, const uint8_t *src, uint8_t length)
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
uint8_t pageunitil= 0;
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
 * ��������:DispMenu
 * ��    ��:��ʾ�ĸ���Ʒ�����ƺͼ۸�                                                          
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��11��16��                                                                    
 *******************************************************************************/ 
void DispMenu(void)
{
		uint8_t i;
	  unsigned char temp[30]={0};  //��Ŵ������ݵ���ʱָ��
    GetMealDetail();   //��ȡ��Ʒ����Ϣ
		for(i=0;i<4;i++)  //��ʾ����
		{
      memcpy(temp,VariableWrite,sizeof(VariableWrite));
      temp[2]= sizeof(Meal_Union.Meal[i].MealName);
      myunion.adress= menu_name_1st+i*0x0100; //�ڻ���ַ����λ��
      temp[4]= myunion.adr[1];
      temp[5]= myunion.adr[0];
			memcpy(temp+6,Meal_Union.Meal[i].MealName,sizeof(Meal_Union.Meal[i].MealName));	
			Uart3_Send(temp,sizeof(temp));	
		}
    for(i=0;i<4;i++) //��ʾ����
    {
      memcpy(temp,VariableWrite,sizeof(VariableWrite));
      temp[2]= 5;
      myunion.adress= menu_price_1st+i*0x0010; //�ڻ���ַ����λ��
      temp[4]= myunion.adr[1];
      temp[5]= myunion.adr[0];
      switch(i)
      {
        case 0:temp[7]= price_1st;break;
        case 1:temp[7]= price_2nd;break;
        case 2:temp[7]= price_3rd;break;
        case 3:temp[7]= price_4th;break;
        default:break;
      }
			Uart3_Send(temp,sizeof(temp));      
    }    
}

 /*******************************************************************************
 * ��������:DispMenuNone
 * ��    ��:���м���ʾ��ƷΪ������������                                                         
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2015��1��17��                                                                    
 *******************************************************************************/ 
const uint8_t menu_err[14]={"������������"};
void DispMenuNone(void)
{
  uint8_t temp[30]={0};  //��Ŵ������ݵ���ʱָ��
  memcpy(temp,VariableWrite,sizeof(VariableWrite));
  temp[2]= sizeof(menu_err)+5;
  myunion.adress= menu_name_none;
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
  memcpy(temp+6,menu_err,sizeof(menu_err));	
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
		for(i=0;i<4;i++)
		{
      memcpy(temp,VariableWrite,sizeof(VariableWrite));
      temp[2]= 5;
      myunion.adress= meal1st_left+i; //�ڻ���ַ����λ��
      temp[4]= myunion.adr[1];
      temp[5]= myunion.adr[0];	
      temp[7]= DefineMeal[sell_type[i]-1].MealCount;//sell_type[i]�洢�����ĸ���Ʒ��ID			
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
 * ��������:GetMealPrice                                                                     
 * ��    ��:��ȡ��ǰ��Ʒ�ļ۸�                                                        
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 
uint32_t GetMealPrice(uint8_t meal_type,uint8_t count)
{
	char price= 0;
  switch(meal_type)
	{
		case 0x01:price= price_1st*count;break;
		case 0x02:price= price_2nd*count;break;
		case 0x03:price= price_3rd*count;break;
		case 0x04:price= price_4th*count;break;
		default:break;
	}
	return price;
}

 /*******************************************************************************
 * ��������:GetMealLastPrice                                                                     
 * ��    ��:��ȡ��ǰ��Ʒ���ۺ�۸�                                                        
 *           ���жϲ�Ʒ��ID������ݸ��ʽ����ȡ��ǰ�Ĳ�Ʒ�ڵ�ǰ�ĸ��ʽ�µ��ۿۡ�                                                                  
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2015��1��17��                                                                    
 *******************************************************************************/ 
uint32_t GetMealLastPrice(uint8_t meal_type,uint8_t count)
{
	uint32_t price= 0;
  uint32_t last_discount_t= 0; /*�ۿ�����*/
  switch(meal_type)
	{
		case 0x01:  /*��ȷ����ƷID*/
		{
      switch(UserActMessageWriteToFlash.UserAct.PayType)
      {
        /*����ݸ��ʽUserActMessageWriteToFlash.UserAct.PayType����ȡ�Ӻ�̨�·���ԭʼ�ۿ�����*/
        case 0x31: /*�ֽ�*/
          last_discount_t= cashcut_1st;break;
        case 0x32: /*����*/
          last_discount_t= gboccut_1st;break;
        case 0x33: /*����ͨ*/
          last_discount_t= sztcut_1st;break;
        case 0x34: /*��Ա��*/
          last_discount_t= vipcut_1st;break;
        default:break;
      }
		}break;
		case 0x02:  /*���ݲ�Ʒ��˳��ı�*/
		{
      switch(UserActMessageWriteToFlash.UserAct.PayType)
      {
        case 0x31: /*�ֽ�*/
          last_discount_t= cashcut_2nd;break;
        case 0x32: /*����*/
          last_discount_t= gboccut_2nd;break;
        case 0x33: /*����ͨ*/
          last_discount_t= sztcut_2nd;break;
        case 0x34: /*��Ա��*/
          last_discount_t= vipcut_2nd;break;
        default:break;
      }
		}break;
		case 0x03:  /*���ݲ�Ʒ��˳��ı�*/
		{
      switch(UserActMessageWriteToFlash.UserAct.PayType)
      {
        case 0x31: /*�ֽ�*/
          last_discount_t= cashcut_3rd;break;
        case 0x32: /*����*/
          last_discount_t= gboccut_3rd;break;
        case 0x33: /*����ͨ*/
          last_discount_t= sztcut_3rd;break;
        case 0x34: /*��Ա��*/
          last_discount_t= vipcut_3rd;break;
        default:break;
      }
		}break;
		case 0x04:  /*���ݲ�Ʒ��˳��ı�*/
		{
      switch(UserActMessageWriteToFlash.UserAct.PayType)
      {
        case 0x31: /*�ֽ�*/
          last_discount_t= cashcut_4th;break;
        case 0x32: /*����*/
          last_discount_t= gboccut_4th;break;
        case 0x33: /*����ͨ*/
          last_discount_t= sztcut_4th;break;
        case 0x34: /*��Ա��*/
          last_discount_t= vipcut_4th;break;
        default:break;
      }
		}break;    
		default:break;
	}
  price= price_1st*last_discount_t/100*count;
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
	UserActMessageWriteToFlash.UserAct.MealType_1st=0;
	UserActMessageWriteToFlash.UserAct.MealType_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealType_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealType_4th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_1st=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_1st_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_4th=0;
	UserActMessageWriteToFlash.UserAct.MealCnt_4th_t=0;
	UserActMessageWriteToFlash.UserAct.MealPrice_1st=0;
	UserActMessageWriteToFlash.UserAct.MealPrice_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealPrice_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealPrice_4th=0;   
  UserActMessageWriteToFlash.UserAct.LastMealPrice_1st=0;
	UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd=0;
	UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd=0;
	UserActMessageWriteToFlash.UserAct.LastMealPrice_4th=0; 
	UserActMessageWriteToFlash.UserAct.MealCost_1st=0;
	UserActMessageWriteToFlash.UserAct.MealCost_2nd=0;
	UserActMessageWriteToFlash.UserAct.MealCost_3rd=0;
	UserActMessageWriteToFlash.UserAct.MealCost_4th=0;
	UserActMessageWriteToFlash.UserAct.LastMealCost_1st=0;
	UserActMessageWriteToFlash.UserAct.LastMealCost_2nd=0;
	UserActMessageWriteToFlash.UserAct.LastMealCost_3rd=0;
	UserActMessageWriteToFlash.UserAct.LastMealCost_4th=0;  
	UserActMessageWriteToFlash.UserAct.MealID=0;
	UserActMessageWriteToFlash.UserAct.Meal_totoal=0;
	UserActMessageWriteToFlash.UserAct.Meal_takeout=0;
	UserActMessageWriteToFlash.UserAct.PayShould=0;
  UserActMessageWriteToFlash.UserAct.LastPayShould=0;
	UserActMessageWriteToFlash.UserAct.PayType=0x30;            //δѡ��֧����ʽ
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

  UserActMessageWriteToFlash.UserAct.MealCnt_1st= UserActMessageWriteToFlash.UserAct.MealCnt_1st_t;
  UserActMessageWriteToFlash.UserAct.MealCost_1st = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealType_1st,UserActMessageWriteToFlash.UserAct.MealCnt_1st);

  UserActMessageWriteToFlash.UserAct.MealCnt_2nd=UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t;
  UserActMessageWriteToFlash.UserAct.MealCost_2nd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealType_2nd,UserActMessageWriteToFlash.UserAct.MealCnt_2nd);

  UserActMessageWriteToFlash.UserAct.MealCnt_3rd=UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t;
  UserActMessageWriteToFlash.UserAct.MealCost_3rd = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealType_3rd,UserActMessageWriteToFlash.UserAct.MealCnt_3rd);

  UserActMessageWriteToFlash.UserAct.MealCnt_4th=UserActMessageWriteToFlash.UserAct.MealCnt_4th_t;
  UserActMessageWriteToFlash.UserAct.MealCost_4th = GetMealPrice(UserActMessageWriteToFlash.UserAct.MealType_4th,UserActMessageWriteToFlash.UserAct.MealCnt_4th);
  
  UserActMessageWriteToFlash.UserAct.PayShould= UserActMessageWriteToFlash.UserAct.MealCost_1st +UserActMessageWriteToFlash.UserAct.MealCost_2nd\
                                                +UserActMessageWriteToFlash.UserAct.MealCost_3rd+UserActMessageWriteToFlash.UserAct.MealCost_4th;

	UserActMessageWriteToFlash.UserAct.Meal_totoal= +UserActMessageWriteToFlash.UserAct.MealCost_4th+UserActMessageWriteToFlash.UserAct.MealCnt_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_2nd+UserActMessageWriteToFlash.UserAct.MealCnt_1st;
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
  PageChange(Acount_interface);//�������
  //��ʾӲ����
	VariableChage(payment_coin,UserActMessageWriteToFlash.UserAct.PayForCoins);
  //��ʾֽ����
	VariableChage(payment_bill,UserActMessageWriteToFlash.UserAct.PayForBills);
  //ˢ����
	VariableChage(payment_card,UserActMessageWriteToFlash.UserAct.PayForCards);
  /*�����˵�*/
	OpenTIM7();
  
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
	  uint8_t temp[50]={0};  //��Ŵ������ݵ���ʱ����
    uint8_t length_t=0;
    uint8_t *p_t=" ";
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 17; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= sync_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,mealname_1st,strcspn(mealname_1st,p_t));break;		
			case 0x02:mystrcat(temp,mealname_2nd,strcspn(mealname_2nd,p_t));break;		
			case 0x03:mystrcat(temp,mealname_3rd,strcspn(mealname_3rd,p_t));break;	
			case 0x04:mystrcat(temp,mealname_4th,strcspn(mealname_4th,p_t));break;	   
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
	  uint8_t temp[50]={0};  //��Ŵ������ݵ���ʱ����
    uint8_t *p_t=" ";
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 17; //0x83 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= record_column1st_name+ floor*(0x0100); 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_id)
		{
			case 0x00:break;
			case 0x01:mystrcat(temp,mealname_1st,strcspn(mealname_1st,p_t));break;		
			case 0x02:mystrcat(temp,mealname_2nd,strcspn(mealname_2nd,p_t));break;		
			case 0x03:mystrcat(temp,mealname_3rd,strcspn(mealname_3rd,p_t));break;	
			case 0x04:mystrcat(temp,mealname_4th,strcspn(mealname_4th,p_t));break;	   
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
 *               ��Ҫ�޸�                                                                
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/  
void DisplayUserRecord(void)
{
	uint8_t floor=0;
	for(floor=0;floor<4;floor++)		//�����Ļ����
	{
		AbnomalMealNameDisp(0,floor);
		AbnomalMealCnttDisp(0xff,floor);
		AbnomalMealCntDisp(0xff,floor);		
	}
	floor=0; //��ԭ
	if(UserActMessageWriteToFlash.UserAct.MealCnt_1st_t>0)  
	{
		AbnomalMealNameDisp(UserActMessageWriteToFlash.UserAct.MealType_1st,floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_1st_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_1st,floor);		
		floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t>0)
	{
		AbnomalMealNameDisp(UserActMessageWriteToFlash.UserAct.MealType_2nd,floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_2nd,floor);	
		floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t>0)
	{
		AbnomalMealNameDisp(UserActMessageWriteToFlash.UserAct.MealType_3rd,floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_3rd,floor);	
		floor++;
	}
	if(UserActMessageWriteToFlash.UserAct.MealCnt_4th_t>0)
	{
		AbnomalMealNameDisp(UserActMessageWriteToFlash.UserAct.MealType_4th,floor);
		AbnomalMealCnttDisp(UserActMessageWriteToFlash.UserAct.MealCnt_4th_t,floor);
		AbnomalMealCntDisp(UserActMessageWriteToFlash.UserAct.MealCnt_4th,floor);	
		floor++;
	}
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
	PassWord[3] = 2;
	PassWord[4] = 0;
	PassWord[5] = 7;
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
  PassWord[0] = 0x01;
	PassWord[1] = 0x08;
	PassWord[2] = 0x04;
	PassWord[3] = 0x07;
	PassWord[4] = 0x05;
	PassWord[5] = 0x0b;
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
	PassWord[1] = 1;
	PassWord[2] = 6;
	PassWord[3] = 6;
	PassWord[4] = 1;
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
uint8_t  clear_cnt=0; //���Ӳ��������
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
	uint8_t i= 0,j=0;
	  switch (VariableAdress)
		{
			case meal_choose: /*�������û�ѡ��*/
			{ 
				//����ǰ����ǹ����û������Ļ���ֱ�ӵ�������Ի���break
				if(VariableData[1]==0x0f) 
				{
					if((Current == current_temperature)||(Current == erro_hanle)||(Current == 0))
						PageChange(Password_interface); 
						break;
				}	
				//�ж��ǲ����ڳ���״̬,������ǿ���״̬��break;
				else if(Current != current_temperature)
        {
          break;
        }
				//����������ĸ������ķ���ֵ������ײ��ж�����һ��ID
				else
				{
          //��Ҫ���뵱û�в�Ʒ��ʱ�򣬲�����ѡ�ͽ���
				  UserActMessageWriteToFlash.UserAct.MealID= sell_type[VariableData[1]-1];
          if(DefineMeal[UserActMessageWriteToFlash.UserAct.MealID-1].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
          {
            SearchMeal(UserActMessageWriteToFlash.UserAct.MealID ,1); //û�з�����ȱʡֵ����ѡ��Ļ���¼��1
            WaitTimeInit(&WaitTime);
            OpenTIM3();
            PageChange(MealNumChoose_interface);//��ʾ��Ӧ����
            PlayMusic(VOICE_1);
				  }
        }
			}break;      				
			case meal_choose_cnt: /*��ǰ��Ʒ�ķ���ѡ��*/
			{
        //pageunitil = MealNumChoose_interface;
				if(VariableData[1]<= DefineMeal[UserActMessageWriteToFlash.UserAct.MealID-1].MealCount)	//���ò�Ʒѡ�������
				{
          PageChange(MealNumChoose_interface+VariableData[1]); //�л���ѡ�˼�����Ʒ
					SearchMeal(UserActMessageWriteToFlash.UserAct.MealID ,VariableData[1]); //���ҵ�ǰ�Ĳ�ƷID�Ƿ����û���ѡ�Ĳ�Ʒ��			
				}
				else
				{
					PageChange(MealNumChoose_interface+ DefineMeal[UserActMessageWriteToFlash.UserAct.MealID-1].MealCount); //�л���ѡ�˼�����Ʒ
					SearchMeal(UserActMessageWriteToFlash.UserAct.MealID ,DefineMeal[UserActMessageWriteToFlash.UserAct.MealID-1].MealCount); //���ҵ�ǰ�Ĳ�ƷID�Ƿ����û���ѡ�Ĳ�Ʒ��			          
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
            PutIntoShopCart(); //��Ҫ�޸�
						PageChange(Menu_interface);
						WaitTimeInit(&WaitTime);
					}break;
 					case 0x03:  /*���빺�ﳵ*/
					{
            if(sellmeal_flag)
            {
              PutIntoShopCart();
              SettleAccounts();
              AcountCopy();
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
			case payment_method: /*���ʽ*/ 
			{
        uint32_t temp1= 0,temp2= 0;
				if(UserActMessageWriteToFlash.UserAct.PayShould==0) goto loop7;
				switch(VariableData[1])
				{
					case 0x01:   /*�ֽ�֧��*/
					{
            //if(cash_limit_flag==true) break; //�����Ǯ���꣬�޷�����
            PageChange(PayWithCash_interface);
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
          case 0x04:   /*��Ա��֧��*/
          {
#ifdef test
            CurrentPoint =5;
            if(!CloseCashSystem()){CloseCashSystem();};// printf("cash system is erro3");  //�ر��ֽ����
#endif
          }break;      
					case 0x05:   /*ȡ��*/
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
				cardbalence_cancel_flag=true;
			}break;      
			case bill_print:
			{
				switch(VariableData[1])
				{
					case 0x01:   /*��ӡСƱ*/
          {
            if(UserActMessageWriteToFlash.UserAct.PrintTick==0x00000000)
            {
              UserActMessageWriteToFlash.UserAct.PrintTick= 0x00000001;
              PrintTickFun(&UserActMessageWriteToFlash.UserAct.PrintTick);
							PageChange(Menu_interface);
              CloseTIM4();
            }              
          }break;
					case 0x02:   /*����ӡСƱ*/
					{
            if(UserActMessageWriteToFlash.UserAct.PrintTick==0x00000000)
            {
              UserActMessageWriteToFlash.UserAct.PrintTick= 0x00000002;
              PrintTickFun(&UserActMessageWriteToFlash.UserAct.PrintTick);
							PageChange(Menu_interface);
              CloseTIM4();				
            }
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
                 /*��*/
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
				uint8_t cnt_t= 0;
			  CurFloor.MealID= sell_type[VariableData[1]-1];	//��ǰ�ľ��ǲ�Ʒ��ID
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
							/*��ʾ��Ʒ,����,�����ʾ*/									
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
						if(0==MealDataCompareFun()) /*ͬ����ʱ���أ�����ʾ�Ա�����*/      
            {
              PageChange(Data_synchronization+2);
              break;        
            }
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
          VariableChage(temprature_set,temprature_old);
        }
        else
        {
          SetTemper(VariableData[1]);
          temprature_old= VariableData[1];  
          auto_heat_flag = false;					
          if(VariableData[1]==99)  VariableChage(temprature_set,temprature_old);  //��ʾ��ǰ���¶�   
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
						}	
					}						
				}
				else if(VariableData[1] == 0x03) /*����*/
				{
          clear_cnt=0;
					StopRefond();
					PageChange(Menu_interface);
				}
			}break;	
      case coins_input: /*��Ҫ�����õ�Ӳ����������������*/
      {
        myunion.adr[0] =	VariableData[1];
				myunion.adr[1] =	VariableData[0];
				CoinTotoal_t   =  myunion.adress;
				VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal+ CoinTotoal_t);//��ʾ����Ӳ����			
			}break;
      case coins_clear: /*���Ӳ�Ҽ���*/
      {
        if(VariableData[1]==0x01)
        {
          clear_cnt++;
          if(clear_cnt>5)
          {
            clear_cnt=0;
            CoinTotoal_t  =  0;
            CoinsTotoalMessageWriteToFlash.CoinTotoal = 0;
            VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal+ CoinTotoal_t);//��ʾ����Ӳ����	        
            VariableChage(coins_input,CoinsTotoalMessageWriteToFlash.CoinTotoal+ CoinTotoal_t); //�����ӵ�Ӳ��������
            WriteCoins();
          }
        }
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

