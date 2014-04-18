 /**
  ******************************************************************************
  * �ļ�: test_lcd.c
  * ����: jackey
  * �汾: V1.0.0
  * ����: DWIN ������������ʵ�ִ�����Ļ�л������ݶ�ȡ
  *       test_lcd
  ******************************************************************************
  *
  *                  	��ΪSTM32F407������ģ��
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
uint8_t Current= 0x01;        //״̬������
uint8_t LinkMachineFlag =0;	  //���е�����ӱ�־��0��ʾû���ӣ�1��ʾ����
int main(void)
{
 	uint16_t temp = 0;
	uint8_t initflag ;
	hardfawreInit(); //Ӳ����ʼ��
  //SendtoServce();  //�ϴ�ǰ���������
  //ReadDatatoBuffer(); //��һ���������������
   /*������  ���ʱ�䣬���±���ʱ��*/
//  EchoFuntion(RTC_TimeRegulate);
//	/*����ǩ��*/
//	SignInFunction();
// 	/*��Ʒ�Ա�����*/
//	MealDataCompareFun();
	Szt_GpbocAutoCheckIn();
	PageChange(Menu_interface); //��ʾѡ�ͽ���
	DispLeftMeal();             //��ʾ��Ʒ����
	if(!CloseCashSystem()) printf("cash system is erro");  //�ر��ֽ����
	LinkTime =0;
	OpenTIM5();
	initflag =0;
	while(1)
  {
		//delay_ms(200);
		DealSeriAceptData();
		//ManipulatorHandler();
    switch(Current)
	  {
	    case current_temperature: /*�¶ȴ�����*/
			{
			  StateSend();
				VariableChage(current_temprature,Temperature);
			}break;
	    case waitfor_money:	 /*�ȴ���Ǯ*/
			{
        if( WaitPayMoney()==Status_OK)
				{
					UserAct.Meal_totoal=UserAct.MealCnt_1st + UserAct.MealCnt_2nd  + UserAct.MealCnt_3rd+ UserAct.MealCnt_4th;
					PageChange(Mealout_interface);
					delay_ms(200);
					if(!CloseCashSystem()) printf("cash system is erro");  //�ر��ֽ����
					//�ı��û���ѡ�͵�����
					Current= data_record;
			  }
			}break;
			case data_record:  /*���ݼ�¼*/
			{
				//���۲͵�����ȫ��д��SD��
         DataRecord();
				 //Current= hpper_out;
				 Current= meal_out;
			}break;
      case hpper_out:	 /*�˱�״̬*/
			{
		    if(UserAct.MoneyBack > 0) //��Ҫ�ұҵ�ʱ�����
		    {
          while(UserAct.MoneyBack > 0) //if >1
          {
             SendOutN_Coin(1);		//�ұ�
	           --UserAct.MoneyBack;
	           delay_ms(200); //��ʱ�úúÿ���
          }
				}
				else  //�����ұҵ�ʱ��ֱ�ӽ������״̬
				{
					Current= meal_out;break;
				}
			  if(OldCoinsCnt>NewCoinsCnt)
		    {
		      delay_ms(2000); //��ʱ�úúÿ���
          UserAct.MoneyBack= OldCoinsCnt- NewCoinsCnt;//
		    }
		    else if(OldCoinsCnt==NewCoinsCnt)
		    {
			    UserAct.MoneyBack= OldCoinsCnt- NewCoinsCnt;//
		    }
			}break;
	    case meal_out:	 /*����״̬*/
			{
			  if( WaitMeal()==Status_OK) //�������
				{
          PageChange(TicketPrint_interface);/*��ӡ������ʾ������*/
			    Current = current_temperature;
				}
			}break;
	    case data_upload:	 /*�����ϴ�*/
	    {
        DataUpload();//����UserAct.ID �ж���Ҫ�ϴ�������

				UserAct.MoneyBack   = 0;
				UserAct.PayAlready  = 0;
		    UserAct.PayForBills = 0;
		    UserAct.PayForCoins = 0;
        UserAct.PayForCards = 0;
			  Current = hpper_out ;
	    }break ;
      case status_upload: /*״̬�ϴ�*/
      {
				Current = current_temperature;
			}
	  }
  }
}
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
