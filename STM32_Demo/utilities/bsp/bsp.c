#include "stdint.h"
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
 uint8_t    DisplayTimes	 ;
 uint8_t    Line = 0,Column = 0 ;
 bool FindMeal(MealAttribute *DefineMeal) 
 {
    for(Line = 0; Line < FloorMealNum; Line++)  //���Ҳ�
		{
			for(Column = 0; Column < 3; Column++)     //������
			{
			  if(DefineMeal[UserActMessageWriteToFlash.UserAct.MealID - 1].Position[Line][Column] > 0)	
			    return true;		                           //����λ���в͵Ļ�����0
			}
		}
		return false ;
 }

  /*******************************************************************************
 * ��������:MoveToFisrtMeal();                                                                    
 * ��    ��:�ҵ��û�ѡ��ĵ�һ�ݲ�Ʒ�������ƶ�����Ӧλ��                                                              
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��6��25��                                                                    
 *******************************************************************************/ 
void MoveToFisrtMeal(void)
{
	if(UserActMessageWriteToFlash.UserAct.MealCnt_1st>0)
	{
	  UserActMessageWriteToFlash.UserAct.MealID = 0x01; 
	}				
	else if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd>0)
  {
    UserActMessageWriteToFlash.UserAct.MealID = 0x02;
	}				
	else if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd>0)
	{
		UserActMessageWriteToFlash.UserAct.MealID = 0x03;	
	}
	else if(UserActMessageWriteToFlash.UserAct.MealCnt_4th>0)
	{
		UserActMessageWriteToFlash.UserAct.MealID = 0x04;	
	}
  if(FindMeal(DefineMeal)) /*���Ҳ�ƷID��λ��*/
  {
		OrderSendCoord(Line,Column);
	}   
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
			
void PrintTickFun(uint32_t *PrintTickFlag)
{      
	if(*PrintTickFlag == 0x00000001 )
	{
		 TIM_Cmd(TIM4, DISABLE);
	 	*PrintTickFlag = 0x00000000;
		/*��ӡСƱ�ĺ���*/
		 SPRT();
	   TIM_Cmd(TIM4, ENABLE);
	}	 
	if(*PrintTickFlag == 0x00000002 )
	{
	   TIM_Cmd(TIM4, DISABLE);
		 *PrintTickFlag = 0x00000000;
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
	uint8_t reduce_money_flag = 0;
	uint32_t temp1=0,temp2=0;
	VariableChage(wait_payfor,WaitTime);
  switch(CurrentPoint)
	{
	  case 1 : 
	  {
	    /*��ʾ���ʽ���ֽ����п�������ͨ*/
			if(WaitTime<53)
			{
			  CurrentPoint = 3;
			  /*֧����ʽ*/			 
			  //UserActMessageWriteToFlash.UserAct.PayType = 0x31;/* �ֽ�֧��*/
        //ȡ���Զ�
        //PlayMusic(VOICE_3);
				//if(!OpenCashSystem()){OpenCashSystem();};// printf("cash system is erro2");  //�ر��ֽ����
			}
		}break;    		
	  case 2:  //����Ļ������ת
	  {
	    /*����ѡ�񸶿ʽ����*/
			CurrentPoint = 3 ;
		  UserActMessageWriteToFlash.UserAct.PayType = 0x31 ;/* �ֽ�֧��*/
	  }break;	  
		         
	  case 3 :  //��Ǯ
		{			
	    UserPayMoney = ReadBills();			 
		  if(UserPayMoney !=0 )	   //��ʾ�յ���Ǯ
		  {
			  UserActMessageWriteToFlash.UserAct.PayAlready  += UserPayMoney;
			  UserActMessageWriteToFlash.UserAct.PayForBills += UserPayMoney;	
				VariableChage(payment_bill,UserActMessageWriteToFlash.UserAct.PayForBills);
			  UserPayMoney = 0 ;
		  }
			CurrentPoint = 5 ;
		}break;    					
	  case 5 ://��ʾ�����˶���Ӳ��	
	  {
		  VariableChage(payment_coin,UserActMessageWriteToFlash.UserAct.PayForCoins);	
		  VariableChage(payment_card,UserActMessageWriteToFlash.UserAct.PayForCards); 
			CurrentPoint = 6;
		}break;		    
	  case 6 : //ͳ��Ǯ��
    {
	    if(UserActMessageWriteToFlash.UserAct.PayAlready +UserActMessageWriteToFlash.UserAct.PayForCards>=UserActMessageWriteToFlash.UserAct.PayShould)		//Ͷ��Ǯ���ڵ���Ҫ����Ǯ
		  {     
		    CurrentPoint = 9;	             
	   	}
	    else
	    { 
		    CurrentPoint = 3; 
	    } 	
		}break;   				 
    case 7 :  /*���п�֧������Ļ����*/
		{
			WaitTimeInit(&WaitTime);
			PageChange(Cardbalence_interface);
			UserActMessageWriteToFlash.UserAct.PayType = 0x32 ;/* ���п�֧��*/
			reduce_money_flag = GpbocDeduct(UserActMessageWriteToFlash.UserAct.PayShould-UserActMessageWriteToFlash.UserAct.PayAlready); //*100;
			if(reduce_money_flag == 1)
			{
				UserActMessageWriteToFlash.UserAct.PayForCards = UserActMessageWriteToFlash.UserAct.PayShould - UserActMessageWriteToFlash.UserAct.PayAlready;
			  UserActMessageWriteToFlash.UserAct.PayAlready += UserActMessageWriteToFlash.UserAct.PayForCards ;
				UART3_ClrRxBuf();
			  CurrentPoint =6;
			}
			else
			{
				WaitTimeInit(&WaitTime);
				PageChange(Acount_interface+2);
				CurrentPoint = 0;
			  /*֧����ʽ*/			 
			  UserActMessageWriteToFlash.UserAct.PayType = 0x00;//���֧����ʽ			
        UART3_ClrRxBuf();
			}
	  }break;
	  case 8 :/*����֧ͨ������Ļ����*/
	  {
			WaitTimeInit(&WaitTime);
			PageChange(Cardbalence_interface);
	    UserActMessageWriteToFlash.UserAct.PayType = 0x33 ;/* ����֧ͨ��*/
			reduce_money_flag = SztDeduct(UserActMessageWriteToFlash.UserAct.PayShould - UserActMessageWriteToFlash.UserAct.PayAlready); //*100;
			if(reduce_money_flag == 1)
			{
				UserActMessageWriteToFlash.UserAct.PayForCards = UserActMessageWriteToFlash.UserAct.PayShould - UserActMessageWriteToFlash.UserAct.PayAlready;
			  UserActMessageWriteToFlash.UserAct.PayAlready += UserActMessageWriteToFlash.UserAct.PayForCards ;
				UART3_ClrRxBuf();
			  CurrentPoint =6;
			}
			else
			{
				WaitTimeInit(&WaitTime);
				PageChange(Acount_interface+2);
				CurrentPoint = 0;
			  /*֧����ʽ*/			 
			  UserActMessageWriteToFlash.UserAct.PayType = 0x00;//���֧����ʽ
        UART3_ClrRxBuf();
			}
		}break;
	  case 9 :  //����ɹ��ر����е�����ϵͳ
		{
			UserActMessageWriteToFlash.UserAct.MoneyBack = UserActMessageWriteToFlash.UserAct.PayAlready - UserActMessageWriteToFlash.UserAct.PayShould;	
			UserActMessageWriteToFlash.UserAct.Meal_totoal = UserActMessageWriteToFlash.UserAct.MealCnt_8th+UserActMessageWriteToFlash.UserAct.MealCnt_7th+ UserActMessageWriteToFlash.UserAct.MealCnt_6th+UserActMessageWriteToFlash.UserAct.MealCnt_5th
                                                      +UserActMessageWriteToFlash.UserAct.MealCnt_4th+UserActMessageWriteToFlash.UserAct.MealCnt_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_2nd+UserActMessageWriteToFlash.UserAct.MealCnt_1st;
      UserActMessageWriteToFlash.UserAct.MoneyBackShould = UserActMessageWriteToFlash.UserAct.MoneyBack; //��¼��Ǯ֮���һ��Ӧ���˵ı�	
      VariableChage(mealout_totle,UserActMessageWriteToFlash.UserAct.Meal_totoal);	
      //����Ҫ��ӡ�����ݸ�ֵ����ؽṹ��	
     	Print_Struct.P_Number1st = UserActMessageWriteToFlash.UserAct.MealCnt_1st;
			Print_Struct.P_Number2nd = UserActMessageWriteToFlash.UserAct.MealCnt_2nd;
			Print_Struct.P_Number3rd = UserActMessageWriteToFlash.UserAct.MealCnt_3rd;
			Print_Struct.P_Number4th = UserActMessageWriteToFlash.UserAct.MealCnt_4th;
			Print_Struct.P_Number5th = UserActMessageWriteToFlash.UserAct.MealCnt_5th;
      Print_Struct.P_Number6th = UserActMessageWriteToFlash.UserAct.MealCnt_6th;
			Print_Struct.P_Number7th = UserActMessageWriteToFlash.UserAct.MealCnt_7th;
      Print_Struct.P_Number8th = UserActMessageWriteToFlash.UserAct.MealCnt_8th;
			Print_Struct.P_Cost1st   = UserActMessageWriteToFlash.UserAct.MealCost_1st;
			Print_Struct.P_Cost2nd   = UserActMessageWriteToFlash.UserAct.MealCost_2nd;
			Print_Struct.P_Cost3rd   = UserActMessageWriteToFlash.UserAct.MealCost_3rd;
			Print_Struct.P_Cost4th   = UserActMessageWriteToFlash.UserAct.MealCost_4th;
      Print_Struct.P_Cost5th   = UserActMessageWriteToFlash.UserAct.MealCost_5th;
      Print_Struct.P_Cost6th   = UserActMessageWriteToFlash.UserAct.MealCost_6th;
      Print_Struct.P_Cost7th   = UserActMessageWriteToFlash.UserAct.MealCost_7th;
      Print_Struct.P_Cost8th   = UserActMessageWriteToFlash.UserAct.MealCost_8th;
			Print_Struct.P_paymoney  = UserActMessageWriteToFlash.UserAct.PayForBills +	UserActMessageWriteToFlash.UserAct.PayForCoins +UserActMessageWriteToFlash.UserAct.PayForCards ;
			Print_Struct.P_PayShould = UserActMessageWriteToFlash.UserAct.PayShould ;
			Print_Struct.P_MoneyBack = UserActMessageWriteToFlash.UserAct.MoneyBack ;
			CurrentPoint = 0 ;
	       return  Status_OK;
		}
	  default :break;
  }
	if(WaitTime<=1) 
	{
    WaitTimeInit(&WaitTime);
		PageChange(Menu_interface);//��ʱ�˳��û���Ʒ����ѡ�����
		if(!CloseCashSystem()){CloseCashSystem();};//printf("cash system is erro6\r\n");  //�ر��ֽ����
		CurrentPoint = 0 ;
		temp1= UserActMessageWriteToFlash.UserAct.MoneyBack= UserActMessageWriteToFlash.UserAct.PayAlready; //��ʱ���յ���Ǯ��Ӳ�ҵ���ʽ����
		temp2= UserActMessageWriteToFlash.UserAct.MoneyBackShould= UserActMessageWriteToFlash.UserAct.PayAlready; //������Ҫ��¼
		ClearUserBuffer();//����û�����
    UserActMessageWriteToFlash.UserAct.MoneyBack= temp1;
    UserActMessageWriteToFlash.UserAct.MoneyBackShould= temp2;
    SaveUserData();//�����û�����  
		if(UserActMessageWriteToFlash.UserAct.MoneyBack>0)
		  Current= hpper_out;
    else
      Current= current_temperature;
	}
  else
  {
    if(WaitTime%10==1) 
    {
      if(UserActMessageWriteToFlash.UserAct.PayType==0x31)
      {
        PlayMusic(VOICE_3);
      }
      else
      {
        PlayMusic(VOICE_2);
      }        
    }    
  }
  return  Status_Action;
}												


uint8_t WaitMeal(void)
{
	uint8_t MealoutCurrentPointer=0;
  //static unsigned char Cmd[20]={0x05, 0x31, 0x30, 0x30, 0x31, 0x30 ,0x30, 0x36, 0x34, 0x30, 0x30, 0x30, 0x34, 0x4D, 0x31, 0x35, 0x43, 0x03, 0x0D ,0x0A};
	uint8_t temp;
	do
	{
		manageusart6data();  //����е�ֵ����ݴ������ȡ��ͷ��������while(1)
		DealSeriAceptData(); //ֻ�����ӡ����
		switch(MealoutCurrentPointer)
		{
			case 0 : /*�����û���ѡ��Ʒ��λ��*/
			{
				//��ֵ��ǰUserActMessageWriteToFlash.UserAct.MealID
				if(UserActMessageWriteToFlash.UserAct.MealCnt_1st>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x01; 
				}				
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x02;
				}				
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x03;	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_4th>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x04;	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_5th>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x05;	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_6th>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x06;	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_7th>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x07;	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealCnt_8th>0)
				{
					UserActMessageWriteToFlash.UserAct.MealID = 0x08;	
				}
				else
				{	
					 //�����Ʒȫ�����꣬�˳���������		
					 //printf("takeafter_meal\r\n");		
					PlayMusic(VOICE_12);				
					return takeafter_meal;
				}
      	if(FindMeal(DefineMeal)) /*���Ҳ�ƷID��λ��*/
        {
					PlayMusic(VOICE_8);	
					MealoutCurrentPointer= 1;
				}
        //printf("case 0/UserAct.MealID == %d\r\n",UserAct.MealID);				
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
				DefineMeal[UserActMessageWriteToFlash.UserAct.MealID - 1].Position[Line][Column]--;
				DefineMeal[UserActMessageWriteToFlash.UserAct.MealID - 1].MealCount--;
				WriteMeal();
				StatisticsTotal();
				DispLeftMeal(); 
				MealoutCurrentPointer = 2 ;
			}break ;   
			case 2 : //����[Line][Column]��ֵ�������� �ȴ�ACK	
			{
				//printf("�����к��е�λ�ã��ȴ���Ӧ\r\n");
				temp =0;
				temp = OrderSendCoord(Line,Column);
				
				if( temp ==1)//���ͳɹ�
				{
					LinkTime =0;
					machinerec.rerelative =0;   //���Ǵ���λ��
					MealoutCurrentPointer = 3 ;
				}
				else				//����ʧ��
				{
					//printf("send coord error\r\n");
					erro_record |= (1<<SendUR6Erro);
					return takemeal_erro;
				}
			}break;  
			case 3 :    /*����ȡ������*/
			{
				//��ѯ��е���Ƿ�׼���ã����׼���÷���ȡ������
				//�����ʱ�� ���ش���
			  if(LinkTime > 10)    //��ʱ
				{
				  LinkTime =0;
					//printf("move to coord timeout!\r\n");
					erro_record |= (1<<SendUR6Erro);
					return takemeal_erro;
				}       
				if(machinerec.regoal ==1)   //����ȡ�͵�
				{
					machinerec.regoal =0 ;
					temp =0;
					temp = OrderGetMeal();   //����ȡ������
					LinkTime =0;
					MealoutCurrentPointer=4;  				
					if(temp ==1)       // ȡ������ͳɹ�
					{
						LinkTime =0;
						MealoutCurrentPointer=4;  
					}
					else          //����ʧ��     
					{
					  //printf(" send getmeal order error\r\n");
						erro_record |= (1<<SendUR6Erro);
						return takemeal_erro;
					}
				}
			}break;
			case 4 :  	/*������������ȡ��*/
			{
				//�����Ʒ����ȡ�Ϳڲ�������
				//�����Ʒȡ���� �����ӳ�����������ϴ�  
				if(machinerec.retodoor == 1)   //������Ϳ�
				{
					machinerec.retodoor = 0;
					//������ȡ������
					PlayMusic(VOICE_9);
				}
				if(machinerec.reenablegetmeal ==1)  //ȡ��5���˻�δȡ����
				{
					machinerec.reenablegetmeal =0; //�¼ӵ���Ҫ����ر������
					//printf("ȡ��5���˻�δȡ����\r\n");	 
					erro_record |= (1<<GetMealError);
					return takemeal_erro;
				}
				if(machinerec.retodoor == 1) //������Ϳ�
				{
					machinerec.retodoor = 0;		
					LinkTime =0;		 
				}
				//printf("��δ�����������ӻ�δ��ȡ��\r\n");
				if( machinerec.remealaway == 1) //���ѱ�ȡ��
				{
				//printf("���ѱ�ȡ��\r\n");
					LinkTime =0;
					machinerec.remealaway = 0;
				  MealoutCurrentPointer=5;
					break;
				}
				if( machinerec.remealnoaway == 1)  //����ȡ�Ϳڹ���20�뻹δ��ȡ��
			  {
					if( LinkTime >=40) //���ڳ��Ϳ�δ��ȡ�ߣ�һֱ�ȴ�ʱ��ʱ�����20s����������ʾȡ��
					{
						if(LinkTime%20==0)
						{
							PlayMusic(VOICE_10);
						}
						LinkTime= 0;
					}					
				//������ʾ����ȡ�߳��ͿڵĲ� "
				} 
			}break;			    
			case 5:     /*���û����ݽ��м�һ*/  //?? �����Ҫ���д����˱ң���Ҫ�޸÷���ֵ���ڷ�Χ
			{
				machinerec.remealnoaway = 0;
				UserActMessageWriteToFlash.UserAct.Meal_takeout++;//ȡ�����ݼ�
				VariableChage(mealout_already,UserActMessageWriteToFlash.UserAct.Meal_takeout);	//UI��ʾ
				MealoutCurrentPointer= 0;
				//printf("case 5/UserAct.MealID == %d\r\n",UserAct.MealID);
				if(UserActMessageWriteToFlash.UserAct.MealID == 0x01)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_1st--;
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_1st==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}	
				}
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x02)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_2nd--;
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_2nd==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x03)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_3rd--;
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_3rd==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}				
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x04)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_4th--; 	
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_4th==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x05)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_5th--; 	
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_5th==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x06)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_6th--; 	
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_6th==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x07)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_7th--; 	
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_6th==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				} 
				else if(UserActMessageWriteToFlash.UserAct.MealID == 0x08)
				{
					UserActMessageWriteToFlash.UserAct.MealCnt_8th--; 	
					//if(UserActMessageWriteToFlash.UserAct.MealCnt_6th==0)
					if(0)
					{
						//printf("tookkind_meal\r\n");
						return tookkind_meal;
					}
					else
					{
						//printf("tookone_meal\r\n");
				    return tookone_meal;
					}						
				}         
				else 
				{
					printf("tookone_meal erro\r\n");
        }
			}
			default:break;
	  }
	}while(1);
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
#define bill_time  600  //500ms�������100msֽ�һ���Ӧ������
bool CloseCashSystem(void)
{
	uint8_t cnt_t=253;
  CloseCoinMachine();			    //�ر�Ͷ�һ�	
	delay_ms(bill_time);
	memset(BillDataBuffer,0xFE,sizeof(BillDataBuffer));
	DisableBills();             //���ò��ر�ֽ�һ�
	do
	{
		cnt_t--;
		delay_ms(1);
		if((RX4Buffer[0]=='0')&&(RX4Buffer[1]=='0'))
		{
			return true;
		}
		else if((RX4Buffer[0]=='F')&&(RX4Buffer[1]=='F'))
		{
			return false;  			
		}
	}while(cnt_t);			 
	 return false; 
}

bool OpenCashSystem(void)
{
	uint8_t cnt_t=254;	
	OpenCoinMachine();    //��Ͷ�һ�	
	delay_ms(bill_time);        //��Ҫ����
	memset(BillDataBuffer,0xFE,sizeof(BillDataBuffer));
	SetBills();           //���ò���ֽ�һ�
	do
	{
		cnt_t--;
		delay_ms(1);
		if((RX4Buffer[0]=='0')&&(RX4Buffer[1]=='0'))
		{
			return true;
		}
		else if((RX4Buffer[0]=='F')&&(RX4Buffer[1]=='F'))
		{
			return false;  			
		}
	}while(cnt_t);	
	return false; 
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
 * ��������:AcountCopy                                                                   
 * ��    ��:�쳣����                                                                 
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void AcountCopy(void)
{
	UserActMessageWriteToFlash.UserAct.MealCnt_1st_t= UserActMessageWriteToFlash.UserAct.MealCnt_1st;
	UserActMessageWriteToFlash.UserAct.MealCnt_2nd_t= UserActMessageWriteToFlash.UserAct.MealCnt_2nd;
	UserActMessageWriteToFlash.UserAct.MealCnt_3rd_t= UserActMessageWriteToFlash.UserAct.MealCnt_3rd;
	UserActMessageWriteToFlash.UserAct.MealCnt_4th_t= UserActMessageWriteToFlash.UserAct.MealCnt_4th;
  UserActMessageWriteToFlash.UserAct.MealCnt_5th_t= UserActMessageWriteToFlash.UserAct.MealCnt_5th;
  UserActMessageWriteToFlash.UserAct.MealCnt_6th_t= UserActMessageWriteToFlash.UserAct.MealCnt_6th;
  UserActMessageWriteToFlash.UserAct.MealCnt_7th_t= UserActMessageWriteToFlash.UserAct.MealCnt_7th;
  UserActMessageWriteToFlash.UserAct.MealCnt_8th_t= UserActMessageWriteToFlash.UserAct.MealCnt_8th;
}

  /*******************************************************************************
 * ��������:PowerupAbnormalHandle                                                                    
 * ��    ��:�����쳣����ֻ��ʾ�����ʶ��������������ʾ�û�������¼                                                                
 *            ������Ҫ�ж��Ƿ�ϵ磬���ж�UserActMessageWriteToFlash.UserAct.paybacke�Ƿ����0����ʾ�û�ѡ���
              ��Ʒ����δ���Ĳ�Ʒ�����Լ�Ǯ��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 
void PowerupAbnormalHandle(int32_t erro_record_t)
{
  AbnormalHandle(erro_record_t);
	erro_record=0;
}

  /*******************************************************************************
 * ��������:PollAbnormalHandle                                                                   
 * ��    ��:��ѯ                                                                 
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��5��9��                                                                    
 *******************************************************************************/ 
void PollAbnormalHandle(void)
{
	uint8_t i=0;
	for(i=32;i>0;i--) //32λ��¼�����쳣��������Ҫ���� ������ͨ������Ա�������
	{
		if(erro_record&(1<<i))
		AbnormalHandle(i);	
	}	
}

  /*******************************************************************************
 * ��������:AbnormalHandle                                                                    
 * ��    ��:�쳣���� �������û����ݵ���ʾ                                                                
 *           ���������쳣������Ҫ�����жϣ��൱�ڿ����ж�                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
uint32_t erro_flag=0;
void AbnormalHandle(uint32_t erro)
{	
	erro_record |= (1<<erro); //�ڿ�ʼ�쳣�����ʱ����Ҫ�õ�
	//printf("erro_record2nd 0x%08X\r\n",erro_record);
	erro_flag = erro; //ֻ���ڴ��������ж�ʱ��Ҫ�õ���
	switch(erro)
	{
		case outage_erro:      //�ϵ磺ֻ���ڿ�����ʱ���ж��Ƿ��жϵ��������� ������Ӧ���˵�Ǯ�����о��Ǹ�Ǯ��ʱ��ϵ�
			{            /*ȡ�ͳ�������*/              /*�˱ҳ�������,������Ǯ��ʱ��ϵ�*/
			  if((UserActMessageWriteToFlash.UserAct.Meal_totoal!=UserActMessageWriteToFlash.UserAct.Meal_takeout)||(UserActMessageWriteToFlash.UserAct.MoneyBack>0))//���ж��Ƿ��в�Ʒû��ȡ�������ж��û�δ�˵�Ǯ
				{
					/*������ϵ�����Ϣ���鿴����������������ֵ*/
          if(UserActMessageWriteToFlash.UserAct.MealID>0)
					{
            //�����ܵ�Ӧ�˱���
            UserActMessageWriteToFlash.UserAct.MoneyBackShould +=
                         (UserActMessageWriteToFlash.UserAct.MealCnt_1st *price_1st +UserActMessageWriteToFlash.UserAct.MealCnt_2nd *price_2nd+ UserActMessageWriteToFlash.UserAct.MealCnt_3rd* price_3rd+ UserActMessageWriteToFlash.UserAct.MealCnt_4th *price_4th
                         +UserActMessageWriteToFlash.UserAct.MealCnt_5th* price_5th+ UserActMessageWriteToFlash.UserAct.MealCnt_6th* price_6th+ UserActMessageWriteToFlash.UserAct.MealCnt_7th* price_7th+ UserActMessageWriteToFlash.UserAct.MealCnt_8th* price_8th);	                         
					  //�����ܵ�δ�˱���
            UserActMessageWriteToFlash.UserAct.MoneyBack +=
                         (UserActMessageWriteToFlash.UserAct.MealCnt_1st *price_1st+ UserActMessageWriteToFlash.UserAct.MealCnt_2nd *price_2nd+ UserActMessageWriteToFlash.UserAct.MealCnt_3rd* price_3rd+ UserActMessageWriteToFlash.UserAct.MealCnt_4th *price_4th
                         +UserActMessageWriteToFlash.UserAct.MealCnt_5th* price_5th+ UserActMessageWriteToFlash.UserAct.MealCnt_6th* price_6th+ UserActMessageWriteToFlash.UserAct.MealCnt_7th* price_7th+ UserActMessageWriteToFlash.UserAct.MealCnt_8th* price_8th);	 
            //�����ϴ��Ľ��׽��
            payfor_meal =(UserActMessageWriteToFlash.UserAct.MealCnt_1st *price_1st+ UserActMessageWriteToFlash.UserAct.MealCnt_2nd *price_2nd+ UserActMessageWriteToFlash.UserAct.MealCnt_3rd* price_3rd+ UserActMessageWriteToFlash.UserAct.MealCnt_4th *price_4th
                         +UserActMessageWriteToFlash.UserAct.MealCnt_5th* price_5th+ UserActMessageWriteToFlash.UserAct.MealCnt_6th* price_6th+ UserActMessageWriteToFlash.UserAct.MealCnt_7th* price_7th+ UserActMessageWriteToFlash.UserAct.MealCnt_8th* price_8th);	  
                        
						UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already= (UserActMessageWriteToFlash.UserAct.MoneyBackShould-UserActMessageWriteToFlash.UserAct.MoneyBack);//�������˱ҵ�Ǯ����Ҫ����Ҫ�����˱ҵ����ݣ�
						DataUpload(Failed);//ֻ�е�UserActMessageWriteToFlash.UserAct.MealID!=0��ʱ����ϴ���Ʒ������
						SaveUserData();
					}
					DisplayAbnormal("E070");
					PageChange(Err_interface);
        }
				else 
				{
					erro_record &= ~(1<<erro); //�����Ҫ���������쳣��
					erro_flag= 0;
					return;
				}
			}break;
		case sdcard_erro:     //SD���洢�쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E000");
	      PageChange(Err_interface);					
				StatusUploadingFun(0xE000); //״̬����
			}break;
		case billset_erro:    //ֽ�һ��쳣
			{
	      PlayMusic(VOICE_11);	  			
				DisplayAbnormal("E010"); 
        PageChange(Err_interface);	
				StatusUploadingFun(0xE010); //״̬����
			}break;
		case coinset_erro:      //Ͷ�һ�
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E020");
	      PageChange(Err_interface);				
				StatusUploadingFun(0xE020); //״̬����
			}break;
		case coinhooperset_erro:    //�˱һ�
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E030");
	      PageChange(Err_interface);				
				StatusUploadingFun(0xE030); //״̬����
				//û�бҿ��˵�ʱ��UserActMessageWriteToFlash.UserAct.Payback ��Ϊ0
			}break;
		case coinhooperset_empty:   //�����ù�
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E032");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE032); //״̬����
			}break;
		case printer_erro:      //��ӡ���쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E040");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE040); //״̬����
			}break;
		case cardread_erro:     //�������쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E050");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE050); //״̬����
			}break;
		case network_erro:     //�����쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E060");
	      PageChange(Err_interface);
			}break;
		case signin_erro:    //ǩ���쳣
		   {
	       PlayMusic(VOICE_11);	
         DisplayAbnormal("E061");			
	       PageChange(Err_interface);  
       }break;
		case upload_erro:     //�����ϴ���ʱ��ͻȻ�ϵ�
			{
        DataUpload(Success);
				erro_record &= ~(1<<upload_erro);
				return;
			};
    case arm_limit: //��е�ֽ�ֹ��λ
		  {
			  if((UserActMessageWriteToFlash.UserAct.Meal_totoal!=UserActMessageWriteToFlash.UserAct.Meal_takeout)||(UserActMessageWriteToFlash.UserAct.MoneyBack>0))//���ж��Ƿ��в�Ʒû��ȡ�������ж��û�δ�˵�Ǯ
				{
					/*������ϵ�����Ϣ���鿴����������������ֵ*/
          if(UserActMessageWriteToFlash.UserAct.MealID>0)
					{					
						SaveUserData();
					}
					DisplayAbnormal("E100");
					PageChange(Err_interface);
        }
				else 
				{
					erro_record &= ~(1<<erro); //�����Ҫ���������쳣��
					erro_flag= 0;
					return;
				}				
      }break;			
		case X_timeout:        //x�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E101");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE101); //״̬����
				DataUpload(Failed);
			}break;
		case X_leftlimit:      //��������������
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E102");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE102); //״̬����
				DataUpload(Failed);
			}break;
		case X_rightlimit:     //����Ҷ����������
			{
				DisplayAbnormal("E103");
	      PlayMusic(VOICE_11);
        StatusUploadingFun(0xE103); //״̬����	
	      PageChange(Err_interface);
				DataUpload(Failed);
			}break;
		case mealtake_timeout: //ȡ�Ϳڴ�������ʱ
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E201");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE201); //״̬����
				DataUpload(Failed);
			}break;
		case Y_timeout:        //y�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);
        DisplayAbnormal("E301");	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE302); //״̬����
				DataUpload(Failed);
			}break;
		case link_timeout:     //���ӳ�ʱ
			{
	      PlayMusic(VOICE_11);
        DisplayAbnormal("E401");	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE401); //״̬����
			}break;
		case Z_timeout:        //z�ᴫ������ʱ
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E501");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE501); //״̬����
				DataUpload(Failed);
			}break;
		case Z_uplimit:        //z������϶�������
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E502");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE502); //״̬����
				DataUpload(Failed);
			}break;
		case Z_downlimit:      //z����¶�������
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E503");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE503); //״̬����
				DataUpload(Failed);
			}break;
		case solenoid_timeout: //��ŷ���ʱ  ???��ʱ���쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E601");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE601); //״̬����
				DataUpload(Failed);
			}break;
		case Eeprom_erro:      //eeprom �쳣
			{
	      PlayMusic(VOICE_11);	
        DisplayAbnormal("E711");
	      PageChange(Err_interface);
				StatusUploadingFun(0xE711); //״̬����
				DataUpload(Failed);
			}break;
		case SendUR6Erro:      //���������쳣��ʱ
			{
	      PlayMusic(VOICE_11);
        DisplayAbnormal("E801");	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE801); //״̬����
				DataUpload(Failed);
      }break;
    case GetMealError:     //��е��5��ȡ������
			{
	      PlayMusic(VOICE_11);
        DisplayAbnormal("E802");	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE802); //״̬����
				DataUpload(Failed);
      }break;
    case MealNoAway:       //���ڳ��Ϳ�20�뻹δ��ȡ��
			{
	      PlayMusic(VOICE_11);
        DisplayAbnormal("E803");	
	      PageChange(Err_interface);
				StatusUploadingFun(0xE803); //״̬����
      }break;
		default:break;
	}
  SaveUserData(); //������һ�����ݱ���һ��
  while(1)
  {
    DealSeriAceptData();
    if(erro_flag==0)
    {
      if(erro_record&(1<<arm_limit)) //�����Ļ�е�ֽ�ֹ��λ
      {
        PageChange(OnlymachieInit_interface);					
        OnlymachieInit(); //��е�ֵĳ�ʼ��
      }
      else
      {
        UserActMessageWriteToFlash.UserAct.MoneyBack=0;
        UserActMessageWriteToFlash.UserAct.MoneyBackShould=0;
        UserActMessageWriteToFlash.UserAct.MoneyBackAlready=0;
        ClearUserBuffer();
        SaveUserData();						
      }
      if(erro_record>=(1<<X_timeout))//����ǻ�е�۴���ֵ�ϴ�һ�����ݣ��Ը�λȡ��
      {
        erro_record&=~0xFFFF0000; //ֻ��ʾһ���쳣
      }
      erro_record &= ~(1<<erro); //һ��ֻ����һ���쳣
      RTC_WriteBackupRegister(RTC_BKP_DR13, erro_record);
      break;
    }
  }
}
  /*******************************************************************************
 * ��������:ErrRecHandle                                                                     
 * ��    ��:�����¼����                                                              
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��7��2��                                                                    
 *******************************************************************************/ 
void ErrRecHandle(void)
{
	 if(erro_record!=0) //���д����¼����Ҫ���д���
	 {
		 AbnormalHandle(outage_erro);//�൱�ڿ����쳣����
		 if(erro_record&(1<<upload_erro))
		 {
			 if(UserActMessageWriteToFlash.UserAct.MealID!=0)
			   AbnormalHandle(upload_erro);//��Ҫ���������ϴ��Ķϵ�
		 }
		 erro_record=0;
	 }
	 else
	 {
		 ClearUserBuffer(); //���֮ǰ��ȡ������
		 UserActMessageWriteToFlash.UserAct.MoneyBack=0;//
	 }
   SaveUserData(); 	
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
	Uart6_Configuration();   //��е�� 1 ,2
 //��ʼ�����λ�����ݽṹ��
  for(i = 0; i <MealKindTotoal; i++)
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
 	 UserActMessageWriteToFlash.UserAct.PayForCoins     = 0;
	 UserActMessageWriteToFlash.UserAct.PayForBills     = 0;
	 UserActMessageWriteToFlash.UserAct.PayForCards     = 0;
	 UserActMessageWriteToFlash.UserAct.PayAlready      = 0;
   SystemInit();
	 PVD_Configuration();        //�������ʼ�� 0 , 0
	 Uart4_Configuration();     //ֽ�һ����ڳ�ʼ�� 1, 2
	 Uart1_Configuration();	    //��ӡ�����ڳ�ʼ��
	 Uart3_Configuration();	    // ��������ʼ��  0 , 3
	 Uart2_Configuration();	    //����ͨ������������ 1 , 0
	 Uart5_Configuration();		//���紮�ڳ�ʼ�� 1 , 1
	 TIM2_Init();		        //���
	 TIM3_Init();		        //��Ʒ����ѡ�񵹼�ʱ
	 TIM4_Init();		        //��ӡ����ʱ5S
	 TIM5_Init();		        //��е�ֵ���ʱ
	 TIM7_Init();				    //���ﳵ���浹��ʱ
   InitCoins();		        //Ͷ�һ���ʼ��
   InitMiniGPIO() ;		   //�˱���ʼ��	 
	 InitVoice();             //������ʼ��
	 MyRTC_Init();              //RTC��ʼ��
	 //IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //�򿪿��Ź�
	 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //�򿪿��Ź�
	 IWDG_SetPrescaler(IWDG_Prescaler_128); //40K /128 =312 = 0X0138
	 IWDG_SetReload(0x0138); // 1S
	 IWDG_Enable();
	 OpenTIM2();
	 delay_ms(1000);
	 SPI_FLASH_Init();          //Flash��ʼ��
	 SPI_FLASH_Init();          //�ظ���ʼ������
   SPI_FLASH_BufferRead(FloorMealMessageWriteToFlash.FlashBuffer, SPI_FLASH_Sector0 , FloorMealNum*6);//��ȡ����Ĳ�Ʒ
   ReadCoins();//��ȡ�ж���Ӳ��	 
	 for(i=0;i<90;i++)
	 {
	   if(FloorMealMessageWriteToFlash.FlashBuffer[i] == 0xff)
	   FloorMealMessageWriteToFlash.FlashBuffer[i]    = 0 ;
	 }
	 //WriteMeal();  //д���Ʒ����,����Ҫ��Ϊд���Ʒ��������Ҫ���Ѻܳ����䣬���ͻȻ�ڿ�������ͻȻ�ϵ����ݾͻᶪʧ
	 StatisticsTotal(); //����ĳ�����Ҫʹ��  	
	 ReadUserData();  //��Ҫ�������ݴ����ж�
   for(i=0;i<UserActiontotoaDatalSize*4;i++)
	 {
	   if(UserActMessageWriteToFlash.FlashBuffer[i] == 0xff)
	   FloorMealMessageWriteToFlash.FlashBuffer[i]    = 0 ;
	 }
}														 
