
#include "bsp.h"
 /*******************************************************************************
 * ��������:WaitTimeInit                                                                     
 * ��    ��:��WaitTime���°��ŵ��µ�ֵ                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
 void  WaitTimeInit(uint8_t *Time)
 {
       	*Time = 60;
 }

 /*******************************************************************************
 * ��������:FindMeal                                                                     
 * ��    ��:�����Ǹ��ط��в�                                                                  
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
 uint16_t   Balance;
 static     uint8_t IsCard	;
 uint8_t    DisplayTimes	 ;
 uint8_t    Line = 0,Column = 0 ;
 bool FindMeal(MealAttribute *DefineMeal) 
 {
    for(Line = 0; Line < FloorMealNum; Line++)  //���Ҳ�
		{
			for(Column = 0; Column < 3; Column++)     //������
			{
			  if(DefineMeal[UserAct.MealID - 1].Position[Line][Column] > 0)	
			    return true;		                           //����λ���в͵Ļ�����0
			}
		}
		return false ;
 }


  /*******************************************************************************
 * ��������:PrintTick                                                                     
 * ��    ��:�Ƿ��ӡСƱ                                                                
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
			
void PrintTickFun(unsigned char *PrintTickFlag)
{      
	if(*PrintTickFlag == 0x01 )
	{
		 TIM_Cmd(TIM4, DISABLE);
	 	*PrintTickFlag = 0 ;
		/*��ӡСƱ�ĺ���*/
		 SPRT();
	   TIM_Cmd(TIM4, ENABLE);
	}	 
	if(*PrintTickFlag == 0x02 )
	{
	   TIM_Cmd(TIM4, DISABLE);
		 *PrintTickFlag = 0;
		 TIM_Cmd(TIM4, ENABLE);
	}
	
}	
  /*******************************************************************************
 * ��������:WaitPayMoney                                                                     
 * ��    ��:�ȴ������û�Ͷ��Ǯ                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
uint8_t CurrentPoint = 1 ; 
uint8_t UserPayMoney =0 ;
unsigned char NewCoinsCnt=0;
unsigned char OldCoinsCnt=0;
unsigned char  WaitPayMoney(void)
{
	uint8_t temp = 0;
	uint8_t temp1;
  switch(CurrentPoint)
	{
	  case 1 : 
	  {
	    /*��ʾ���ʽ���ֽ����п�������ͨ*/
			if(WaitTime<56)
			{
			  CurrentPoint = 3;
			  /*֧����ʽ*/			 
			  UserAct.PayType = '1';/* �ֽ�֧��*/
			}
		}break;    		
	  case 2:  //����Ļ������ת
	  {
	    /*����ѡ�񸶿ʽ����*/
			CurrentPoint = 3 ;
		  UserAct.PayType = '1' ;/* �ֽ�֧��*/
	  }break;	  
		         
	  case 3 :  //��Ǯ
		{			
	    UserPayMoney = ReadBills();			 
		  if(UserPayMoney !=0 )	   //��ʾ�յ���Ǯ
		  {
			  UserAct.PayAlready  += UserPayMoney;
			  UserAct.PayForBills += UserPayMoney;	
				VariableChage(payment_bill,UserAct.PayForBills);
			  UserPayMoney = 0 ;
			  WaitTimeInit(&WaitTime); 
		  }
			CurrentPoint = 4 ;
		}break;    					
	  case 4 :   //��ˢ������Ǯ
	  {
	    if( temp != 0)  //ˢ��
	    { 
			  IsCard =1 ;	  //��ʾ����ˢ������
			  UserAct.PayAlready  += temp;
			  UserAct.PayForCards += temp ;
			  temp = 0 ;      
        CurrentPoint = 5 ;
 	    }
		  else
		    CurrentPoint = 5;
		}break;
	  case 5 ://��ʾ�����˶���Ӳ��	
	  {
		  VariableChage(payment_coin,UserAct.PayForCoins);	
		  VariableChage(payment_card,UserAct.PayForCards);
			CurrentPoint = 6;
		}break;		    
	  case 6 : //ͳ��Ǯ��
    {			
	    if(UserAct.PayAlready +UserAct.PayForCards>=UserAct.PayShould)		//Ͷ��Ǯ���ڵ���Ҫ����Ǯ
		  {     
		    CurrentPoint = 9;	            
			  if(UserAct.PayAlready <UserAct.PayShould)
			  {
			    CurrentPoint = 3;
			    return Status_Error ;
			  }  
	   	}
	    else
	    { 
		    CurrentPoint = 3; 
	    } 	
		}break;   				 
    case 7 :  /*���п�֧������Ļ����*/
		{
			UserAct.PayType = '2' ;/* ���п�֧��*/
			temp1 = 0;
			//temp1 = GpbocDeduct(UserAct.PayShould *100);
			temp1 = GpbocDeduct(1);
			if(temp1 == 1)
			{
			  UserAct.PayForCards = UserAct.PayShould ;
			  UserAct.PayAlready += UserAct.PayForCards ;
			  CurrentPoint =6;
			}
	  }break;
	  case 8 :/*����֧ͨ������Ļ����*/
	  {
	    UserAct.PayType = '3' ;/* ����֧ͨ��*/
			temp1 = 0;
			//temp1 = SztDeduct(UserAct.PayShould * 100);
			temp1 = SztDeduct(1);
			if(temp1 == 1)
			{
			  UserAct.PayForCards = UserAct.PayShould ;
        UserAct.PayAlready += UserAct.PayForCards ;
			  CurrentPoint =6;
			}
		}break;
	  case 9 :  //����ɹ��ر����е�����ϵͳ
		{
			UserAct.MoneyBack = UserAct.PayAlready - UserAct.PayShould;	
      OldCoinsCnt= UserAct.MoneyBack ; //���������ִֻ��һ��
			VariableChage(mealout_totle,UserAct.Meal_totoal);	
      NewCoinsCnt= 0; 
      //����Ҫ��ӡ�����ݸ�ֵ����ؽṹ��	
     	Print_Struct.P_Number1st = UserAct.MealCnt_1st;
			Print_Struct.P_Number2nd = UserAct.MealCnt_2nd;
			Print_Struct.P_Number3rd = UserAct.MealCnt_3rd;
			Print_Struct.P_Number4th = UserAct.MealCnt_4th;
			Print_Struct.P_Cost1st   = UserAct.MealCost_1st;
			Print_Struct.P_Cost2nd   = UserAct.MealCost_2nd;
			Print_Struct.P_Cost3rd   = UserAct.MealCost_3rd;
			Print_Struct.P_Cost4th   = UserAct.MealCost_4th;
			Print_Struct.P_paymoney  = UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards ;
			Print_Struct.P_PayShould = UserAct.PayShould ;
			Print_Struct.P_MoneyBack = UserAct.MoneyBack ;
			WaitTime  = 0;
			TIM_Cmd(TIM4, ENABLE);
			CurrentPoint = 0 ;
	       return  Status_OK;
		}
	  default :break;
  }
  return  Status_Action;
}												


uint8_t  CurrentPointer = 0 ;

uint8_t WaitMeal(void)
{
  //static unsigned char Cmd[20]={0x05, 0x31, 0x30, 0x30, 0x31, 0x30 ,0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x34, 0x4D, 0x31, 0x35, 0x43, 0x03, 0x0D ,0x0A};
	switch(CurrentPointer)
	{
		case 0 : /*�����û���ѡ��Ʒ��λ��*/
		{
			//��ֵ��ǰUserAct.MealID
	    if(UserAct.MealCnt_1st>0)
         UserAct.MealID = 0x01;	
      else if(UserAct.MealCnt_2nd>0)
				 UserAct.MealID = 0x02;	
      else if(UserAct.MealCnt_3rd>0)
				 UserAct.MealID = 0x03;	
      else if(UserAct.MealCnt_4th>0)
				 UserAct.MealID = 0x04;	
			else
			{	
         //�����Ʒȫ�����꣬�˳���������				
				 printf("UserAct.MealCnt is none");
			}
			if(FindMeal(DefineMeal)) /**/
			   CurrentPointer= 1;break;	
		}break;
	  case 1 : /* ���Ͳ͵���Ŀ��һ*/
		{			 
	    FloorMealMessageWriteToFlash.FloorMeal[Line].MealCount --; 
	    switch(Column)
		  {
				case 0 :   FloorMealMessageWriteToFlash.FloorMeal[Line].FCount -- ;	  break;
				case 1 :   FloorMealMessageWriteToFlash.FloorMeal[Line].SCount -- ;	  break;
				case 2 :   FloorMealMessageWriteToFlash.FloorMeal[Line].TCount -- ;	  break;
				default:  break;
		  }
			DefineMeal[UserAct.MealID - 1].Position[Line][Column]--;
		  DefineMeal[UserAct.MealID - 1].MealCount--;
			CurrentPointer = 2 ;
		}break ;
    case 2 : /*�����к��е�λ�ã��ȴ���Ӧ*/
		{
       //����[Line][Column]��ֵ�������� �ȴ�ACK	 
			CurrentPointer = 3 ;
	  }break;  
		case 3 :    /*����ȡ������*/
		{
			//��ѯ��е���Ƿ�׼���ã����׼���÷���ȡ������
			//�����ʱ�� ���ش���
		  //���û�в�Ʒ CurrentPointer=0;  else CurrentPointer=3
			CurrentPointer=4;
		}break;
	  case 4 :  	/*������������ȡ��*/
		{ 	
      //�����Ʒ����ȡ�Ϳڲ�������
			//�����Ʒȡ���� �����ӳ�����������ϴ�  
			CurrentPointer=5;
	  }break;			    
    case 5:     /*���û����ݽ��м�һ*/
		{
	    if(UserAct.MealID == 0x01)
			  UserAct.MealCnt_1st--;
      else if(UserAct.MealID == 0x02)
				UserAct.MealCnt_2nd--;
      else if(UserAct.MealID == 0x03)
				UserAct.MealCnt_3rd--; 	
      else if(UserAct.MealID == 0x04)
				UserAct.MealCnt_4th--; 	
      UserAct.Meal_takeout++;//ȡ�����ݼ�һ
      VariableChage(mealout_already,UserAct.Meal_takeout++);	//UI��ʾ��һ	
			CurrentPointer= 0;
			return 0;
		}
		default:break;
	}
	return Status_Action;
}   

  /*******************************************************************************
 * ��������:CloseCashSystem                                                                     
 * ��    ��:�ر��ֽ����                                                        
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
bool CloseCashSystem(void)
{
	uint8_t cnt_t=20,money=0;
  CloseCoinMachine();			    //�ر�Ͷ�һ�	
	DisableBills();             //���ò��ر�ֽ�һ�
	do
  {
		cnt_t--;
		delay_ms(10);
    ReadBill();		
	}  // ��ʱ
	while((cnt_t>0)&&(DisableBillFlag== NACK)); //����ʱ���߽��յ�ACK��
	if(DisableBillFlag== ACK) 
	{
		DisableBillFlag= NACK;
			return true;
	}
	else 
	{
		DisableBillFlag= NACK;
			return false;
	}
}
bool OpenCashSystem(void)
{
	int cnt_t=20;	
	OpenCoinMachine();    //��Ͷ�һ�	
	SetBills();           //���ò���ֽ�һ�
	do
  {
		cnt_t--;
		ReadBill();
		delay_ms(10);	
	}
	while((cnt_t>=0)&&(EnableBillFlag== NACK));//
	if(EnableBillFlag== ACK) 
	{
		EnableBillFlag= NACK;
			return true;
	}
	else 
	{
		EnableBillFlag= NACK;
			return false;
	}
}

  /*******************************************************************************
 * ��������:ClearingFuntion                                                                     
 * ��    ��:�������� ���������Ƿ��ͳɹ�������û�з��ͳɹ����������͡�ȫ��������Ϻ���㡣                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void ClearingFuntion(void)
{
	 
	RTC_TimeShow();//���ʱ��
	if(TimeDate.Hours == 5 || TimeDate.Hours == 19)
	{
	  // �������� ��
	      ClearingFun();	  //��̨��������
		 // BankFlashCard_Upload(); //��������	 ������ǩ
	}		 		 
}

 /*
  PVD ---- �͵�ѹ���                ��ռ���ȼ�  0      �����ȼ� 0 		���ڱ���sd��
  Ӳ�һ�               �ⲿ�ж�5                 0                1
  ֽ�һ�               ����4                     0                2
  IC��                 ����2                     0                3  
  ��Ļ                 �����ж�3                 0                4
                       CANͨ��                   0                5
  ��ӡ                 ����1                     0                6
  �˱ҵ���ʱ           ��ʱ��4                   1                0
  ��ʱ����ʱ60s        ��ʱ��3                   1                1
  �������             ��ʱ��2                   1                2
  �������             �ⲿ�ж�4                 1                3
  ��ʱ�ɼ��¶Ȳɼ�     ��ʱ��7                   1                4
 */
  /*******************************************************************************
 * ��������:hardfawreInit                                                                     
 * ��    ��:Ӳ����ʼ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 

void hardfawreInit(void)
{
  uint8_t i, j, k;
  Uart6_Configuration();
//��ʼ�����λ�����ݽṹ��
  for(i = 0; i < 4; i++)
	{
		DefineMeal[i].MealPrice = 0;
		DefineMeal[i].MealCount = 0;
		for(j = 0; j < 15; j++)
		{
			for(k = 0; k < 3; k++)
			{
				DefineMeal[i].Position[j][k] = 0; 
			}
		}	
	}
 	 UserAct.PayForCoins     = 0;
	 UserAct.PayForBills     = 0;
	 UserAct.PayForCards     = 0;
	 UserAct.PayAlready      = 0;
   SystemInit();
//	 delay_ms(30000); //�ϵ�ȴ�·��������
	 Uart4_Configuration();     //ֽ�һ����ڳ�ʼ�� 
	 Uart1_Configuration();	    //��ӡ�����ڳ�ʼ��
	 Uart3_Configuration();	    // ��������ʼ��
	 Uart2_Configuration();	    //����ͨ������������
	 Uart5_Configuration();		//���紮�ڳ�ʼ��
	 Uart6_Configuration();
//	 TIM2_Init();		        //���
//	 TIM3_Init();		        //���ڶ�ʱ������ʱ
//	 TIM4_Init();		        //����
	 TIM5_Init();		        //����ʱ�˱�
//	 TIM7_Init();				  //���ڶ�ʱ�ɼ��¶�
   InitCoins();		        //Ͷ�һ���ʼ��
   InitMiniGPIO() ;		   //�˱���ʼ��
	 PageChange(Logo_interface); //��ʾlogo
	 PageChange(Logo_interface); //�ظ�һ�ξͿ��Գɹ�
	 InitVoice()  ;             //������ʼ��
	 MyRTC_Init();              //RTC��ʼ��
	 SPI_FLASH_Init();          //Flash��ʼ��
	 SPI_FLASH_Init();          //�ظ���ʼ������
   SPI_FLASH_BufferRead(FloorMealMessageWriteToFlash.FlashBuffer, SPI_FLASH_Sector0 , FloorMealNum*6);//��ȡ����Ĳ�Ʒ
   //ReadCoins();//��ȡ�ж���Ӳ��	 
	 VariableChage(coins_in,Coins_totoal);//��ʾ����Ӳ����
	 for(i=0;i<90;i++)
	 {
	   if(FloorMealMessageWriteToFlash.FlashBuffer[i] == 0xff)
	   FloorMealMessageWriteToFlash.FlashBuffer[i]    = 0 ;
	 }
	 WriteMeal();  //д���Ʒ����
	 StatisticsTotal(); //����ĳ�����Ҫʹ��  	
}														 

