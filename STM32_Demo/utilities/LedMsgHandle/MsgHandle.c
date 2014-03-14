#include "stm32f4xx.h"
#include "MsgHandle.h"
#include "serialscreen.h"
#include "vioce.h"
#include "bsp.h"
 LCDStruction  	 LCDstructure;
MealAttribute DefineMeal[9];	//����9����ϵ
FloorAttribute FloorMeal[FloorMealNum], CurFloor;
UserSelection UserAct;
FloorMealMessage FloorMealMessageWriteToFlash;
uint8_t  BillActionFlag = 1;
uint8_t  PassWordLen=0;	//����ĳ���Ϊ0
uint8_t  TemperatureSwitch = 0;
uint8_t  TemperatureCur;
uint8_t  WaitTime;


 void LCD_DisTemp(void)
 {
  	LCDstructure.LCD_Temp = 0 ;
 }
 void  LCD_En_Temp(void)
 {
  	LCDstructure.LCD_Temp = 1 ;
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
		for(j = 0; j < 9; j++)	 //9�����ID��
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



//void GetMealPrice(uint8_t MealId)
//{
//    if(MealId ==1 || MealId == 2 || MealId == 3)
//	{
//	    DefineMeal[MealId-1].MealPrice = 15;
//		UserAct.MealPrice = 15;
//		UserAct.PayShould = 15;
//	}
//	else if(MealId ==4 || MealId == 5 || MealId == 6)
//	{
//	    DefineMeal[MealId-1].MealPrice = 20;
//		UserAct.MealPrice = 20;
//		UserAct.PayShould = 20;
//	}
//	else if(MealId ==7 || MealId == 8 || MealId == 9)
//	{
//	    DefineMeal[MealId-1].MealPrice = 25;
//		UserAct.MealPrice = 25;
//		UserAct.PayShould = 25;
//	}
//}
 void GetMealPrice(uint8_t MealId)
{
    if(MealId ==1 || MealId == 2 || MealId == 3)
	{
	    DefineMeal[MealId-1].MealPrice = 4;
		UserAct.MealPrice = 4;
		UserAct.PayShould = 4;
	}
	else if(MealId ==4 || MealId == 5 || MealId == 6)
	{
	    DefineMeal[MealId-1].MealPrice = 5;
		UserAct.MealPrice = 5;
		UserAct.PayShould = 5;
	}
	else if(MealId ==7 || MealId == 8 || MealId == 9)
	{
	    DefineMeal[MealId-1].MealPrice = 5;
		UserAct.MealPrice = 5;
		UserAct.PayShould = 5;
	}
}
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
	DisplayFColumn(CurFloor.FCount);
	DisplaySColumn(CurFloor.SCount);
	DisplayTColumn(CurFloor.TCount);
    
}
  /*******************************************************************************
 * ��������:cancel                                                                     
 * ��    ��:����������                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void cancel(void)
{
  	
    PictrueDisplay(HOME_PAGE);
    DisplayMealCount();
}
  /*******************************************************************************
 * ��������:MsgHandle                                                                     
 * ��    ��:����������                                                                   
 *                                                                               
 * ��    ��:uint8_t cmd                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 

uint8_t MsgHandle(uint8_t cmd)
{
  static 	uint8_t  InputPassWord[6],PassWord[6];
  uint8_t  PictrueNum = 0;
  uint8_t i= 0,j=0;
  unsigned	char ReturnMessage = 0 ;
 //  printf("cmd=%x\r\n",cmd);
  switch(cmd)
  {
	case 0x00:		   /*������*/
	case 0x01:		   /*ѡ��1�Ųͺ�*/
	case 0x02:		   /*ѡ��2�Ųͺ�*/
	case 0x03:
	case 0x04:		   /*ѡ��3�Ųͺ�*/ //4-4=0
	case 0x05:		   /*ѡ��4�Ųͺ�*/ //5-4=0
	case 0x06:		   /*ѡ��5�Ųͺ�*/ //6-4=0
	case 0x07:		   /*ѡ��6�Ųͺ�*///7-4=0
	case 0x08:		   /*ѡ��7�Ųͺ�*/ //8-4=0
	case 0x09:		   /*ѡ��8�Ųͺ�*/ //9-4=0
	case 0x0A:		   /*ѡ��9�Ųͺ�*/ //10-4=0
	case 0x0B:		   /*ѡ��10�Ųͺ�*///11-4=0
	case 0x0C:		   /*ѡ��11�Ųͺ�*///12-4=0
		{
		 /*�ж�ѡ��Ĳ��Ƿ����0 */
	    if(DefineMeal[cmd-1].MealCount > 0)	   //Ҫѡ���ĸ���
		{
		/*Ĭ���û�ѡ��ķ���Ϊ1*/
		 UserAct.MealCount = 1 ;
		 /*�����û�ѡ������Ǹ����*/
		 UserAct.MealID  = cmd  ;               //�û�ѡ�����ʲô��ϵ	
		 /*������ʾ��Ӧ��ͼƬ�����*/
		 PictrueNum = 3+(cmd-1)*7;            //Ҫ��ʾ����ͼƬ
		 /*�õ�ѡ��з��ļ۸�*/
		 GetMealPrice(cmd);	                   //�õ���Ǯ
		 /*��ʾͼƬ*/
		 PictrueDisplay(PictrueNum);	           //��ʾͼƬ
		 /*������ϵͳ*/
		 OpenMoneySystem();			               //������ϵͳ
		 /*��ʱ300ms �ϵ�ʱ��ϵͳ���ȶ����ȴ��ȶ�*/
		 delay_ms(300);			                   //��Ӳ�һ���ʱ��ϵͳ���ܴ��ڲ��ȶ�״̬ ��ʱ��ϵͳ�ȶ�
		 /**/
		 CoinCountFlag  = 0;					   //
		 /*��ʾĬ�ϵ�һ��*/
         DisplayMealNum(1); 	                   //��ʾ����
		 /*��ʾ�ͻ�Ͷ�ҵĽ��*/
		 DisplayBills(UserAct.PayForBills);	       //��ʾֽ��
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		 DisplayCoins(UserAct.PayForCoins);	       //��ʾӲ��
		 /*��ʾ�û�ˢ�����*/
		 DisplayCards(UserAct.PayForCards);	       //��ʾˢ��
		 /*��ʾ�ܽ��*/	  
		 DisplayMealPrice(UserAct.MealPrice);      //��ʾ�ܼ۸�
		 /*�ȴ�ʱ��*/
		 WaitTime = 60 ;						   //��ʱʱ��
		 /*��ʾ�ȴ�ʱ��*/
		 DisplayWaitTime(WaitTime);
		 /*��������*/
		 PlayMusic(VOICE_1);	                   //��ѡ�����������
		 /*��������ʱ 60*/
		 OpenTIM3();							   //���ڵ���ʱ		

		 ReturnMessage = 1 ;
	 	}
		else
		{
		   /* ��ʾ��ҳ*/
		   PictrueDisplay(HOME_PAGE);
		   /*��ʾ����*/
		   DisplayMealCount();
		   /*��ʾ�¶�*/
		   DisplayTemperature(Temperature);
		}

  	  break;
  }
   case 0x0D:  //����LOGҳ��
	{

	}break;
   case 0x0E:
	{
	   /*��ʾ�û����ý���*/
	   LCDstructure.LCD_Temp = 0 ;	
	    PictrueDisplay(66);
	    PassWordLen = 0;
		for(i = 0; i < 6; i++)
		{
			InputPassWord[i] = 0;
		}
		
	}break;	

//	case 0x0F:	//����IC����ֵҳ��
//	{
//
//	    CloseTIM3();
//		OpenCoinMachine();   //����Ӳ�һ�
//	   	PictrueDisplay(IC_PAGE);
//		if(UserAct.RechxargeCounter == 0)
//		{	
//	    PlayMusic(VOICE_7); 
//		ReturnMessage = 3 ;	 //��ֵ����
//		}
//		else
//		{
//	  	 ReturnMessage = 4 ;
//		 PlayMusic(VOICE_8);
//		
//		}
//		DisplayRechangeBills(UserAct.RechxargeCounter);
//	}break;
	case 0x10:   //�����������1-10	   16   --  20
	case 0x11:	 // 17				   17	  --23
	case 0x12:	//					   18	  --26
	case 0x13:	//					   19	  --29
	case 0x14:	//					   20	  --32
	case 0x15:	//					   21	  --23
	case 0x16:	//					   17	  --23
	case 0x17:	//					   17	  --23
	case 0x18:	//					   17	  --23
	case 0x19:	//					   17	  --23
	         {
			  LCDstructure.LCD_Temp = 0 ;
			 /*�û�ѡ��ķ���*/
			 UserAct.MealCount = cmd - 15;    //ѡ��ķ���
			 /*�жϺз�����Ŀ�Ƿ�����û�ѡ��ķ���*/
			 if(UserAct.MealCount <= DefineMeal[UserAct.MealID - 1].MealCount)	  //�ж��û�������ķ����Ƿ�>�û��ķ���
		      {
			    /*�����ܼ�Ǯ=����*����*/
		       UserAct.PayShould = UserAct.MealPrice * UserAct.MealCount;
//			   printf("UserAct.PayShould=%d\r\n",UserAct.PayShould);
			   /*�ж��������Ƿ���ȷ*/
			   if(UserAct.MealCount > 0 && UserAct.MealCount <= 5)
			   {
			    /*��ʾ�����ͼ۸�*/
						
			//	DisPlayChar(Display_MealNum,sizeof(Display_MealNum),UserAct.MealCount);
		        DisplayMealNum(UserAct.MealCount); //��ʾ������  ��ʼĬ��Ϊһ��
				 
			//	DisPlayChar(Display_MealPrice,sizeof(Display_MealPrice),UserAct.PayShould);
		        DisplayMealPrice(UserAct.PayShould);//��ʾ�ܼ۸� 
			   }
		      }
			 else
			 {
			 /*Ĭ��Ϊһ��*/
			  UserAct.MealCount = 1 ;
			  /*��ʾ�۸���ܼ۸�*/
		//	  DisPlayChar(Display_MealNum,sizeof(Display_MealNum),UserAct.MealCount);
		      DisplayMealNum(UserAct.MealCount); //��ʾ������  ��ʼĬ��Ϊһ��
		      UserAct.PayShould = UserAct.MealPrice*UserAct.MealCount;
		      DisplayMealPrice(UserAct.PayShould);//��ʾ�ܼ۸� 
		//	  DisPlayChar(Display_MealPrice,sizeof(Display_MealPrice),UserAct.PayShould);
			 }
		     }
			    /*������һ������*/
			    CurrentPoint =   2;	
				/*����ֵ*/
				ReturnMessage =  1;  
		 break;
   	case 0x1A: //����ȡ��
	{
		LCDstructure.LCD_Temp = 1 ;	   //��Ҫ��ʾ�¶�
		CloseTIM3();
		BackPolls();  
	    PictrueDisplay(HOME_PAGE);
        DisplayMealCount();    	
		ReturnMessage = 0; 
	    CurrentPoint = 0;
	    TemperatureDisplay(Temperature);
	}break;
	case 0x1B: //�����
	{	 
	      LCDstructure.LCD_Temp = 0 ;
	     PictrueDisplay((UserAct.MealID-1)*7 + 3);
	     DisplayMealNum(UserAct.MealCount); 	                   //��ʾ����
		 /*��ʾ�ͻ�Ͷ�ҵĽ��*/
	     DisplayBills(UserAct.PayForBills);	       //��ʾֽ��
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
		 DisplayCoins(UserAct.PayForCoins);	       //��ʾӲ��
		 /*��ʾ�û�ˢ�����*/
		 DisplayCards(UserAct.PayForCards);	       //��ʾˢ��
		 /*��ʾ�ܽ��*/	  
	     DisplayBills(UserAct.PayForBills);	       //��ʾֽ��
		 /*��ʾ�ͻ�Ͷ�ҵ�Ӳ����*/
	     DisplayCoins(UserAct.PayForCoins);	       //��ʾӲ��
		 /*��ʾ�û�ˢ�����*/
		 /*��ʾ�ܽ��*/		 
	     DisplayMealPrice(UserAct.PayShould);      //��ʾ�ܼ۸�
		 BackPolls();
//		CloseTIM3(); 
//	    PictrueDisplay(HOME_PAGE);	    
//	    cancel(); 	
//		CloseMoneySystem();
//		TemperatureDisplay(Temperature);
//	    CurrentPoint = 0; 
	
	}break;																			 
	 	case 0x1C: //�����˸��
	{
	    if(PassWordLen > 0)
		{
	        PassWord[PassWordLen] = 0;
	        PassWordLen--;
		    DisplayPassWord(PassWordLen);
		}
		
	}break;

	case 0x1D:	//����ȷ��
	{
	    LCDstructure.LCD_Temp = 0 ;
	    GetPassWord(PassWord);
    	if(VerifyPassword(PassWord, InputPassWord,6) == 0)
		{
		    /*������֤���󣬽��������������*/
		    PictrueDisplay(66);
			PassWordLen = 0;
			
		}
		else
		{
		    /*���� ����Ա����*/
		    PictrueDisplay(68);
		    /*��ʾ��Ŀ*/
			LCDstructure.LCD_Temp = 1 ;
			DisplayMealCount();
			TemperatureDisplay(Temperature);
			PassWordLen = 0;
		}

	}break;
	case 0x1E: //���뷵��
	{
	    LCDstructure.LCD_Temp = 1 ;
		PictrueDisplay(HOME_PAGE);
		TemperatureDisplay(Temperature);
		PassWordLen = 0;
		DisplayMealCount();
	}break;

	 
   	case 0x1F:	          //��Ӧ���� 0
	case 0x20:			  //��Ӧ���� 1
	case 0x21:			  //��Ӧ���� 2
	case 0x22:			  //��Ӧ���� 3
	case 0x23:			  //��Ӧ���� 4
	case 0x24:			  //��Ӧ���� 5
	case 0x25:			  //��Ӧ���� 6
	case 0x26:			  //��Ӧ���� 7
	case 0x27:			  //��Ӧ���� 8
	case 0x28:			  //��Ӧ���� 9
	case 0x29:			  //��Ӧ���� A
	case 0x2A:			  //��Ӧ���� B
	case 0x2B:			  //��Ӧ���� C
	case 0x2C:			  //��Ӧ���� D
	case 0x2D:			  //��Ӧ���� E
	case 0x2E:			  //��Ӧ���� F
	{
	    if(PassWordLen < 6)
		{
		    InputPassWord[PassWordLen] = (cmd - 0x1F);
		    PassWordLen++;
			DisplayPassWord(PassWordLen);

		}
	}break;


	case 0x30:	 //��һ��+
	{
	   if(CurFloor.FCount < 3)
		{
		    CurFloor.FCount++;
			CurFloor.MealCount++;
			DisplayFColumn(CurFloor.FCount);
		}
	}break;
	case 0x31:   //�ڶ���+
	{
	 	if(CurFloor.SCount < 3)
		{
		    CurFloor.SCount++;
			CurFloor.MealCount++;
			DisplaySColumn(CurFloor.SCount);
		}
	
	}break;
	case 0x32:	 //������+
	{
	  if(CurFloor.TCount < 3)
		{
		    CurFloor.TCount++;
			CurFloor.MealCount++;
			DisplayTColumn(CurFloor.TCount);
		}

	}break;
		case 0x33:	 //��һ��-
	{  
	  	if(CurFloor.FCount > 0)
		{
		    CurFloor.FCount--;
			CurFloor.MealCount--;
			DisplayFColumn(CurFloor.FCount);
		}

	}break;
	case 0x34:	 //�ڶ���-
	{
	 	if(CurFloor.SCount > 0)
		{
		    CurFloor.SCount--;
			CurFloor.MealCount--;
			DisplaySColumn(CurFloor.SCount);
		}

	}break;
		case 0x35:	 //������-
	{
	     if(CurFloor.TCount > 0)
		{
		    CurFloor.TCount--;
			CurFloor.MealCount--;
			DisplayTColumn(CurFloor.TCount);
		}
	}break;
	case 0x36:	 //����+
	{
	   if(CurFloor.FloorNum < FloorMealNum)
		{
		    CurFloor.FloorNum++;
			DisplayFloor(CurFloor.FloorNum);
			InitSetting();
		}
	}break;
	case 0x37:	 //����-
	{
	    if(CurFloor.FloorNum > 1)
		{
		    CurFloor.FloorNum--;
			DisplayFloor(CurFloor.FloorNum);
			InitSetting();
		}
	}break;  
	case 0x38:	 //ID��+
	{
	  	    if(CurFloor.MealID < 9)
		{
		    CurFloor.MealID++;
			DisplayMealID(CurFloor.MealID);
			InitSetting();
		}
	}break;
	case 0x39:	 //ID��-
	{
	    if(CurFloor.MealID > 1)
		{
		    CurFloor.MealID--;
//			DisPlayChar(Display_MealID,sizeof(Display_MealID),CurFloor.MealID);
			DisplayMealID(CurFloor.MealID);
			InitSetting();
		}
	}break;
		case 0x3A:	 //����
	{

        CurFloor.FCount = 3;
		CurFloor.SCount = 3;
		CurFloor.TCount = 3; 
		CurFloor.MealCount = 9;
		DisplayFColumn(CurFloor.FCount);
		DisplaySColumn(CurFloor.SCount);
		DisplayTColumn(CurFloor.TCount);	  
	}break;
		case 0x3B:	 //���
	{				 
	    CurFloor.FCount = 0;
		CurFloor.SCount = 0;
		CurFloor.TCount = 0; 
		CurFloor.MealCount = 0;
		DisplayFColumn(CurFloor.FCount);
		DisplaySColumn(CurFloor.SCount);
		DisplayTColumn(CurFloor.TCount);
	}break;
		case 0x3C:	 //ȷ��
	{
	   //printf("CurFloor.MealID=%d\r\n",CurFloor.MealID);
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
		for(j = 0; j < 9; j++)	 //9�����ID��
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
				//	printf("DefineMeal[%d].Position[%d][0]=%d\r\n",j,i,DefineMeal[j].Position[i][0]);
			    }
		    }
		}
//		    for(i=0;i<90;i++)
//			{
//			 printf("FloorMealMessageWriteToFlash.FlashBuffer[%d]=%d\r\n",i,FloorMealMessageWriteToFlash.FlashBuffer[i]);
//			}
		           WriteMeal();
//		            DataWriteStmFlash(0X080f0002,FloorMealMessageWriteToFlash.FlashBuffer,FloorMealNum*6);
	                for(i=0;i<9;i++)
					;
              //     printf("DefineMeal[%d].MealCount=%d\r\n",i,DefineMeal[i].MealCount);
	}break;
		case 0x3D:	 //ȡ��
	{

		CurFloor.FCount    = 0;
		CurFloor.SCount    = 0;
		CurFloor.TCount    = 0;
		CurFloor.MealID    = 0;
		CurFloor.MealCount = 0;
		CurFloor.FloorNum  = 0;	
		LCDstructure.LCD_Temp = 1 ;
		PictrueDisplay(HOME_PAGE);
		DisplayMealCount();	
		TemperatureDisplay(Temperature); 
	    
	}break;
		case 0x3E:	 //����
	{
	   
		CurFloor.FCount    = 0;
		CurFloor.SCount    = 0;
		CurFloor.TCount    = 0;
		CurFloor.MealID    = 0;
		CurFloor.MealCount = 0;
		CurFloor.FloorNum  = 0;	 
		LCDstructure.LCD_Temp = 1 ;
	    PictrueDisplay(SETMEAL_PAGE);
		DisplayMealCount();
		TemperatureDisplay(Temperature); 
	}break;
	case 0x42:	 //���ò˵�ID
	case 0x43:   
	case 0x44:	 
	case 0x45:	 
	case 0x46:	 
	case 0x47:	 
	case 0x48:	 
	case 0x49:  
	case 0x4A:
	{
	   /*�������ý���*/
	   LCDstructure.LCD_Temp = 0;
	    PictrueDisplay(INPUTMEAL_PAGE);
		/*�û�ѡ��ķ���ID*/
		UserAct.MealID     = cmd -65;	
		/*��ǰ��������*/
		CurFloor.FCount    = 0;
		CurFloor.SCount    = 0;
		CurFloor.TCount    = 0;
		CurFloor.MealID    = UserAct.MealID;
		CurFloor.MealCount = 0;
		CurFloor.FloorNum  = cmd -65;
		DisplayMealID(CurFloor.MealID);
		DisplayFloor(CurFloor.FloorNum); 
		/*�жϵ�ǰ�к����Ƿ��ѽ�����*/
		CurFloor.FCount = FloorMealMessageWriteToFlash.FloorMeal[UserAct.MealID-1].FCount;
		CurFloor.SCount = FloorMealMessageWriteToFlash.FloorMeal[UserAct.MealID-1].SCount;
		CurFloor.TCount = FloorMealMessageWriteToFlash.FloorMeal[UserAct.MealID-1].TCount;
		CurFloor.MealCount = FloorMealMessageWriteToFlash.FloorMeal[UserAct.MealID-1].MealCount;
   		DisplayFColumn(CurFloor.FCount);
		DisplaySColumn(CurFloor.SCount);
		DisplayTColumn(CurFloor.TCount);

	}break;
	case 0x4B:   //�Ų�����
	{	 
	   /*������ý���Ų�����*/
	   	LCDstructure.LCD_Temp = 0 ;
	    PictrueDisplay(INPUTMEAL_PAGE);
		UserAct.MealID     = 1;		 //�ͻ�ѡ��Ĳ˵���
		CurFloor.FCount    = 0;		 //��һ�е���Ŀ
		CurFloor.SCount    = 0;		 // ��2�����Ŀ
		CurFloor.TCount    = 0;	     //��3�����Ŀ
		CurFloor.MealID    = 1;	     //�ͻ�ѡ��Ĳ˵���
		CurFloor.MealCount = 0;		 //��ǰ�������
		CurFloor.FloorNum  = 1;		 //��ǰ����һ��
	    DisplayMealID(CurFloor.MealID);
		DisplayFloor(CurFloor.FloorNum);   
		DisplayFColumn(FloorMealMessageWriteToFlash.FloorMeal[CurFloor.MealID-1].FCount);
		DisplaySColumn(FloorMealMessageWriteToFlash.FloorMeal[CurFloor.MealID-1].SCount);
		DisplayTColumn(FloorMealMessageWriteToFlash.FloorMeal[CurFloor.MealID-1].TCount);
           
   
   }break;
  	case 0x4C:	 //�Ų�����ȡ��
	{
	   /*��ʾ��Ҫ����Ŀ���¶�*/
	   	LCDstructure.LCD_Temp = 1 ;	
	    PictrueDisplay(HOME_PAGE);
	    DisplayMealCount();
		TemperatureDisplay(Temperature);	
	}break;
	case 0x4D:	 //IC����ֵ����
	{
       	if(BillActionFlag == 1)
		{
	    CurrentPoint = 0 ;
	    ReturnMessage = 6 ; 
		PictrueDisplay(HOME_PAGE);
	    DisplayMealCount();
		TemperatureDisplay(Temperature);
		}
		else
		ReturnMessage = 4; 
	}break;
	case 0x4E:	 //IC����ֵȷ�� 
	{
		//д���ݽ���
		ReturnMessage = 5;

	}break;
	case 0x4F:	 //IC��������ֵ
	{
	//��ֽ�һ�
	 PlayMusic(VOICE_7);
	 DisplayRechangeBills(UserAct.RechxargeCounter);
 	 ReturnMessage = 3 ;  
 	 
	}break;
	case 0x50:	//�¶ȿ��ذ�ť
	{
	 
	}break;
	 
	case 0x51 :   /*ѡ���ֽ�֧��*/
	{
	 	   CurrentPoint = 3; 
		   ReturnMessage =  1;
	}
	break ;
	case 0x52:  /*���п�֧��*/
	{
	      ReturnMessage =  1;
		  CurrentPoint = 7; 

	} break;
	case 0x53:  /*ѡ������֧ͨ��*/
	{

	     CurrentPoint =  8;
       ReturnMessage =  1;
	} break;
	case 0x54:  /*�Ƿ��ӡСƱ--��*/
	{ 
	    /*�ͻ�Ҫѡ���Ʊ*/
		
         UserAct.PrintTick = 1 ;
		 ReturnMessage =  2;
	 
	} break;
   	case 0x55:  /*�Ƿ��ӡСƱ--��*/
	{	 
	     /*�ͻ���ѡ���Ʊ*/
	      UserAct.PrintTick = 2; 
		    ReturnMessage =  2;
	} break;
	 case 0x56:  /*�˱ҹ���*/
	{
	     ReturnMessage =  9;
	} break;
	case 0x57 :	 //ѡ���������
	 {
	   	LCDstructure.LCD_Temp = 1 ;	
	    BackPolls();
	  	CloseTIM3(); 
	    PictrueDisplay(HOME_PAGE);	    
	    cancel(); 	
		CloseMoneySystem();
	    TemperatureDisplay(Temperature);
	    CurrentPoint = 0; 
	 }
	 break;
  default : break;
} 
 //printf("LCDstructure.LCD_Temp=%d\r\n",LCDstructure.LCD_Temp);
return ReturnMessage ;

 }
