#include "stm32f4xx.h"
#include "MsgHandle.h"
#include "serialscreen.h"
#include "bsp.h"

MealAttribute DefineMeal[MealKindTotoal];	//����6����ϵ
UserActMessage UserActMessageWriteToFlash;//�û�����д��flash
FloorAttribute FloorMeal[FloorMealNum], CurFloor;
UserSelection UserAct;
FloorMealMessage FloorMealMessageWriteToFlash;
CoinsTotoalMessage CoinsTotoalMessageWriteToFlash;
uint8_t  WaitTime=0;
uint8_t  WaitMealTime=0;

/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value: Byte to be converted.
  * @retval Converted byte
  */
static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint8_t bcdhigh = 0;
  
  while (Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }
  
  return  ((uint8_t)(bcdhigh << 4) | Value);
}

/**
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value: BCD value to be converted.
  * @retval Converted word
  */
static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (Value & (uint8_t)0x0F));
}

 /*******************************************************************************
 * ��������:StatisticsTotal                                                                     
 * ��    ��:ͳ����Ŀ                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
 void StatisticsTotal(void)
 {	 
    uint8_t i,j;
		for(j = 0; j < MealKindTotoal; j++)	 //4�����ID��
		{
      DefineMeal[j].MealCount = 0;
      for(i = 0; i < 15; i++)
      {
        if(FloorMealMessageWriteToFlash.FloorMeal[i].MealID == j + 1)	 /*ǰ���Ǹ���ʾ����ж��flash�����ID ������Ľ��бȽ�*/
        {
          DefineMeal[j].MealCount      = DefineMeal[j].MealCount + FloorMealMessageWriteToFlash.FloorMeal[i].MealCount;
          DefineMeal[j].Position[i][0] = FloorMealMessageWriteToFlash.FloorMeal[i].FCount;
          DefineMeal[j].Position[i][1] = FloorMealMessageWriteToFlash.FloorMeal[i].SCount;
          DefineMeal[j].Position[i][2] = FloorMealMessageWriteToFlash.FloorMeal[i].TCount;
        }
      }
		}
 }
 /*******************************************************************************
 * ��������:InitSetting                                                                   
 * ��    ��:�����õ���������                                                                
 *                                                                               
 * ��    ��:                                                                 
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��4��9��                                                                     
 *******************************************************************************/  
void InitSetting(void)
{
  if(FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealID == CurFloor.MealID)
  {
		CurFloor.MealCount = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].MealCount;
		CurFloor.FCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].FCount;
		CurFloor.SCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].SCount;
		CurFloor.TCount    = FloorMealMessageWriteToFlash.FloorMeal[CurFloor.FloorNum - 1].TCount;
	}
	else
	{
		CurFloor.MealCount = 0;
		CurFloor.FCount    = 0;
		CurFloor.SCount    = 0;
		CurFloor.TCount    = 0;
	}

	VariableChage(row_1st,CurFloor.FCount);
	VariableChage(row_2nd,CurFloor.SCount);
	VariableChage(row_3rd,CurFloor.TCount);
}

 /*******************************************************************************
 * ��������:SearchMeal                                                                    
 * ��    ��:���ҵ�ǰ�Ĳ�ƷID�Ƿ����û���ѡ�Ĳ�Ʒ�� �������û�ѡ��Ĳ�Ʒ����ID�����û�ѡ��Ľṹ����                                                                
 *                                                                               
 * ��    ��:��ǰ��Ʒ��ID�� ��ǰ��Ʒ������                                                                
 * ��    ��:��                                                                     
 * ��    ��:false:����  true:��ȡ                                                               
 * �޸�����:2014��9��19��                                                                    
 *******************************************************************************/ 
bool SearchMeal(uint32_t MealID ,uint32_t MealNum)
{
	/*�������ֵ��صĲ�ƷID������*/
  if(UserActMessageWriteToFlash.UserAct.MealType_1st== UserActMessageWriteToFlash.UserAct.MealID)
	{
		UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= MealNum;
    UserActMessageWriteToFlash.UserAct.MealPrice_1st=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1); //��ȡ��ǰ��Ʒ�ļ۸�
	}
	else if(UserActMessageWriteToFlash.UserAct.MealType_2nd== UserActMessageWriteToFlash.UserAct.MealID)
	{
		UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= MealNum;
    UserActMessageWriteToFlash.UserAct.MealPrice_2nd=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
	}
	else if(UserActMessageWriteToFlash.UserAct.MealType_3rd== UserActMessageWriteToFlash.UserAct.MealID)
	{
		UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= MealNum;
    UserActMessageWriteToFlash.UserAct.MealPrice_3rd=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);    
	}
	else if(UserActMessageWriteToFlash.UserAct.MealType_4th== UserActMessageWriteToFlash.UserAct.MealID)
	{
		UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= MealNum;
    UserActMessageWriteToFlash.UserAct.MealPrice_4th=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
	}
	/*���û���������һ���Ĳ�ƷΪ��*/
  else
	{
		if(UserActMessageWriteToFlash.UserAct.MealType_1st== 0) //�����һ��IDΪ�յĻ���ֱ�ӽ�ID��ֵ��
		{
			UserActMessageWriteToFlash.UserAct.MealType_1st= MealID;
			UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= MealNum;
      UserActMessageWriteToFlash.UserAct.MealPrice_1st=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
		}
		else if(UserActMessageWriteToFlash.UserAct.MealType_2nd== 0)
		{
			UserActMessageWriteToFlash.UserAct.MealType_2nd= MealID;
			UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= MealNum;
      UserActMessageWriteToFlash.UserAct.MealPrice_2nd=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
		}
		else if(UserActMessageWriteToFlash.UserAct.MealType_3rd== 0)
		{
			UserActMessageWriteToFlash.UserAct.MealType_3rd= MealID;
			UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= MealNum;
      UserActMessageWriteToFlash.UserAct.MealPrice_3rd=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
		}
		else if(UserActMessageWriteToFlash.UserAct.MealType_4th== 0)
		{
			UserActMessageWriteToFlash.UserAct.MealType_4th= MealID;
			UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= MealNum;
      UserActMessageWriteToFlash.UserAct.MealPrice_4th=GetMealPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
		}
		else
    {
			return false; //����
		}			
	}
	return true; //��ȷ
}

 /*******************************************************************************
 * ��������:MealArr                                                                     
 * ��    ��: ��Ʒ������                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
uint32_t payfor_meal = 0;        //������Ʒ�Ѹ���Ǯ

void MealArr(uint8_t index)
{
    uint32_t  PayBill=0;
    uint32_t  MoneyPayBack_Already_t=0;
    uint8_t   MealPrice_t=0;
/*-------��ȡID�ͼ۸��ۺ�۸�------------------------*/
    memcpy(CustomerSel.MealID,Meal_Union.Meal[index-1].MealID,4);    
    memcpy(CustomerSel.MealName,Meal_Union.Meal[index-1].MealName,20);
    memcpy(CustomerSel.MealPrice,Meal_Union.Meal[index-1].MealPrice,6);
    MealPrice_t= RTC_Bcd2ToByte(CustomerSel.MealPrice[4]);  //1.��ת��Ϊbin  
    MealPrice_t= GetMealLastPrice(index,1);                      //��ȡ�ۺ�۸�
    CustomerSel.MealPrice[4]= RTC_ByteToBcd2(MealPrice_t); //2.��bin��X�ۿۣ�Ȼ��ת��ΪBCD 
    //CustomerSel.MealPrice[4]= RTC_ByteToBcd2(MealPrice_t);  //2.��bin��X�ۿۣ�Ȼ��ת��ΪBCD 
/*------�����Ʒ������-----------------*/
		CustomerSel.MealNo  =  0x01; //ȡһ����Ʒ�ϴ�һ������
		/*�����Ʒ������*/
    CustomerSel.PayType =  UserActMessageWriteToFlash.UserAct.PayType;  //	UserAct.PayType  ;
	 	/*֧���˶���Ǯ*/
		PayBill= payfor_meal* 100; //�Ѹ���Ǯ=���ұҵ�Ǯ+��Ʒ�ĵ��ۣ�Ԫת��Ϊ��
		CustomerSel.DealBalance[0]      =	      PayBill / 10000000000 %100;
		CustomerSel.DealBalance[0]      =       CustomerSel.DealBalance[0]/10 *16 +CustomerSel.DealBalance[0]%10 ;   
		CustomerSel.DealBalance[1]      =	      PayBill / 100000000 %100;
		CustomerSel.DealBalance[1]      =       CustomerSel.DealBalance[1]/10 *16 +CustomerSel.DealBalance[1]%10 ;                              
		CustomerSel.DealBalance[2]      =	      PayBill / 1000000 %100;
		CustomerSel.DealBalance[2]      =       CustomerSel.DealBalance[2]/10 *16 +CustomerSel.DealBalance[2]%10 ;
		CustomerSel.DealBalance[3]      =	      PayBill / 10000 %100;
		CustomerSel.DealBalance[3]      =       CustomerSel.DealBalance[3]/10 *16 +CustomerSel.DealBalance[3]%10 ;
		CustomerSel.DealBalance[4]      =	      PayBill / 100 %100;
		CustomerSel.DealBalance[4]      =       CustomerSel.DealBalance[4]/10 *16 +CustomerSel.DealBalance[4]%10 ;
		CustomerSel.DealBalance[5]      =	      PayBill % 100 ;
		CustomerSel.DealBalance[5]      =       CustomerSel.DealBalance[5]/10 *16 +CustomerSel.Change[5]%10 ;		
		/*Ҫ�ϴ������������:��һ���ϴ����ۺ�MoneyPayBack_Already_2nd=0���ڶ����ϴ���ʱ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already=0��
    ��һ�ξ�ȡ��ʧ�ܵ�ʱ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already!=0,UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd!=0,������Ӿ����ܵ��˱�
    */
    MoneyPayBack_Already_t= (UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already + UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd) *100;
    
    CustomerSel.Change[0]      =	     MoneyPayBack_Already_t / 10000000000 %100;
    CustomerSel.Change[0]      =         CustomerSel.Change[0]/10 *16 +CustomerSel.Change[0]%10 ;   
    CustomerSel.Change[1]      =	     MoneyPayBack_Already_t / 100000000 %100;
    CustomerSel.Change[1]      =         CustomerSel.Change[1]/10 *16 +CustomerSel.Change[1]%10 ;                              
    CustomerSel.Change[2]      =	     MoneyPayBack_Already_t / 1000000 %100;
    CustomerSel.Change[2]      =         CustomerSel.Change[2]/10 *16 +CustomerSel.Change[2]%10 ;
    CustomerSel.Change[3]      =	     MoneyPayBack_Already_t / 10000 %100;
    CustomerSel.Change[3]      =         CustomerSel.Change[3]/10 *16 +CustomerSel.Change[3]%10 ;
    CustomerSel.Change[4]      =	     MoneyPayBack_Already_t / 100 %100;
    CustomerSel.Change[4]      =         CustomerSel.Change[4]/10 *16 +CustomerSel.Change[4]%10 ;
    CustomerSel.Change[5]      =	     MoneyPayBack_Already_t % 100 ;
    CustomerSel.Change[5]      =         CustomerSel.Change[5]/10 *16 +CustomerSel.Change[5]%10 ;
		/*�����Ʒ��ʣ�����*/
		CustomerSel.RemainMealNum[0]  =	 (DefineMeal[index-1].MealCount>>8)&0xff;
		CustomerSel.RemainMealNum[1]  =	  DefineMeal[index-1].MealCount &0xff;
    PayBill= 0; payfor_meal=0;UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already=0;UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd=0;
}
