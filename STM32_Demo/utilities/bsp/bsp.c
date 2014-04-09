
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
 uint8_t FindMeal(MealAttribute *DefineMeal) 
 {
     for(Line = 0; Line < FloorMealNum; Line++)
		{
			for(Column = 0; Column < 3; Column++)
			{
			  if(DefineMeal[UserAct.MealID - 1].Position[Line][Column] > 0)	
			  return 0;		
			}
			if(Column < 3)
			{
				return 0 ;
			}
		}
		return 1 ;
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
		  SPRT(UserAct.MealID-1);
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
uint8_t CurrentPoint = 0 ; 
uint8_t UserPayMoney =0 ;
unsigned char  WaitPayMoney(void)
{
    uint8_t PicNumber;
	uint8_t temp = 0 ;
	uint8_t temp1 ;
	unsigned char times = 10 ;
	unsigned char Status = 0;
  switch(CurrentPoint)
	{
	case 0 :	//�����Ǹ�λ�õĲ�
	     {	  
	       if(FindMeal(DefineMeal) == 0)
	       {
			     CurrentPoint = 1 ; 
		     }
		     else 
		       return Status_Error	;
 	     }
          break ;
	case 1 : 
	         /*��ʾ���ʽ���ֽ����п�������ͨ*/
			  if(WaitTime<56)
			  {
			    CurrentPoint = 3;
			    /*֧����ʽ*/			 
			    UserAct.PayType = '1';/* �ֽ�֧��*/
			  }break;    		
	case 2 :  //��ƽ���ʲô����Ǯ
	       /*����ѡ�񸶿ʽ����*/
			    CurrentPoint = 3 ;
				  UserAct.PayType = '1' ;/* �ֽ�֧��*/
		   break;	         
	case 3 :  //��Ǯ	  
	       	//UserPayMoney = ReadBills();			 
		      if(UserPayMoney !=0 )	   //��ʾ�յ���Ǯ
		      {
			      UserAct.PayAlready  += UserPayMoney;
			      UserAct.PayForBills += UserPayMoney;	
			   	  UserPayMoney = 0 ;
			   	  WaitTimeInit(&WaitTime);
//				    DisplayBills(UserAct.PayForBills);
			      CurrentPoint = 5 ;
		      }
        break;					
	case 4 :   //��ˢ������Ǯ
	        if( temp != 0)  //ˢ��
	        { 
			      IsCard =1 ;	  //��ʾ����ˢ������
			      UserAct.PayAlready  += temp;
			      UserAct.PayForCards += temp ;
			      temp = 0 ;      
//			      DisplayCards(UserAct.PayForCards);
        	  CurrentPoint = 5 ;
 	        }
		      else
		        CurrentPoint = 5;
				break;
	case 5 ://��ʾ�����˶���Ӳ��		    
		       CurrentPoint = 6 ;
	      break;
	case 6 :  
         /*��ֹ�������Ļ*/
	       if(UserAct.PayAlready +UserAct.PayForCards>=UserAct.PayShould)		//Ͷ��Ǯ���ڵ���Ҫ����Ǯ
		     {     
		       CurrentPoint = 9;	                //����	 
			     if(UserAct.PayAlready <UserAct.PayShould)
			     {
			       CurrentPoint = 3;
			       return Status_Error ;
			     } 
//		 	   CloseTIM3();
//         CloseMoneySystem();			 
	   	  }
	      else
	      { 	 /*�ж��Ƿ��յ���Ǯ�ı�־λ*/
//		      if(Rev_Money_Flag == 1)
//			    {
//			      Rev_Money_Flag =0 ;
//			      /*��Ǯ�Ž�Ǯ��*/
//		        Polls();/*�������һ��Ǯ*/
//			    }
		      CurrentPoint = 3; 
	      } 	
	     	  break;   				 
    case 7 :  /*���п�֧��*/
			  UserAct.PayType = '2' ;/* �ֽ�֧��*/
			  temp1 = 0;
			  //temp1 = GpbocDeduct(UserAct.PayShould *100);
			  //temp1 = GpbocDeduct(1);
			  if(temp1 == 1)
			  {
			    UserAct.PayForCards = UserAct.PayShould ;
			    UserAct.PayAlready += UserAct.PayForCards ;
			    CurrentPoint =6;
			  }
	          break;
	 case 8 :/*����֧ͨ��*/
	      UserAct.PayType = '3' ;/* �ֽ�֧��*/
			  temp1 = 0;
			  //temp1 = SztDeduct(UserAct.PayShould * 100);
			  //temp1 = SztDeduct(1);
			  if(temp1 == 1)
			  {
			    UserAct.PayForCards = UserAct.PayShould ;
			    UserAct.PayAlready += UserAct.PayForCards ;
			    CurrentPoint =6;
			  }
	          break;
	  case 9 :  //�ر����е�����ϵͳ
			  UserAct.MoneyBack = UserAct.PayAlready - UserAct.PayShould;	
			  Print_Struct.P_ID        = UserAct.MealID ;
			  Print_Struct.P_paymoney  = UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards ;
			  Print_Struct.P_PayShould = UserAct.PayShould ;
			  Print_Struct.P_MoneyBack =	UserAct.MoneyBack ;
	
			 WaitTime  = 0;
			 //DisplayWaitTime1(WaitTime);//�ȴ�ʱ��
			 /*����ʱ*/
			 TIM_Cmd(TIM4, ENABLE);
			 if(UserAct.MoneyBack > 0)
			 {
			   //OpenCoins();    //�ұ�		
			 }
			 CurrentPoint = 0 ;
	       return  Status_OK;
	  default :break;
  }
  return  Status_Action;
}												




unsigned char  Record_buffer[1024] = {0} ;  //Ϊʲô����ֲ�����û�����أ�
uint8_t WaitMeal(void)
{
   
	uint8_t  i ;
	static uint8_t  CurrentPointer = 0 ;
  static unsigned char Cmd[20]={0x05, 0x31, 0x30, 0x30, 0x31, 0x30 ,0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x34, 0x4D, 0x31, 0x35, 0x43, 0x03, 0x0D ,0x0A};
	 /*�ж��Ƿ��ӡСƱ*/ 			
  //PrintTickFun(&UserAct.PrintTick);
	switch(CurrentPointer)
	{
	  case 0 :  
	     /* ���Ͳ͵���Ŀ��һ*/
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
			   CurrentPointer = 1 ;
			 break ;
    case 1 :   /*�����и��С���ʱ�ȴ����ȴ���е�ֵ��ƶ�λ��*/
	      switch(Column)
				{
				  case 0 :   Column =3;	  break;
				  case 1 :   Column =2;	  break;
				  case 2 :   Column =1;	  break;
				  default :break;
				}
	      Cmd[16]=0x40 + Column ;
		 		Cmd[15]=0x30 + (Line+1) %10 ;
				Cmd[14]=0x30 + (Line+1) /10 ;
				for(i=0;i<20;i++)
				{
				  USART_SendData(USART6,Cmd[i] );//����1����һ���ַ�
 	        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);//�ȴ��������
				  delay_ms(10);
				}
				CurrentPointer = 2 ;
				TIM_Cmd(TIM2, ENABLE);
				MealArr(UserAct.MealID);
		      /*����ȡ�����ݸ�������*/
		    memset(Record_buffer,0,1024);    
				Record_buffer[1012] = 'L' ;
			  Record_buffer[1013] = 'e' ; 
				Record_buffer[1014] = 'n' ;
				Record_buffer[1015] = 'g' ;  
				Record_buffer[1016] = 'h' ;
				Record_buffer[1017] = 't' ;
				Record_buffer[1018] = ':' ; 
				if(TakeMealsFun(Record_buffer) == 0x01) //��ʾ����ʧ��
			  {
				  Record_buffer[1022] = 'N' ;
				  Record_buffer[1023] = 'O' ; /*��ʾ����ʧ�ܣ�д���־λ�����´η���*/  
			  }
				else 
			  {  
					Record_buffer[1021] = 'Y' ;
				  Record_buffer[1022] = 'e' ;
					Record_buffer[1023] = 's' ; /*��ʾ����ʧ�ܣ�д���־λ�����´η���*/  
        }
		  		WriteDatatoSD(Record_buffer);
		  break ;
	 case 2 :   	  
	       /*��ʱ3�룬Ȼ�󿪿��أ��û�������*/	             
//	      if (TimerFlag > 4 )
//			  {
//			    TIM_Cmd(TIM2, DISABLE); 
//			    TimerFlag = 0;				 
//          CurrentPointer = 3 ;
//				  GPIO_SetBits(GPIOC, GPIO_Pin_13);
//				  TIM_Cmd(TIM2, ENABLE);  	
//				}
			break;
	 case 3 :   /*�Ͽ����Ϳ��أ�*/		
//	      if (TimerFlag > 2 )
//			  {
//			    TIM_Cmd(TIM2, DISABLE);
//			    TimerFlag = 0;				  
//          CurrentPointer = 4 ;
//				  GPIO_ResetBits(GPIOC, GPIO_Pin_13);	
//				  TIM_Cmd(TIM4, DISABLE);
//				  DisplayWaitTime1(WaitTime);
//				  PlayMusic(VOICE_3);		//��3.��ȡ��
//				  TIM_Cmd(TIM2, ENABLE);  
//				}			 
			break;
	 case 4 :  	/*������������ȡ��*/
	           	
//	      if (TimerFlag > 2 )
//			  {
//			    Polls();/*�������һ��Ǯ*/
//			    TimerFlag = 0;
//				  TIM_Cmd(TIM2, DISABLE);  
//          CurrentPointer = 5 ;
//				  if(IsCard == 1)
//				  {
//					  IsCard = 0 ;   //���� 
//				  }
//				}   
			break;
	 case 5 :  /*��ʾ���*/	
	      WriteMeal();   /*���浱ǰ�ķ���*/
				//PlayMusic(VOICE_4);	    //4.��ӭ�´ι���
				CurrentPointer = 6 ;			   
			break;
	 case 6 :	/*��ӭ�´ι���*/	 
	 	  	 
				CurrentPointer = 7;				
			break;
	 case 7 : 	/*�ص�������*/  							  
				UserAct.MoneyBack   = 0 ;
				UserAct.PayAlready  = 0;
		    UserAct.PayForBills = 0;
		    UserAct.PayForCoins = 0;
		    UserAct.PayForCards = 0;
//				TimerFlag = 0 ;
				CurrentPointer = 0 ;
				return 0 ;
	 case 8 :  break;		                
   default : break ;	 	
  }      
  return 1 ;
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

    /*******************************************************************************
 * ��������:TempHandler                                                                     
 * ��    ��:�¶ȴ�����                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
 void  TempHandler(void)
 {
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

    for(i = 0; i < 4; i++)
	{
	
		DefineMeal[i].MealPrice = 0;
		DefineMeal[i].MealCount = 0;
		for(j = 0; j < 15; j++)
		{
			for(k = 0; k < 3; k++)
			{
				DefineMeal[i].Position[j][k] = 0; /**/
			}
		}
		
	}
 	 UserAct.PayForCoins     = 0;
	 UserAct.PayForBills     = 0;
	 UserAct.PayForCards     = 0;
	 UserAct.PayAlready      = 0;
	 IsCard            = 0;
//	 LCD_En_Temp();

   SystemInit();
//	 delay_ms(30000);
//	 LED_Init();
//     InitBills();               //ֽ�һ���ʼ�� 
// 	 Uart1_Configuration();	    //����1��ʼ��
	 Uart3_Configuration();	    // ��������ʼ��
//	 Uart2_Configuration();	    //����ͨ������������
//	 Uart5_Configuration();		//���紮�ڳ�ʼ��
//	 Uart6_Configuration();
//	 //InitCards();			    //IC����ʼ��
//	 TIM2_Init();		        //���
//	 TIM3_Init();		        //���ڶ�ʱ������ʱ
//	 TIM4_Init();		        //����
//	 TIM5_Init();		        //����ʱ�˱�
//	 TIM7_Init();				//���ڶ�ʱ�ɼ��¶�
//	 InitCoins();		        //Ӳ�ҳ�ʼ��
//	 InitCoinsHopper() ;		 //�˱���ʼ��
////	 InitCoinMachine();		    //Ӳ�һ���ʼ��
////	 CAN_InitConfig();	        //can��ʼ��
// 	 PictrueDisplay(0);        //��ʾLOGO
//	 InitVoice()  ;             //������ʼ��
	 MyRTC_Init();              //RTC��ʼ��
	 SPI_FLASH_Init();          //Flash��ʼ��
//	 LED_Init();
  // SPI_FLASH_SectorErase(SPI_FLASH_Sector0);
	 ReadMeal();
	 for(i=0;i<90;i++)
	 {
	   if(FloorMealMessageWriteToFlash.FlashBuffer[i] == 0xff)
	   FloorMealMessageWriteToFlash.FlashBuffer[i]    = 0 ;
	 }
	 WriteMeal();
	 StatisticsTotal();   	
// PictrueDisplay(HOME_PAGE);	       //��ʾ�˵�ҳ��
// DS18B20_DEMO();
// TemperatureDisplay(Temperature);  //��ʾ�¶�	  
// DisplayMealCount();			   //��ʾ�͵���Ŀ 
 //delay_ms(30000);
	 
}														 

