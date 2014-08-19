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
 * ��������:MealArr                                                                     
 * ��    ��: ��Ʒ������                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
uint32_t payfor_meal = 0;        //������Ʒ�Ѹ���Ǯ

/*�ϴ����˱�����Ϊ����״̬��*/
uint32_t MoneyPayBack_Already=0;      //�����ϴ���
uint32_t MoneyPayBack_Already_1st = 0;//��Ǯ����ұ�
uint32_t MoneyPayBack_Already_2nd = 0;//ȡ�ͳ������ұ�
void MealArr(unsigned char index)
{
    uint32_t  PayBill=0;
    uint32_t  MoneyPayBack_Already_t=0;
	  switch(index)
	  {
      case 0x01 :	/*�����Ʒ��ID*/
			{				
 		    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x41;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x16 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 0x02 :	/*�����Ʒ��ID*/
			{
 		    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x42;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x16 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 0x03 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x43;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x16 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;
      case 0x04 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x44;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x16 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;	 
      case 0x05 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x45;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x18 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;	
      case 0x06 :	/*�����Ʒ��ID*/
			{
 		  	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x46;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x18 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
			}break ;	
	    default :
			{
 		  	CustomerSel.MealID[0]		   =   0x00;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x00;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x00 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;				
      }break ;
	  }	
/*------�����Ʒ������-----------------*/
		CustomerSel.MealNo  =  0x01; //ȡһ����Ʒ�ϴ�һ������
		/*�����Ʒ������*/
    CustomerSel.PayType =  UserActMessageWriteToFlash.UserAct.PayType;  //	UserAct.PayType  ;
		CustomerSel.MealName =  	index ;
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
		/*Ҫ�ϴ������������:��һ���ϴ����ۺ�MoneyPayBack_Already_2nd=0���ڶ����ϴ���ʱ��MoneyPayBack_Already=0��
    ��һ�ξ�ȡ��ʧ�ܵ�ʱ��MoneyPayBack_Already!=0,MoneyPayBack_Already_2nd!=0,������Ӿ����ܵ��˱�
    */
    MoneyPayBack_Already_t= (MoneyPayBack_Already + MoneyPayBack_Already_2nd) *100;
    
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
    PayBill= 0; payfor_meal=0;MoneyPayBack_Already=0;MoneyPayBack_Already_2nd=0;
}
