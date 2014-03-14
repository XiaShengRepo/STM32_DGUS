#include "bsp.h"



uint16_t   Balance;
//uint8_t    CANStatus;
static     uint8_t IsCard	;
uint8_t    DisplayTimes	 ;
uint8_t    Line = 0,Column = 0 ;


void OpenMoneySystem(void)
{
 //   OpenCards();   //�򿪿�
//	OpenCoinMachine();   //����Ӳ�һ�
}


void CloseMoneySystem(void)
{
  //CloseCards();
//	CloseCoinMachine();
//    NoAcceptMoneyFun();

} 
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
 uint8_t FindMeal(MealAttribute *DefineMeal) 
 {
     for(Line = 0; Line < FloorMealNum; Line++)
		{
			for(Column = 0; Column < 3; Column++)
			{
//			  printf("UserAct.MealID=%d\r\n",UserAct.MealID);
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
// /*******************************************************************************
// * ��������:CanRecvCmd                                                                     
// * ��    ��:����can�������Ƿ���ȷ                                                                  
// *                                                                               
// * ��    ��:��                                                                     
// * ��    ��:��                                                                     
// * ��    ��:void                                                               
// * �޸�����:2013��8��28��                                                                    
// *******************************************************************************/
// uint8_t CanRecvCmd(uint8_t p)
// {
//        uint8_t i = 0 ;
//  		while(i < 100)
//		{
//			if(CANStatus == p)
//			{
//				return 0;
//			}
//			else
//			{
//				i++;
//			    delay_ms(100);
//			}
//		}
//		if(i == 100)
//		{
//			PictrueDisplay(47);
//			DisplayErrorMealPrice(300);
//			return 1 ;
//		}
//	  return 0;
// }

 /*******************************************************************************
 * ��������:MealArr                                                                     
 * ��    ��: ��Ʒ������                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void MealArr(unsigned char index)
{
      int  MoneyBack = 0 ,PayBill = 0;
	  switch(index)
	  {
     
      case 1 :	/*�����Ʒ��ID*/	 
 		     	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x20;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;

 

				 break ;
      case 2 :	/*�����Ʒ��ID*/
 		        CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x21;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
      case 3 :	/*�����Ʒ��ID*/
 		  	    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x22;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x15 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
       case 4 :	/*�����Ʒ��ID*/
 		  	    CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x23;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x20 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
        case 5 :	/*�����Ʒ��ID*/
 		        CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x24;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x20 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
      case 6 :	/*�����Ʒ��ID*/
 		        CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x25;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x20 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
      case 7 :	/*�����Ʒ��ID*/
 		        CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x26;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x25 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
      case 8 :	/*�����Ʒ��ID*/
 		     	CustomerSel.MealID[0]		   =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x27;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x25 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
      case 9 :	/*�����Ʒ��ID*/
 		       CustomerSel.MealID[0]		       =   0x10;
				CustomerSel.MealID[1]		   =   0x00;
				CustomerSel.MealID[2]		   =   0x00;
				CustomerSel.MealID[3]		   =   0x28;
				/*�����Ʒ�ļ۸�*/
				CustomerSel.MealPrice[0]      =    0x00 ;
				CustomerSel.MealPrice[1]      =	   0x00 ;
				CustomerSel.MealPrice[2]      =	   0x00	;
				CustomerSel.MealPrice[3]      =	   0x00 ;
				CustomerSel.MealPrice[4]      =	   0x25 ;
				CustomerSel.MealPrice[5]      =	   0x00 ;
				 break ;
	default : break ;

	}			/*�����Ʒ������*/
				CustomerSel.MealNo         =  Print_Struct.P_Number;
				/*�����Ʒ������*/
                CustomerSel.PayType        =  UserAct.PayType; //	UserAct.PayType  ;
				if(CustomerSel.PayType == '1')	/*������ֽ���*/
				{
				MoneyBack  =Print_Struct.P_MoneyBack *100 ;  /*����10��*/
			//	printf("UserAct.MoneyBack=%d\r\n",UserAct.MoneyBack);
				/*ʮ����ת����16*/
				CustomerSel.Change[0]      =	     MoneyBack / 10000000000 %100;
				CustomerSel.Change[0]      =         CustomerSel.Change[0]/10 *16 +CustomerSel.Change[0]%10 ;   
				CustomerSel.Change[1]      =	     MoneyBack / 100000000 %100;
				CustomerSel.Change[1]      =         CustomerSel.Change[1]/10 *16 +CustomerSel.Change[1]%10 ;                              
				CustomerSel.Change[2]      =	     MoneyBack / 1000000 %100;
				CustomerSel.Change[2]      =         CustomerSel.Change[2]/10 *16 +CustomerSel.Change[2]%10 ;
				CustomerSel.Change[3]      =	     MoneyBack / 10000 %100;
				CustomerSel.Change[3]      =         CustomerSel.Change[3]/10 *16 +CustomerSel.Change[3]%10 ;
				CustomerSel.Change[4]      =	     MoneyBack / 100 %100;
				CustomerSel.Change[4]      =         CustomerSel.Change[4]/10 *16 +CustomerSel.Change[4]%10 ;
				CustomerSel.Change[5]      =	     MoneyBack % 100 ;
				CustomerSel.Change[5]      =         CustomerSel.Change[5]/10 *16 +CustomerSel.Change[5]%10 ;
				MoneyBack = 0 ;
			 	}
				/*�����Ʒ��ʣ�����*/
				CustomerSel.RemainMealNum[0]  =	 (DefineMeal[index-1].MealCount>>8)&0xff;
				CustomerSel.RemainMealNum[1]  =	  DefineMeal[index-1].MealCount &0xff;
//				printf("CustomerSel.RemainMealNum[0]  =%d\r\n",CustomerSel.RemainMealNum[0]);
//				printf("CustomerSel.RemainMealNum[1]  =%d\r\n",CustomerSel.RemainMealNum[1]);
				/*���������*/
				CustomerSel.MealName	   =  	   index ;
				/*���˶����ֽ�*/
			//	 PayBill  =UserAct.PayForBills +*100 ;  /*����10��*/
		        PayBill  =	UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards ;
	 			 /*֧���˶���Ǯ*/
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
//	    	 printf(" Print_Struct.P_Number  =%d\r\n",Print_Struct.P_Number );		 //ID
//		     printf(" Print_Struct.P_ID =%d\r\n",Print_Struct.P_ID);   //����
//			 printf(" UserAct.MealPrice =%d\r\n",UserAct.MealPrice);	//����
//			 printf(" Print_Struct.P_paymoney =%d\r\n",Print_Struct.P_paymoney);	//�ܼ�
//			 printf(" Print_Struct.P_PayShould =%d\r\n",Print_Struct.P_PayShould);  //Ͷ�˶���ֽ��
//			 printf(" Print_Struct.P_MoneyBack =%d\r\n",Print_Struct.P_MoneyBack);   //Ͷ�˶���Ӳ��
		 TIM_Cmd(TIM4, DISABLE);
		 PictrueDisplay((UserAct.MealID-1)*7+9);	           //��ʾͼƬ
		 /*��ʾĬ�ϵ�һ��*/
		 /*��ʾ�ͻ�Ͷ�ҵĽ��*/
		 DisPlayChar(Display_Bills,sizeof(Display_Bills),UserAct.PayForBills);
//		 printf(" UserAct.PayForBills =%d\r\n",UserAct.PayForBills);  //Ͷ�˶���ֽ��
	//	 DisplayBills(UserAct.PayForBills);	       //��ʾֽ��
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		 DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);
	//	 DisplayCoins(UserAct.PayForCoins);	       //��ʾӲ��
		 /*��ʾ�û�ˢ�����*/
		  DisPlayChar(Display_Cards,sizeof(Display_Cards),UserAct.PayForCards);
		 /*��ʾ�ܽ��*/

		 DisplayMealNum1(Print_Struct.P_Number);

		 DisplayMealPrice1(UserAct.PayShould); 
	 	*PrintTickFlag = 0 ;
		/*��ӡСƱ�ĺ���*/
	
		 SPRT(UserAct.MealID-1);
	    TIM_Cmd(TIM4, ENABLE);
	  }	 

	  if(*PrintTickFlag == 0x02 )
	  {
	     TIM_Cmd(TIM4, DISABLE);
	     PictrueDisplay((UserAct.MealID-1)*7+9);	    //��ʾͼƬ
			 /*��ʾĬ�ϵ�һ��*/
		 /*��ʾ�ͻ�Ͷ�ҵĽ��*/
     	 DisPlayChar(Display_Bills,sizeof(Display_Bills),UserAct.PayForBills);
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		 DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);
		 /*��ʾ�û�ˢ�����*/
		  DisPlayChar(Display_Cards,sizeof(Display_Cards),UserAct.PayForCards);
		 /*��ʾ�ܽ��*/
	 	 DisplayMealNum1(Print_Struct.P_Number);
		 DisplayMealPrice1(UserAct.PayShould); 
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
//		     printf("Line = %d,Column =%d\r\n",Line,Column)	  ;   
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
			    /*����ѡ�񸶿ʽ����*/		 
				PictrueDisplay(5+(UserAct.MealID-1)*7);
				DisPlayChar(Display_Bills,sizeof(Display_Bills),UserAct.PayForBills);

		        /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		        DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);
		        /*��ʾ�û�ˢ�����*/
		        DisPlayChar(Display_Cards,sizeof(Display_Cards),UserAct.PayForCards);

				/*��ʾ�ܽ��*/
			    DisplayMealPrice(UserAct.PayShould);      //��ʾ�ܼ�� 

			    DisplayMealNum(UserAct.MealCount); //��ʾ������  ��ʼĬ��Ϊһ��

			    CurrentPoint = 3 ;

			   /*֧����ʽ*/			 
			   UserAct.PayType = '1' ;/* �ֽ�֧��*/
			  }

           	break;	
	case 2 :  //��ƽ���ʲô����Ǯ
	              /*����ѡ�񸶿ʽ����*/
	            PictrueDisplay(5+(UserAct.MealID-1)*7);
		        DisPlayChar(Display_Bills,sizeof(Display_Bills),UserAct.PayForBills);
		  
		       /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		        DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);
		       /*��ʾ�û�ˢ�����*/
		        DisPlayChar(Display_Cards,sizeof(Display_Cards),UserAct.PayForCards);
				/*��ʾ�ܽ��*/
				DisplayMealPrice(UserAct.PayShould);      //��ʾ�ܼ�� 

				DisplayMealNum(UserAct.MealCount); //��ʾ������  ��ʼĬ��Ϊһ��
			    CurrentPoint = 3 ;
				UserAct.PayType = '1' ;/* �ֽ�֧��*/
		   break;	         
	case 3 :  //��Ǯ	  

	       	  UserPayMoney = ReadBills();			 
		      if(UserPayMoney !=0 )	   //��ʾ�յ���Ǯ
		    {
			    UserAct.PayAlready  += UserPayMoney;
			    UserAct.PayForBills += UserPayMoney;	
			   	UserPayMoney = 0 ;
			   	WaitTimeInit(&WaitTime);
				DisplayBills(UserAct.PayForBills);
			    CurrentPoint = 5 ;
		    }
		   
	case 4 :   //��ˢ������Ǯ
	     	   //temp = 	ChangeforMeal();
	           if( temp != 0)  //ˢ��
	        { 
			   IsCard =1 ;	  //��ʾ����ˢ������
			   UserAct.PayAlready  += temp;
			   UserAct.PayForCards += temp ;
			   temp = 0 ;
//			   printf(" UserAct.PayForCards =%d",UserAct.PayForCards);	           
			   DisplayCards(UserAct.PayForCards);
        	   CurrentPoint = 5 ;
 	        }
		    else
		       CurrentPoint = 5;
	case 5 ://��ʾ�����˶���Ӳ��		    
		 	  //DisplayCoins(UserAct.PayForCoins);	
			   DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);
		       CurrentPoint = 6 ;
	case 6 :  
             /*��ֹ�������Ļ*/
          	  _LCD_Disable_RxInit();
	       if(UserAct.PayAlready +UserAct.PayForCards>=UserAct.PayShould)		//Ͷ��Ǯ���ڵ���Ҫ����Ǯ
		  {     
		    CurrentPoint = 9;	                //����	 
			if(UserAct.PayAlready <UserAct.PayShould)
			{
			  CurrentPoint = 3;
			  return Status_Error ;
			} 
		 	CloseTIM3();
			/*��ʾ�����У���ӡ����Ľ���*/
			PictrueDisplay(7+(UserAct.MealID-1)*7);	  
            CloseMoneySystem();			 
	   	  }
	  else
	    { 	 /*�ж��Ƿ��յ���Ǯ�ı�־λ*/
		     if(Rev_Money_Flag == 1)
			  {
			   Rev_Money_Flag =0 ;
			   /*��Ǯ�Ž�Ǯ��*/
		      Polls();/*�������һ��Ǯ*/
			  }
			  _LCD_Enable_RxInit();
		      CurrentPoint = 3; 
		
	    } 	
	     	  break;   				 
    case 7 :  /*���п�֧��*/
			  UserAct.PayType = '2' ;/* �ֽ�֧��*/
			  temp1 = 0;
			  //temp1 = GpbocDeduct(UserAct.PayShould *100);
			  temp1 = GpbocDeduct(1);
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
			  temp1 = SztDeduct(1);
			  if(temp1 == 1)
			  {
			    UserAct.PayForCards = UserAct.PayShould ;
			    UserAct.PayAlready += UserAct.PayForCards ;
			    CurrentPoint =6;
			  }
	          break;
	case 9 :  //�ر����е�����ϵͳ
			BillActionFlag = 1;		
			UserAct.MoneyBack = UserAct.PayAlready - UserAct.PayShould;	
			PlayMusic(VOICE_2);
			DisplayMealNum1(UserAct.MealCount);
			DisplayMealPrice1(UserAct.PayShould); 
		    DisPlayChar(Display_Bills,sizeof(Display_Bills),UserAct.PayForBills);
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		    DisPlayChar(Display_Coins,sizeof(Display_Coins),UserAct.PayForCoins);

		 /*��ʾ�û�ˢ�����*/
		    DisPlayChar(Display_Cards,sizeof(Display_Cards),UserAct.PayForCards);
//			DisplayBills(UserAct.PayForBills);
//			DisplayCoins(UserAct.PayForCoins);
//			DisplayCards(UserAct.PayForCards);	
//			printf(" UserAct.MealID =%d\r\n",UserAct.MealID);		 //ID
//		    printf(" UserAct.MealCount =%d\r\n",UserAct.MealCount);   //����
//			printf(" UserAct.MealPrice =%d\r\n",UserAct.MealPrice);	//����
//			printf(" UserAct.PayShould =%d\r\n",UserAct.PayShould);	//�ܼ�
//			printf(" UserAct.PayForBills =%d\r\n",UserAct.PayForBills);  //Ͷ�˶���ֽ��
//			printf(" UserAct.PayForCoins =%d\r\n",UserAct.PayForCoins);   //Ͷ�˶���Ӳ��
//			printf(" UserAct.PayForCards =%d\r\n",UserAct.PayForCards);  //��֧���˶���Ǯ
//		    printf(" UserAct.PayAlready =%d\r\n",UserAct.PayAlready);	   //�ѽ����˶���Ǯ
 //	     	printf(" UserAct.MoneyBack  =%d\r\n",UserAct.MoneyBack );		//�һض���Ǯ
//			printf(" UserAct.IC_Card_Balance =%d\r\n",UserAct.IC_Card_Balance);	   //��ֵ
//			printf(" UserAct.RechxargeCounter =%d\r\n",UserAct.RechxargeCounter);
            /*��ӡ��Ϣ*/
		     Print_Struct.P_Number    = UserAct.MealCount ;
			 Print_Struct.P_ID        = UserAct.MealID ;
			 Print_Struct.P_paymoney  = UserAct.PayForBills +	UserAct.PayForCoins +UserAct.PayForCards ;
			 Print_Struct.P_PayShould = UserAct.PayShould ;
			 Print_Struct.P_MoneyBack =	UserAct.MoneyBack ;


			WaitTime  = 0;
			DisplayWaitTime1(WaitTime);//�ȴ�ʱ��
		     _LCD_Enable_RxInit();
			/*����ʱ*/
			TIM_Cmd(TIM4, ENABLE);
			if(UserAct.MoneyBack > 0)
			{
			   // CoinFlag = 1;    //ϵͳÿ���������������ܽ��жϲ�֪��Ϊʲô
			    OpenCoins();    //�ұ�
				CoinFlag = 1; 
				
			}
			LedCmd = 0 ;
			CurrentPoint = 0 ;
		

			return  Status_OK;
	case 10 :  //����Ƿ�Ҫ�������
	{
	  

	}
	default :break;
   }

  	return  Status_Action;
 }												



/*										  14 15 16
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 31 43 03 0D 0A        1c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 31 42 03 0D 0A		   1b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 31 41 03 0D 0A        1a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 32 43 03 0D 0A        2c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 32 42 03 0D 0A		   2b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 32 41 03 0D 0A        2a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 33 43 03 0D 0A        3c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 33 42 03 0D 0A		   3b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 33 41 03 0D 0A        3a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 34 43 03 0D 0A        4c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 34 42 03 0D 0A		   4b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 34 41 03 0D 0A        4a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 35 43 03 0D 0A        5c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 35 42 03 0D 0A		   5b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 35 41 03 0D 0A        5a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 36 43 03 0D 0A        6c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 36 42 03 0D 0A		   6b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 36 41 03 0D 0A        6a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 37 43 03 0D 0A        7c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 37 42 03 0D 0A		   7b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 37 41 03 0D 0A        7a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 38 43 03 0D 0A        8c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 38 42 03 0D 0A		   8b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 38 41 03 0D 0A        8a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 39 43 03 0D 0A        9c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 39 42 03 0D 0A		   9b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 39 41 03 0D 0A        9a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 30 43 03 0D 0A        10c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 30 42 03 0D 0A		   10b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 30 41 03 0D 0A        10a


05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 31 43 03 0D 0A        11c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 31 42 03 0D 0A		   11b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 31 41 03 0D 0A        11a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 32 43 03 0D 0A        12c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 32 42 03 0D 0A		   12b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 32 41 03 0D 0A        12a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 33 43 03 0D 0A        13c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 33 42 03 0D 0A		   13b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 33 41 03 0D 0A        13a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 34 43 03 0D 0A        14c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 34 42 03 0D 0A		   14b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 34 41 03 0D 0A        14a

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 35 43 03 0D 0A        15c
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 35 42 03 0D 0A		   15b
05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 35 41 03 0D 0A        15a

100100640004M01C   
1C

05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 31 41 03 0D 0A 
100100640004M01A                         


05 31 30 30 31 30 30 36 34 30 30 30 34 4D 30 32 43 03 0D 0A 
 100100640004M02C                 


05 31 30 30 31 30 30 36 34 30 30 30 34 4D 31 35 43 03 0D 0A 
100100640004M15C
*/
unsigned char  Record_buffer[1024] = {0} ;  //Ϊʲô����ֲ�����û�����أ�
uint8_t WaitMeal(void)
{
   
	uint8_t  i ;
	static uint8_t  CurrentPointer = 0 ;
  static unsigned char Cmd[20]={0x05, 0x31, 0x30, 0x30, 0x31, 0x30 ,0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x34, 0x4D, 0x31, 0x35, 0x43, 0x03, 0x0D ,0x0A};
	 /*�ж��Ƿ��ӡСƱ*/ 			
    PrintTickFun(&UserAct.PrintTick);
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
				 default :break;
				}
				 UserAct.MealCount--;
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
	            if (TimerFlag > 4 )
			   {
			    TIM_Cmd(TIM2, DISABLE); 
			    TimerFlag = 0;				 
                CurrentPointer = 3 ;
				GPIO_SetBits(GPIOC, GPIO_Pin_13);
				TIM_Cmd(TIM2, ENABLE);  	
				}
			    break;
	 case 3 :   /*�Ͽ����Ϳ��أ�*/
			
	            if (TimerFlag > 2 )
			   {
			    TIM_Cmd(TIM2, DISABLE);
			    TimerFlag = 0;				  
                CurrentPointer = 4 ;
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);	
				TIM_Cmd(TIM4, DISABLE);
				DisplayWaitTime1(WaitTime);
				PlayMusic(VOICE_3);		//��3.��ȡ��
				TIM_Cmd(TIM2, ENABLE);  
				}			 
			    break;
	 case 4 :  	/*������������ȡ��*/
	           	
	            if (TimerFlag > 2 )
			   {
			    Polls();/*�������һ��Ǯ*/
			    TimerFlag = 0;
				TIM_Cmd(TIM2, DISABLE);  
                CurrentPointer = 5 ;
				 if(IsCard == 1)
				  {
					   	IsCard = 0 ;   //���� 
					    PictrueDisplay(GETMEAL_PAGE);  //��ȡ�ͽ���
					    DisplayBalance(UserAct.IC_Card_Balance);
				  }
				}
	 	       
			    break;
	 case 5 :  /*��ʾ���*/	
	            WriteMeal();   /*���浱ǰ�ķ���*/
				PlayMusic(VOICE_4);	    //4.��ӭ�´ι���
				CurrentPointer = 6 ;			   
			    break;
	 case 6 :	/*��ӭ�´ι���*/	 
	 	  	 
				CurrentPointer = 7;				
			    break;
	 case 7 : 	/*�ص�������*/  
				//
			
				PictrueDisplay(HOME_PAGE);  //��ȡ�ͽ���
				TemperatureDisplay(Temperature);
				DisplayMealCount();								  
				UserAct.MealCount= 0 ;
				UserAct.MoneyBack   = 0 ;
				UserAct.PayAlready  = 0;
		    	UserAct.PayForBills = 0;
		    	UserAct.PayForCoins = 0;
		    	UserAct.PayForCards = 0;
				TimerFlag = 0 ;
				CurrentPointer = 0 ;
				return 0 ;
	case 8 :
	      
	           break;		       
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
 	if(TemperatureFlag == 1 &&  LCDstructure.LCD_Temp == 1 )
	{    
	 
	   TemperatureFlag = 0 ;
	   if(DS18B20_DEMO()==0)
	   {
	    if(Temperature < 70)//�������ˡ�Ȼ����ʾ
	    TemperatureDisplay(Temperature); 
	   }
	   OpenTIM7(); 
	}
}
  /*******************************************************************************
 * ��������:LcdHandler                                                                     
 * ��    ��:LCD������                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 

 void LcdHandler(void)
 {
   if(LedCmd != 0)
    {
   	 LedCmd = 0 ;
	 Current=MsgHandle(LedRecv[4]);
	 USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	
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

    for(i = 0; i < 9; i++)
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
	 UserAct.RechxargeCounter= 0;
     UserAct.IC_Card_Balance	= 0;
	 UserAct.PayAlready      = 0;
	 IsCard            = 0;
	 LCD_En_Temp();

     SystemInit();
	 delay_ms(30000);
	 LED_Init();
     InitBills();               //ֽ�һ���ʼ�� 
 	 Uart1_Configuration();	    //����1��ʼ��
	 Uart3_Configuration();	    // ��������ʼ��
	 Uart2_Configuration();	    //����ͨ������������
	 Uart5_Configuration();		//���紮�ڳ�ʼ��
	 Uart6_Configuration();
	 //InitCards();			    //IC����ʼ��
	 TIM2_Init();		        //���
	 TIM3_Init();		        //���ڶ�ʱ������ʱ
	 TIM4_Init();		        //����
	 TIM5_Init();		        //����ʱ�˱�
	 TIM7_Init();				//���ڶ�ʱ�ɼ��¶�
	 InitCoins();		        //Ӳ�ҳ�ʼ��
	 InitCoinsHopper() ;		 //�˱���ʼ��
//	 InitCoinMachine();		    //Ӳ�һ���ʼ��
//	 CAN_InitConfig();	        //can��ʼ��
 	 PictrueDisplay(0);        //��ʾLOGO
	 InitVoice()  ;             //������ʼ��
	 MyRTC_Init();              //RTC��ʼ��
	 SPI_FLASH_Init();          //Flash��ʼ��
	 LED_Init();
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

