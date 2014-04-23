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
uint8_t waitmeal_status=0;    //�ȴ�ȡ��״̬
int main(void)
{
 	uint16_t temp = 0;
	hardfawreInit(); //Ӳ����ʼ��
	printf("hardfawreInit is ok\r\n");  //�ر��ֽ����
	if(!CloseCashSystem()) 
	printf("cash system is erro\r\n");  //�ر��ֽ����
	OnlymachieInit();  //��е�ֳ�ʼ��
	
  //SendtoServce();  //�ϴ�ǰ���������
  //ReadDatatoBuffer(); //��һ���������������
   /*������  ���ʱ�䣬���±���ʱ��*/
  EchoFuntion(RTC_TimeRegulate);
	printf("EchoFuntion is ok\r\n");  //�ر��ֽ����
	/*����ǩ��*/
	SignInFunction();
	printf("SignInFunction is ok\r\n");  //�ر��ֽ����
 	/*��Ʒ�Ա�����*/
	//MealDataCompareFun();
	printf("MealDataCompareFun is ok\r\n");  //�ر��ֽ����
	Szt_GpbocAutoCheckIn();
	printf("Szt_GpbocAutoCheckIn is ok\r\n");  //�ر��ֽ����
	PageChange(Menu_interface); //��ʾѡ�ͽ���
	DispLeftMeal();             //��ʾ��Ʒ����
	while(1)
  {
		//delay_ms(200);
		DealSeriAceptData();
		manageusart6data();
    switch(Current)
	  {
	    case current_temperature: /*�¶ȴ�����*/
			{
				StateSend();
				if(LinkTime >=5)
				{
					temp =0;
					temp = OrderSendLink();  //Ϊ1�ɹ���Ϊ0ʧ��
					VariableChage(current_temprature,Temperature); //5Sһ��
					if(!CloseCashSystem()) printf("cash system is erro\r\n");  //�ر��ֽ����
					printf("temp = %d",temp);
				}
				//��ʾ����ʱ
				if(WaitTime==0)
				{					 
					PageChange(Menu_interface);//��ʱ�˳��û���Ʒ����ѡ�����
					WaitTimeInit(&WaitTime);
				}
				else if(WaitTime!=60)
				{
					VariableChage(count_dowm,WaitTime); //��С�ĳ���������ն���ֱ�ӽ���
				}
			}break;
	    case waitfor_money:	 /*�ȴ���Ǯ*/
			{
				SaveUserData();
        if( WaitPayMoney()==Status_OK)
				{
          PageChange(TicketPrint_interface);/*��ӡ������ʾ������*/
					WaitTime=5;//5S��ʱ
	        OpenTIM4();    
					delay_ms(200);
					if(!CloseCashSystem()) printf("cash system is erro");  //�ر��ֽ����
					//�ı��û���ѡ�͵�����
					Current= data_record;
			  }
			}break;
			case data_record:  /*���ݼ�¼*/
			{
				//���۲͵�����ȫ��д��SD��
         //DataRecord();
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
				else  //�����ұҵ�ʱ��ֱ�ӽ������״̬,
				{
					if(UserAct.Cancle== 0x00) //�ж��ǲ���ȡ������
					{
					  Current= meal_out; 
						break;
					}
					else
					{
						UserAct.Cancle= 0x00;
						Current= current_temperature;
					}
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
	    case meal_out:	 /*����״̬�����ڳ��ͣ��ѳ�һ�ֲ�Ʒ���������*/
			{
				SaveUserData();
				waitmeal_status= WaitMeal();
			  if(waitmeal_status == takeafter_meal) //�������
				{
					PageChange(Menu_interface);
           Current = current_temperature; 
					//������ϣ��������¶ȴ���
				}
				else if(waitmeal_status == tookkind_meal) //ȡ��һ�ֲ�Ʒ
				{
					Current = data_upload;
				}
				else if(waitmeal_status == tookone_meal)  //ȡ��һ����Ʒ
				{
					PageChange(Mealout_interface);//��֪����Ҫ��Щʲô
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
				UserAct.Meal_takeout= 0;
				UserAct.Meal_totoal = 0;
			  Current = current_temperature ;
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
