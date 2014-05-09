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
int32_t erro_record=0x00000000; //������λ

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x0001, 0x0002, 0x0003,};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

int main(void)
{
 	uint16_t temp = 0;
	hardfawreInit(); //Ӳ����ʼ��
	printf("hardfawreInit is ok\r\n");  //�ر��ֽ����
	//if(!CloseCashSystem())  
  //OnlymachieInit();  //��е�ֳ�ʼ��
	printf("OnlymachieInit ok\r\n");  //
	//delay_ms(30000);
   /*������  ���ʱ�䣬���±���ʱ��*/
  if(!EchoFuntion(RTC_TimeRegulate)) AbnormalHandle(network_erro);
	printf("EchoFuntion ok\r\n");  //	
	/*����ǩ��*/
	if(!SignInFunction())       AbnormalHandle(signin_erro);
	printf("SignInFunction ok\r\n");  //
  SendtoServce();  //�ϴ�ǰ���������
	printf("SendtoServce");
	/*����ͨǩ��*/
	if(!Szt_GpbocAutoCheckIn()) AbnormalHandle(cardchck_erro);
	printf("Szt_GpbocAutoCheckIn ok\r\n");	
	PageChange(Menu_interface); //��ʾѡ�ͽ���
	delay_ms(1500);
	if(!CloseCashSystem())  AbnormalHandle(billset_erro);	
	DispLeftMeal();             //��ʾ��Ʒ����
	while(1)
  {
		//delay_ms(200);
		DealSeriAceptData();
		manageusart6data();   //
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
					//if(!CloseCashSystem()) printf("cash system is erro\r\n");  //�ر��ֽ����
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
					PlayMusic(VOICE_7);					
					CloseTIM3();
					CloseTIM7();					
					WaitTime=5;//5S��ʱ   
	       	OpenTIM4(); 
					//�ı��û���ѡ�͵�����
					MoneyBack =UserAct.MoneyBack *100 ;  /*����10��*/
			    mealvariety =0; 
					Current= hpper_out;
					UserAct.Meal_takeout= 0;					
					if(UserAct.PayType == '1')
					{
						delay_ms(3000);
						if(!CloseCashSystem()) printf("cash system is erro1\r\n");  //�ر��ֽ����
					}
			  }
			}break;
//			case data_record:  /*���ݼ�¼*/
//			{
//				//���۲͵�����ȫ��д��SD��
//         //DataRecord();
//				 Current= hpper_out;
//				 //Current= meal_out;
//			}break;
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
					if(ErrorType ==1)
					{
						AbnormalHandle(coinhooperset_empty);
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
		      delay_ms((OldCoinsCnt-NewCoinsCnt)/10*1000+1000); //��ʱ�úúÿ���
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
				if(WaitTime==0)
				{
					WaitTimeInit(&WaitTime);
		      PlayMusic(VOICE_8);			
				}
				waitmeal_status= WaitMeal();       
			  if(waitmeal_status == takeafter_meal) //�������
				{
					PageChange(Menu_interface);
					//������ϣ��������¶ȴ���
          UserAct.MoneyBack   = 0;
          UserAct.PayAlready  = 0;
          UserAct.PayForBills = 0;
          UserAct.PayForCoins = 0;
          UserAct.PayForCards = 0;
          UserAct.Meal_takeout= 0;
          UserAct.Meal_totoal = 0;
					//������ﳵ
					ClearUserBuffer();          
					VariableChage(mealout_already,UserAct.Meal_takeout);	//UI��ʾ
					Current = current_temperature;
				}
				else if(waitmeal_status == tookkind_meal) //ȡ��һ�ֲ�Ʒ
				{
					PageChange(Mealout_interface);
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
			  Current = meal_out;		
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

// FLASH_Unlock();

//  /* EEPROM Init */
//  EE_Init();

///* --- Store successively many values of the three variables in the EEPROM ---*/
//  /* Store 0x1000 values of Variable1 in EEPROM */
//  for (VarValue = 1; VarValue <= 0x1000; VarValue++)
//  {
//    EE_WriteVariable(VirtAddVarTab[0], VarValue);
//  }

//  /* read the last stored variables data*/
//  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);


//  /* Store 0x2000 values of Variable2 in EEPROM */
//  for (VarValue = 1; VarValue <= 0x2000; VarValue++)
//  {
//    EE_WriteVariable(VirtAddVarTab[1], VarValue);
//  }

//  /* read the last stored variables data*/
//  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
//  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);


//  /* Store 0x3000 values of Variable3 in EEPROM */
//  for (VarValue = 1; VarValue <= 0x3000; VarValue++)
//  {
//    EE_WriteVariable(VirtAddVarTab[2], VarValue);
//  }

//  /* read the last stored variables data*/
//  EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
//  EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
//  EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);

//  while (1);	
