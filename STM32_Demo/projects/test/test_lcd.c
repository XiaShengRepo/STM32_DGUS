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

#define current_temperature  0x01
#define waitfor_money        0x02
#define payment_success      0x03
#define hpper_out            0x04
#define print_tick           0x05
#define meal_out             0x06
#define data_upload          0x07
#define data_record          0x08

uint8_t Current= 0x01;  //
int main(void)
{
	hardfawreInit(); //Ӳ����ʼ��
  SendtoServce();  //�ϴ�ǰ���������
//  ReadDatatoBuffer(); //��һ���������������
//   /*��������ʱ�䣬���±���ʱ��*/
//  EchoFuntion(RTC_TimeRegulate);
//	/*����ǩ��*/
//	SignInFunction();	
// 	/*��Ʒ�Ա�����*/
//	MealDataCompareFun();	 
//	Szt_GpbocAutoCheckIn(); 
	PageChange(Menu_interface); //��ʾѡ�ͽ��� 
	DispLeftMeal();             //��ʾ��Ʒ����
	if(!CloseCashSystem()) printf("cash system is erro");  //�ر��ֽ����
	while(1) 
  {		
		DealSeriAceptData();
    switch(Current)
	  {  
	    case current_temperature : /*�¶ȴ�����*/
			{

			}break;
	    case waitfor_money:	  /*�ȴ���Ǯ*/
			{				
        if( WaitPayMoney()==Status_OK)
				{
					Current= data_record;
			  }
				}break;
			case data_record: /*���ݼ�¼*/
			{
				Current= hpper_out;
			}break;
      case hpper_out:	//�˱�״̬
			{
		    if(UserAct.MoneyBack > 0) //��Ҫ�ұҵ�ʱ�����
		    {
          while(UserAct.MoneyBack > 0) //if >1
          {
             SendOutN_Coin(1);		//�ұ�
	           --UserAct.MoneyBack;	
	           delay_ms(100); //��ʱ�úúÿ���			
          }
				}
				else  //�����ұҵ�ʱ��ֱ�ӽ������״̬
				{					
					Current= hpper_out;break;
				}
			  if(OldCoinsCnt>NewCoinsCnt)
		    {
		      delay_ms(1000); //��ʱ�úúÿ���	
          UserAct.MoneyBack= OldCoinsCnt- NewCoinsCnt;// 
		    }
		    else if(OldCoinsCnt==NewCoinsCnt)
		    {
			    UserAct.MoneyBack= OldCoinsCnt- NewCoinsCnt;//
		    }		
			}break;				
	    case meal_out:	 /*����״̬*/
			{
			  if( WaitMeal()==Status_OK) //��һ�βͷ�һ������
			    Current = data_upload;
				else
					Current = current_temperature;
			}break;
	    case data_upload:	 //�����ϴ�
	    {	
			  Current = meal_out ; 
	    }break ; 
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
