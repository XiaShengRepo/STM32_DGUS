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

typedef struct DispMeal
{
	char meal_id;
	char meal_cnt;
	char meal_cost;
}DispMealStuct;

DispMealStuct DispMeal[4]={0};

const char price_1st= 15;
const char price_2nd= 20;
const char price_3rd= 25;
const char price_4th= 30;

const char meat_name[12]={"���ܲ�����"};
const char chicken_name[10]={"�㹽����"};
const char duck_name[10]={"��Ƥ��Ѽ"};
const char fish_name[10]={"�������"};

/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRam
{
		int16_t	 	adress ;
		int8_t  	adr[2] ;
}myunion;


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
		temp[2]= 4;
		myunion.adress= Variable; 
		temp[4]= myunion.adr[0];
		temp[5]= myunion.adr[1];
		myunion.adress= Value; 
		temp[6]= myunion.adr[0];
		temp[7]= myunion.adr[1];		
		Uart3_Send(temp,sizeof(temp));	
}							

 /*******************************************************************************
 * ��������:TextDisp                                                                  
 * ��    ��:��ʾ�ַ�������,                                                          
 *                                                                               
 * ��    ��:meal_name                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void TextDisp(uint16_t meal_name)							
{
	  char temp[20]={0};  //��Ŵ������ݵ���ʱ����
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 13; //0x83 0x00 0x41 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
		myunion.adress= column1st_name; 
		temp[4]= myunion.adr[1];
		temp[5]= myunion.adr[0];
		switch(meal_name)
		{
			case column1st_name:strcat(temp,meat_name);break;	
			case column2nd_name:strcat(temp,chicken_name);break;		
			case column3rd_name:strcat(temp,duck_name);break;	
			case column4th_name:strcat(temp,fish_name);break;
			default:break;			
		}
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
	temp[2]= PassWordLen+ 3; //  0x83 00 00 ******
	myunion.adress= password_show; 
  temp[4]= myunion.adr[1];
  temp[5]= myunion.adr[0];
	for(i=0;i<PassWordLen;i++) temp[i+6]='*';
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
void PageChange(char page)
{
		unsigned char temp[7]={0};
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));	
		temp[4]=	PIC_ID;	
	  temp[6]=  page;
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
		for(i=0;i<4;i++){		
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4;
		myunion.adress= meat+i; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[0];
		temp[5]= myunion.adr[1];
		//�����ݽ�����䣬��Ҫ�õ�flash
		temp[7]= DefineMeal[i].MealCount;
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
	  temp[2]= 4;
		myunion.adress= count_dowm; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[0];
		temp[5]= myunion.adr[1];
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
char GetMealPrice(char meal_type,char count)
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
	  temp[2]= 4;
		temp[5]= (0x31+(meal_id-1)*2); //Ǯ�ұ�����ַ
    temp[7]= GetMealPrice(meal_id,meal_count);	
		Uart3_Send(temp,sizeof(temp));		
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

	switch(UserAct.MealID)
	{
		case 0x01:
		{
			UserAct.MealCnt_1st=UserAct.MealCnt_1st_t;
		}break;
		case 0x02:
		{
			UserAct.MealCnt_2nd=UserAct.MealCnt_2nd_t;
		}break;
		case 0x03:
		{
			UserAct.MealCnt_3rd=UserAct.MealCnt_3rd_t;
		}break;
		case 0x04:
		{
			UserAct.MealCnt_4th=UserAct.MealCnt_4th_t;
		}break;
		default:break;	
	}
	PageChange(UserAct.MealID*3+6);
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
	//����������� current = payformoney
  //��ʾӲ����
	VariableChage(payment_coin,UserAct.PayForCoins);
  //��ʾֽ����
	VariableChage(payment_bill,UserAct.PayForBills);
  //ˢ����
	VariableChage(payment_card,UserAct.PayForCards);
  //��ʾ�ȴ�ʱ��
	VariableChage(wait_payfor,60);
	//������ʾ����
	Floor= 0; //ÿ�ν�һ�ν��˽������Ҫ����д������
	if(UserAct.MealCnt_1st>0)
	{
		DispMeal[Floor].meal_id= 0x01; //0x01Ϊ��һ����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_1st; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_1st; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}
	if(UserAct.MealCnt_2nd>0)
	{
		DispMeal[Floor].meal_id= 0x02; //0x02Ϊ�ڶ�����Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_2nd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_2nd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;
	}		
  if(UserAct.MealCnt_3rd>0)
	{
		DispMeal[Floor].meal_id= 0x03; //0x03Ϊ��������Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_3rd; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_3rd; //��Ʒ���ܼ۵ĸ�ֵ
		Floor++;		
	}		
	if(UserAct.MealCnt_4th>0)
	{
		DispMeal[Floor].meal_id= 0x04; //0x01Ϊ���ĸ���Ʒ��ID
		DispMeal[Floor].meal_cnt= UserAct.MealCnt_4th; //��Ʒ��������ֵ
		DispMeal[Floor].meal_cost= UserAct.MealCost_4th; //��Ʒ���ܼ۵ĸ�ֵ
	}
	//������ʾ�Ĳ���
	UserAct.PayShould= (UserAct.MealCost_1st+UserAct.MealCost_2nd+UserAct.MealCost_3rd+UserAct.MealCost_4th);
	VariableChage(mealtotoal_cost,UserAct.PayShould);
}

 /*******************************************************************************
 * ��������:GetPassWord                                                                     
 * ��    ��:ԭʼ���� ���Լ����޸����뺯��                                                               
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
 * ��������:ChangeVariableValues                                                                   
 * ��    ��:�ı����ݱ�����ֵ                                                                                                                          
 * ��    ��:������ַ,��������,��������                                                                  
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 
char meat_cnt_t= 0,chicken_cnt_t= 0,duck_cnt_t= 0,fish_cnt_t= 0; /*��ʱ����*/
uint8_t  PassWordLen=0;	//����ĳ���Ϊ0
uint8_t  PassWord[6]={0};
uint8_t  InputPassWord[6]={0};
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
							UserAct.MealID= VariableData[1]; //��ǰ�û�ѡ�͵�ID
							UserAct.MealCnt_1st= 1;//����Ĭ�Ϸ���Ϊ 1
							PageChange(Meal1st_interface);//��ʾ��Ӧ����
							CutDownDisp(60);//��ʾ����ʱʱ��
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x02:
						if(DefineMeal[1].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_2nd= 1;//����Ĭ�Ϸ���Ϊ 1
							PageChange(Meal2rd_interface);//��ʾ��Ӧ����	
							CutDownDisp(60);//��ʾ����ʱʱ��
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;							
					case 0x03:
						if(DefineMeal[2].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_3rd= 1;//����Ĭ�Ϸ���Ϊ 1
							PageChange(Meal3ns_interface);//��ʾ��Ӧ����	
							CutDownDisp(60);//��ʾ����ʱʱ��
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}break;						
					case 0x04:
					{
						if(DefineMeal[3].MealCount > 0)	   //�жϲ�Ʒ�Ƿ����0
						{
							UserAct.MealID= VariableData[1];
							UserAct.MealCnt_4th= 1;//����Ĭ�Ϸ���Ϊ 1
							PageChange(Meal4th_interface);//��ʾ��Ӧ����	
							CutDownDisp(60);//��ʾ����ʱʱ��
							MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th);//�����û���ѡ��ƷID����ʾ�ϼ�Ǯ��
						}			
					}break;
					case 0x05:  /*�����û���*/
					{
						PageChange(Password_interface);
					}break;
					default:break;
				}
			}break;
        				
			case meat_cnt: /*���ܲ��������*/
			{
				if(VariableData[1]<= DefineMeal[0].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_1st_t= VariableData[1];
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st_t);
				}
				else
				{
					UserAct.MealCnt_1st_t= DefineMeal[0].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_1st_t);
					VariableChage(meat_cnt,UserAct.MealCnt_1st_t);	//�ı������ַ����
				}	 						 
			}break;
			case chicken_cnt:/*�㹽��������*/
			{
				if(VariableData[1]<= DefineMeal[1].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_2nd_t= VariableData[1];
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd_t);
				}
				else
				{
					UserAct.MealCnt_2nd_t= DefineMeal[1].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_2nd_t);
					VariableChage(meat_cnt,UserAct.MealCnt_2nd_t);	//�ı������ַ����
				}	 						 
			}break;
			case duck_cnt:/*��Ƥ��Ѽ*/
			{
				if(VariableData[1]<= DefineMeal[2].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_3rd_t= VariableData[1];
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd_t);
				}
				else
				{
					UserAct.MealCnt_3rd_t= DefineMeal[2].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_3rd_t);
					VariableChage(meat_cnt,UserAct.MealCnt_3rd_t);	//�ı������ַ����			
				}	 
			}break;	
			case fish_cnt:/*�������*/
			{
				if(VariableData[1]<= DefineMeal[3].MealCount)	//���ò�Ʒѡ�������
				{
					UserAct.MealCnt_4th_t= VariableData[1];
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th_t);
				}
				else
				{
					UserAct.MealCnt_4th_t= DefineMeal[3].MealCount;
					MealCostDisp(UserAct.MealID,UserAct.MealCnt_4th_t);
					VariableChage(meat_cnt,UserAct.MealCnt_4th_t);	//�ı������ַ����
				}
			}break;	
   
      case mealcnt_choose:/*��ҳѡ��ť*/
      {
			  switch(VariableData[1])
				{
					case 0x01:  /*���빺�ﳵ*/
					{
						PutIntoShopCart();		
					}
					case 0x02:  /*����ѡ��*/
					{
						PageChange(Menu_interface);
					}
 					case 0x03:  /*����*/
					{
						SettleAccounts();
					}
					case 0x04:  /*ȡ��*/
					{
						//������е��û�����
						UserAct.MealCnt_1st_t=0;
						UserAct.MealCnt_2nd_t=0;
						UserAct.MealCnt_3rd_t=0;
						UserAct.MealCnt_4th_t=0;
						PageChange(Menu_interface);
					}
					default:break;
				}
			}break;
			/*�ı������в�Ʒ������*/
			/*��Ҫ��Ʒ��ID���к�:column1st_count, MealID*/
			case column1st_count:MealID= DispMeal[0].meal_id;
			case column2nd_count:MealID= DispMeal[1].meal_id;
			case column3rd_count:MealID= DispMeal[2].meal_id;
			case column4th_count:MealID= DispMeal[3].meal_id;
			{
				switch(MealID)
				{
					case 0x01:
					{
					  if(VariableData[1]<=DefineMeal[MealID].MealCount)//���û�ѡ������С�ڻ���ڴ��ʱ
				    {
					    UserAct.MealCnt_1st =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_1st =UserAct.MealCnt_1st *price_1st; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_1st); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_1st); //�ı䵥�ܼۣ���ַƫ��1
				    }
				    else
				    {	
							VariableChage(VariableAdress,UserAct.MealCnt_1st); //ά��ԭ����ѡ������
				    }					
					}break;
					case 0x02:
					{
					  if(VariableData[1]<=DefineMeal[MealID].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_2nd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_2nd =UserAct.MealCnt_2nd *price_2nd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_2nd); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_2nd); //�ı䵥�ܼ�
				    }
				   else
				    {
					    VariableChage(VariableAdress,UserAct.MealCnt_2nd); //ά��ԭ����ѡ������
				    }									
					}break;
					case 0x03:
					{
					  if(VariableData[1]<=DefineMeal[MealID].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_3rd =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_3rd =UserAct.MealCnt_3rd *price_3rd; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_3rd); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_3rd); //�ı䵥�ܼ�
				    }
				   else
				    {
					     VariableChage(VariableAdress,UserAct.MealCnt_3rd); //ά��ԭ����ѡ������
				    }							
					}break;
					case 0x04:
					{
					  if(VariableData[1]<=DefineMeal[MealID].MealCount)//����Ҫ�ж�����
				    {
					    UserAct.MealCnt_4th =VariableData[1]; //���ı��ֵ���ص��ṹ����
					    UserAct.MealCost_4th =UserAct.MealCnt_4th *price_4th; //���㵥�ܼ�
					    VariableChage(VariableAdress,UserAct.MealCnt_4th); //�ı�����
					    VariableChage(VariableAdress+1,UserAct.MealCost_4th); //�ı䵥�ܼ�
				    }
				   else
				    {
					     VariableChage(VariableAdress,UserAct.MealCnt_4th); //ά��ԭ����ѡ������
				    }							
					}break;
					default:break;						
				}
				UserAct.PayShould= (UserAct.MealCost_1st+UserAct.MealCost_2nd+UserAct.MealCost_3rd+UserAct.MealCost_4th);
	      VariableChage(mealtotoal_cost,UserAct.PayShould);
			}break;
			case payment_method: /*���ʽ*/
			{
				switch(VariableData[1])
				{
					case 0x01:   /*�ֽ�֧��*/
					{
						//OpenCashSystem();
					}break;
					case 0x02:   /*����Ԥ����*/
					{
						//CloseCashSystem();
					}break;
					case 0x03:   /*����֧ͨ��*/
					{
						//CloseCashSystem();
					}break;
					case 0x04:   /*ȡ��*/
					{
						//CloseCashSystem();
					}break;
					default:break;		
				}					
			}break;
			case bill_print:
			{
				switch(VariableData[1])
					case 0x01:   /*��ӡСƱ*/
					case 0x02:   /*����ӡСƱ*/
					  default:break;
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
					case 0x11:/*ȷ�ϼ�*/
					{
	          GetPassWord(PassWord);
    	      if(VerifyPassword(PassWord, InputPassWord,6) == 0)
		        {
		          /*������֤���󣬽��������������*/
		           PageChange(Password_interface);
							 DisplayPassWord(0);
			         PassWordLen = 0;
		        }
		        else
		        {
		           /*���� ����Ա����*/
		           PageChange(MealSet_interface);
							 DisplayPassWord(0);//���������ʾ
			         PassWordLen = 0;
		        }
	        }break;						
					case 0x12:/*����*/
					{
		        PageChange(Menu_interface);
						DisplayPassWord(0);//���������ʾ
		        PassWordLen = 0;	
					}break;
          default:break;					
        } 					
			}
			case admin_set:
			{
				switch(VariableData[1])
				{
					case 0x01:  /*�¶����ð�ť*/
					case 0x02:  /*��Ʒ���ð�ť*/
					case 0x03:	/*ȡ����*/
          default:break;
				}					
			}break;
			case meal_num:
			{
				//VariableData[1]; ��Ҫ�Ե�ǰ�ͺŽ��м�¼,Ȼ���ٴ���
			  CurFloor.MealID= VariableData[1];	
        InitSetting();//��յ�һ�����е����� 
			}break;
      case floor_num:
			{
			  CurFloor.FloorNum= VariableData[1];
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
						for(j = 0; j < 4; j++)	 //4�����ID��
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
							{														 /*�͵�ID�Ǵ�1-9*/
								if(FloorMealMessageWriteToFlash.FloorMeal[i].MealID == j + 1)
								{
									DefineMeal[j].MealCount      = DefineMeal[j].MealCount + FloorMealMessageWriteToFlash.FloorMeal[i].MealCount;
									DefineMeal[j].Position[i][0] = FloorMealMessageWriteToFlash.FloorMeal[i].FCount;
									DefineMeal[j].Position[i][1] = FloorMealMessageWriteToFlash.FloorMeal[i].SCount;
									DefineMeal[j].Position[i][2] = FloorMealMessageWriteToFlash.FloorMeal[i].TCount;
								}
							}
						}
						WriteMeal();
					}break;
					case 0x04:  /*ȡ��*/
					{
						CurFloor.FCount    = 0;
						CurFloor.SCount    = 0;
						CurFloor.TCount    = 0;
						CurFloor.MealID    = 0;
						CurFloor.MealCount = 0;
						CurFloor.FloorNum  = 0;	
						PageChange(Menu_interface);
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
					default:break;	
				}
			}break;
			case temprature_set:
			{
			//VariableData[1]; �¶ȱ���=VariableData[1];
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
	char VariableLength= 0;   //�������ݵĳ���
	while(UART3_GetCharsInRxBuf()>0) //��ȡ��������С��ֱ��������������
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

