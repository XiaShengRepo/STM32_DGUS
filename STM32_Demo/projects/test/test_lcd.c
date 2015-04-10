 /**
  ******************************************************************************
  * �ļ�: test_lcd.c
  * ����: jackey
  * �汾: V4.0.3
  * ����: ��ְ֮������һ�棬�Ѿ������ȶ���
  *       ���Ҫ����΢��֧������Ҫ������ͨ�Ų���ȫ���޸ģ������н�������д��
  ******************************************************************************
  *
  *                  	��ΪSTM32F407������ģ��
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
uint8_t Current= 0x00;        //״̬������
uint8_t LinkMachineFlag =0;	  //���е�����ӱ�־��0��ʾû���ӣ�1��ʾ����
bool   cash_limit_flag=false;  //��ֹ���ֽ�����ϵͳ
uint8_t waitmeal_status=0;    //�ȴ�ȡ��״̬
uint32_t erro_record=0x00000000; //������λ
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x0001, 0x0002, 0x0003,};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;
uint32_t sellsecond_remain_old=0;
uint8_t cid_data[50]={0};
int main(void)
{
  hardfawreInit(); //Ӳ����ʼ��
	if(SD_Initialize()) AbnormalHandle(sdcard_erro);	//SD�����*/ 
	PageChange(OnlymachieInit_interface);
  if(erro_record&(1<<arm_limit)) 
  {
		AbnormalHandle(arm_limit);//��Ҫ���������ϴ��Ķϵ�
	}
  else 
  {
    delay_ms(1000);    //��ʱ�Ż���е�ָ�λ
	  OnlymachieInit();  //��е�ֳ�ʼ��
  }
	PageChange(SignInFunction_interface);
  if(!EchoFuntion(RTC_TimeRegulate)) 
    AbnormalHandle(network_erro);  /*��������ʱ��,���±���ʱ��*/
  else
    SetScreenRtc();/*������Ļ��RTC*/
	MachineHeatSet();//����ʱ���ж��Ƿ�������
	PageChange(SignInFunction_interface);
	if(!SignInFunction())       
		AbnormalHandle(signin_erro); /*����ǩ��*/
  if(signin_state == 3) DispMenu();
  else  DispMenuNone(); 
	Current= current_temperature;
  DispLeftMeal();             //��ʾ��Ʒ����	
	ErrRecHandle();          //�û����ݶϵ�����ݴ������ϴ�
	PageChange(Szt_GpbocAutoCheckIn_interface);
	delay_ms(1000);
  SendtoServce();          //�ϴ�ǰ���������
	//if(!Szt_GpbocAutoCheckIn()) AbnormalHandle(cardchck_erro);//����ͨǩ��
	Szt_GpbocAutoCheckIn();
 	if((CoinsTotoalMessageWriteToFlash.CoinTotoal<50)||( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)== 0)) 	
 	  AbnormalHandle(coinhooperset_erro); //������Ӳ����С��50 �� Ӳ�һ��������� ���� 
	StatusUploadingFun(0xE800); //�������������ϴ�
	PageChange(Logo_interface);	
	if(!CloseCashSystem())
	{		
		if(!CloseCashSystem())AbnormalHandle(billset_erro);	
	}
  delay_ms(200);
	PageChange(Menu_interface); //��ʾѡ�ͽ���
  PageChange(Menu_interface); //��ʾѡ�ͽ���

	while(1)
  {
		DealSeriAceptData();
		manageusart6data();  
    switch(Current)
	  {
	    case current_temperature: /*�¶ȴ�����*/
			{
        if(sellsecond_remain!=sellsecond_remain_old)
        {
          selltime_hour_r= sellsecond_remain/3600;
          selltime_minute_r= (sellsecond_remain%3600)/60;
          selltime_second_r= (sellsecond_remain%3600)%60;
          VariableChage(wait_sellmeal_hour,selltime_hour_r);
          VariableChage(wait_sellmeal_minute,selltime_minute_r);
          VariableChage(wait_sellmeal_second,selltime_second_r);
          sellsecond_remain_old= sellsecond_remain;
          if(sellsecond_remain==0) 
          {
            PageChange(Menu_interface);
            sellmeal_flag= true;           
          }
        }
				StateSend(); //״̬����
				MachineHeatSet();//���ü��Ȼ�������
				if((LinkTime==1)||(LinkTime==2)||(LinkTime==3))
				{
				  VariableChage(coins_in,CoinsTotoalMessageWriteToFlash.CoinTotoal+CoinTotoal_t);//��ʾ����Ӳ����
					VariableChage(coins_back,Coins_cnt);
				}
				if(LinkTime >=5)
				{
					if(OrderSendLink()==0)//����ʧ��ֱ�ӱ���
          {
						if(OrderSendLink()==0)
						{
							erro_record |= (1<<link_timeout);
							Current= erro_hanle;
            }
          }
				}
				//��ʾ����ʱ,���Ը��߱��Ϊ��ѡ�͵���ʱ���и���
				if(UserActMessageWriteToFlash.UserAct.MealID)
				{
					if(WaitTime==0)
					{					 
						PageChange(Menu_interface);//��ʱ�˳��û���Ʒ����ѡ�����
						WaitTimeInit(&WaitTime);
					}
          else
          {
            if(WaitTime%5==1) 
            {
              if(pageunitil==MealNumChoose_interface)
              {
                if(sellmeal_flag) PlayMusic(VOICE_1); //ֻ�е��ڿ����۲͵�ʱ�򲥷�����     
              }                
            }             
          }
				}
        if(machinerec.redoor ==0) //����״̬
        { 
          machinerec.rerelative = 0;
          PageChange(DoorOpened_interface);//��ʾ���Ž��棬���ܽ����κβ���
        }
        else if(machinerec.redoor ==1) //����״̬
        {
          if(machinerec.rerelative ==1) //����е�����ʵ���λ�ã����л�Ϊ�۲ͽ���
          {
            PageChange(Menu_interface);
            //PageChange(pageunitil); /*��������У�ֱ��ת����ǰҳ��*/
            machinerec.redoor = 2; //���ŵ�״̬��Ϊ����״̬
          }
        }
			}break;
	    case waitfor_money:	 /*�ȴ���Ǯ*/
			{
        if( WaitPayMoney()==Status_OK)
				{
          PageChange(TicketPrint_interface);/*��ӡ������ʾ������*/
					PlayMusic(VOICE_7);					
					CloseTIM3();
					CloseTIM7();
          payfor_meal= UserActMessageWriteToFlash.UserAct.MoneyBackShould; /*���ԣ���һЩ���⣬���ޱҿ��˵�ʱ����Ҫ��Ӧ���˱ҵ���������*/
          UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already =0;
          UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st=0;
          UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd=0;
          UserActMessageWriteToFlash.UserAct.PrintTick=0x00000000; //��ʼΪ0������ʹ��ӡСƱ��Ϊһ��
/*------------------------------ǿ�д�ӦСƱ��ͷ----------------------------------------
          if(UserActMessageWriteToFlash.UserAct.PrintTick==0x00000000)
          {
            UserActMessageWriteToFlash.UserAct.PrintTick= 0x00000001;
            PrintTickFun(&UserActMessageWriteToFlash.UserAct.PrintTick);
            CloseTIM4();
          }
--------------------------------ǿ�д�ӦСƱ��β----------------------------------------*/
					UserActMessageWriteToFlash.UserAct.Cancle= 0x00; //�������
          if(UserActMessageWriteToFlash.UserAct.MoneyBack>0) //���б�Ҫ��ʱ�����˱�
					  Current= hpper_out;
          else
            Current= meal_out;
					UserActMessageWriteToFlash.UserAct.Meal_takeout= 0;	
          VariableChage(mealout_already,UserActMessageWriteToFlash.UserAct.Meal_takeout);	//UI��ʾ					
					if(UserActMessageWriteToFlash.UserAct.PayType == '1') //�ֽ�
					{
						CloseCoinMachine();			    //�ر�Ͷ�һ�	
						delay_ms(500);
						if(!CloseCashSystem()){CloseCashSystem();};// printf("cash system is erro1\r\n");  //�ر��ֽ����
					}
          TrackDateToBuff(); //���佫���ݱ���ˢ����������������
			  }
				SaveUserData();
			}break;
      case hpper_out:	 /*�˱�״̬:�ó����ǰ������һ����10��Ӳ�ң���������˱һ���ת�����*/
			{
				uint16_t i=0,cnt_t=0;
			  uint16_t coins_time=0;
				OldCoinsCnt= CoinsTotoalMessageWriteToFlash.CoinTotoal; //��¼֮ǰ��δ�˱�ʱ���ڵ�Ӳ����
        NewCoinsCnt= UserActMessageWriteToFlash.UserAct.MoneyBackShould; //��¼�˱ҵ�Ӧ��Ҫ�˵�Ǯ�����ԶԶϵ��ʱ�����ݽ��м�¼
        if(CoinsTotoalMessageWriteToFlash.CoinTotoal>=UserActMessageWriteToFlash.UserAct.MoneyBack)	//�����ǻ��ڵ�Ӳ��>Ӧ�˵ı�
				{					
					if(UserActMessageWriteToFlash.UserAct.MoneyBack >0) //��Ҫ�ұҵ�ʱ�����
					{
						coins_time= (UserActMessageWriteToFlash.UserAct.MoneyBack/10); 
						cnt_t =  UserActMessageWriteToFlash.UserAct.MoneyBack%10;		
						UserActMessageWriteToFlash.UserAct.MoneyBack= 0;			
						for(i=0;i<coins_time+1;i++) //һ���˱�10����
						{
							if(i!=coins_time)
							{
								UserActMessageWriteToFlash.UserAct.MoneyBack+= SendOutN_Coin(10);		
							}
							else
							{
								if(cnt_t>0)
									UserActMessageWriteToFlash.UserAct.MoneyBack+= SendOutN_Coin(cnt_t);	
								else
									break;
							}
						} 							
						if(ErrorType ==1) //�����ٴη����˱� 
						{
              erro_record |= (1<<coinhooperset_empty);
							delay_ms(1500); 
							UserActMessageWriteToFlash.UserAct.MoneyBack= SendOutN_Coin(UserActMessageWriteToFlash.UserAct.MoneyBack);//���ж��ٱ�δ��
							if(ErrorType ==1)//�˱һ��ޱҴ���,ֱ�ӽ������״̬
							{
								erro_record |= (1<<coinhooperset_empty);	
								if(erro_record>=(1<<X_timeout))//����������ǵ�һ���˱ҳɹ�������� ����ǻ�е���쳣������Ӧ��Ҫ�˵�Ǯ �� ���˵�Ǯ ֱ�ӽ��д�����
								{
                 //�����Ѿ����˶���Ǯ 
                  UserActMessageWriteToFlash.UserAct.MoneyBackAlready= UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;                                  
                 //����ڶ������˶��ٱ�
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd= UserActMessageWriteToFlash.UserAct.MoneyBackAlready- UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
                  
                 //��Ҫ�ϴ����׽�� = ����û�г��Ĳ�Ʒ���ܼ�+
                   payfor_meal+=(UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd\
                                +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);
                                  	                                 		           
									Current = erro_hanle;	
								}
                else if(UserActMessageWriteToFlash.UserAct.Cancle== 0x01)//�����ȡ������,������������
                {
                  UserActMessageWriteToFlash.UserAct.Cancle= 0x00;
                  Current = erro_hanle;	
                }
								else /*���׳ɹ�����е��û�д���*/
								{
                  //�����ϴ����Ѹ���Ǯ= ���ڸ��˵�Ǯ+ ��֮��������������һ����Ʒ��ʱ���뵱ǰ��Ʒ�ļ۸�
                  payfor_meal= UserActMessageWriteToFlash.UserAct.MoneyBackShould;
                  //�����ܵ����˵�Ǯ
									UserActMessageWriteToFlash.UserAct.MoneyBackAlready= UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;
                  //��һ���˱ҵ����ݱ���
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st = UserActMessageWriteToFlash.UserAct.MoneyBackAlready;     
                  //����һ���˱ҵ����ݸ�ֵ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already�����ϴ�
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already = UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;                      
									Current= meal_out;//�ұҴ����ʱ���Ǽ�������
								}
								SaveUserData();
								break;
							}
							else  //�˱�OK ,���ǽ����ж�
              {
                erro_record &= ~(1<<coinhooperset_empty);//�˱Ҵ�����λ��0 
								if(erro_record>=(1<<X_timeout))//����ǻ�е���쳣��ֱ�ӽ��д�����
								{
                  //�����Ѿ����˶���Ǯ 
                  UserActMessageWriteToFlash.UserAct.MoneyBackAlready = UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;      
                 //����ڶ������˶��ٱ�
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd= UserActMessageWriteToFlash.UserAct.MoneyBackAlready- UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
                 //��Ҫ�ϴ����׽�� = ����û�г��Ĳ�Ʒ���ܼ�+(δ�ϴ���Ǯ����)
                   payfor_meal+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd\
                                 +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);	          
									Current = erro_hanle;	
								}
                else if(UserActMessageWriteToFlash.UserAct.Cancle== 0x01)//�����ȡ������,������������
                {
                  ClearUserBuffer();//����û�����
                  SaveUserData();//�����û�����  
                  Current = current_temperature;	
                }
								else /*�����˱ң���е��û�д���*/
								{
                  //�����ϴ����Ѹ���Ǯ
                  payfor_meal= UserActMessageWriteToFlash.UserAct.MoneyBackShould;
                  //�����ܵ����˵�Ǯ
									UserActMessageWriteToFlash.UserAct.MoneyBackAlready= UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;
                  //��һ���˱ҵ����ݱ���
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st = UserActMessageWriteToFlash.UserAct.MoneyBackAlready;		
                  //����һ���˱ҵ����ݸ�ֵ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already�����ϴ�
                  UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already = UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;    
                  //�ұҴ����ʱ���Ǽ�������
                  Current= meal_out;
								}               
                SaveUserData(); 
              }
						}
            else //�˱�OK ,���ǽ����ж�
            {
              erro_record &= ~(1<<coinhooperset_empty);//�˱Ҵ�����λ��0 
              if(erro_record>=(1<<X_timeout))//����ǻ�е���쳣��ֱ�ӽ��д�����
              {
               //�����Ѿ����˶���Ǯ 
                UserActMessageWriteToFlash.UserAct.MoneyBackAlready = UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;                                
               //����ڶ������˶��ٱ�
                UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd= UserActMessageWriteToFlash.UserAct.MoneyBackAlready- UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
               //��Ҫ�ϴ����׽�� = ����û�г��Ĳ�Ʒ���ܼ�+��û���ϴ���Ǯ������
                payfor_meal+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd\
                              +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);	                 
                Current = erro_hanle;	
              }
              else if(UserActMessageWriteToFlash.UserAct.Cancle== 0x01)//�����ȡ������,������������
              {
                ClearUserBuffer();//����û�����
                SaveUserData();//�����û�����  
                Current = current_temperature;	
              }
              else /*�˱ҳɹ�����е��û�д���*/
              {   
                //�����ϴ����Ѹ���Ǯ����ȡ�͵�ʱ�����ۼ�
                payfor_meal= UserActMessageWriteToFlash.UserAct.MoneyBackShould;
                //�����ܵ����˵�Ǯ
                UserActMessageWriteToFlash.UserAct.MoneyBackAlready = UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;
                //��һ���˱ҵ����ݱ���
                UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st = UserActMessageWriteToFlash.UserAct.MoneyBackAlready;
                //����һ���˱ҵ����ݸ�ֵ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already�����ϴ�
                UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already = UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
                //�ұҴ����ʱ���Ǽ�������
                Current= meal_out;
              }
							SaveUserData();
              break;                
            }              
          }   
          else  //�����ұ�ʱ��ֱ�ӽ������
          {
            //��һ���˱ҵ����ݱ���
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st = UserActMessageWriteToFlash.UserAct.MoneyBackAlready;
            //����һ���˱ҵ����ݸ�ֵ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already�����ϴ�
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already = UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
            Current= meal_out; 
            SaveUserData();
            break;            
          }
        }          
				else //����Ӳ�Ҳ���ʱ,MoneyBack����
				{ 
					if(erro_record>=(1<<X_timeout))//����ǻ�е���쳣��ֱ�ӽ��д�����
					{
           //�����Ѿ����˶���Ǯ 
            UserActMessageWriteToFlash.UserAct.MoneyBackAlready = UserActMessageWriteToFlash.UserAct.MoneyBackShould- UserActMessageWriteToFlash.UserAct.MoneyBack;
           //����ڶ������˶��ٱ�
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd= UserActMessageWriteToFlash.UserAct.MoneyBackAlready- UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;                       
           //��Ҫ�ϴ����׽�� = ����û�г��Ĳ�Ʒ���ܼ�
            payfor_meal+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.MealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.MealPrice_2nd
                          +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.MealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.MealPrice_4th);	                 
            Current = erro_hanle;	
					}
          else
          {
            //��һ���˱ҵ����ݱ���
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st = UserActMessageWriteToFlash.UserAct.MoneyBackAlready;
            //����һ���˱ҵ����ݸ�ֵ��UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already�����ϴ�
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already = UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
            //�ұҴ����ʱ���Ǽ�������	
            Current= meal_out; 
          }
					erro_record |= (1<<coinhooperset_empty);
				}
				SaveUserData();
			}break;
	    case meal_out:	 /*����״̬�����ڳ��ͣ��ѳ�һ�ֲ�Ʒ���������*/
			{
        uint8_t meal_kind=0;//��Ʒ�������
        uint8_t meal_empty_cnt=0;
        uint8_t check_cnt=0; //ˢ�����ϴ�����
        uint8_t check_cnt_t=0;
				waitmeal_status= WaitMeal();  
			  if(waitmeal_status == takeafter_meal) //�������
				{
					if(UserActMessageWriteToFlash.UserAct.MoneyBack>0) //����������UserActMessageWriteToFlash.UserAct.MoneyBack>0 ֱ�ӽ��������
					{
						erro_record |= (1<<coinhooperset_empty);
						Current = erro_hanle; 
					}
					else
					{
						/*�޴�״̬������ﳵ*/
						ClearUserBuffer(); 
            SaveUserData();			
						/*�޴�״̬���뵽�۲ͽ���*/
						PageChange(Menu_interface);
					  Current = current_temperature;
					}
          for(meal_kind=0;meal_kind<MealKindTotoal;meal_kind++)
          {
            if(DefineMeal[meal_kind].MealCount==0)
              ++meal_empty_cnt;
          }
          if(meal_empty_cnt==MealKindTotoal)
          {
            PageChange(Menu_interface+1);//��ʾ��������
            SendtoServce(); //�������Ʒ����Ҫ��֮ǰû���ϴ��������ϴ�
            while(1)
            {
              if(1== UpperGpboc()) ++check_cnt; 
              if(1== SztAutoSend()) ++check_cnt; 
              check_cnt_t++;
              if((check_cnt_t>10)||(check_cnt>=2))//��ʱ�˳���ѭ�� �ɹ��˳���ѭ��
              {
                check_cnt=0;
                check_cnt_t=0;
                break; 
              }
            }
          }
          else
          {
            meal_empty_cnt=0;
          }
				}
				else if(waitmeal_status == tookone_meal)  //ȡ��һ����Ʒ
				{
          payfor_meal+= GetMealLastPrice(UserActMessageWriteToFlash.UserAct.MealID,1);
					Current = data_upload;					
				}
        else if(waitmeal_status == takemeal_erro)
				{
					//�����ܵ�Ӧ�˱���	
				  UserActMessageWriteToFlash.UserAct.MoneyBackShould+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd\
                                                               +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);
          //���㻹�ж��ٱ�û����
          UserActMessageWriteToFlash.UserAct.MoneyBack+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd\
                                                         +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);
					PageChange(Err_interface);
					UserActMessageWriteToFlash.UserAct.Cancle= 0x01;
				  /*����бҽ����˱ң�����ޱҽ��������*/
					if(erro_record&(1<<coinhooperset_empty))
					{
            //����Ҫ�ϴ��Ľ��׽��
            payfor_meal+= (UserActMessageWriteToFlash.UserAct.MealCnt_1st*UserActMessageWriteToFlash.UserAct.LastMealPrice_1st+UserActMessageWriteToFlash.UserAct.MealCnt_2nd*UserActMessageWriteToFlash.UserAct.LastMealPrice_2nd
                          +UserActMessageWriteToFlash.UserAct.MealCnt_3rd*UserActMessageWriteToFlash.UserAct.LastMealPrice_3rd+UserActMessageWriteToFlash.UserAct.MealCnt_4th*UserActMessageWriteToFlash.UserAct.LastMealPrice_4th);	              
            //����Ҫ�ϴ������˱ҵĽ���
            UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_2nd= UserActMessageWriteToFlash.UserAct.MoneyBackAlready- UserActMessageWriteToFlash.UserAct.MoneyPayBack_Already_1st;
            /*�ܵ���Ҫ�ϴ��ı�*/  /*֮ǰδ���ϴ���Ӧ�˵ı�*/        /*����������Ӧ�˵ı�*/                                                                                                         
						Current = erro_hanle;
					}
          else 
					{
					  Current = hpper_out;
					}						
				}
        SaveUserData();			        
			}break;
	    case data_upload:	 /*�����ϴ�*/
	    {  		
        DataUpload(Success);//����UserActMessageWriteToFlash.UserAct.ID �ж���Ҫ�ϴ�������
			  Current = meal_out;		
				SaveUserData();
	    }break ;
      case erro_hanle: /*�쳣״̬����:��Ҫ�Գ���һֱ���ڴ�����*/
      {
				//��ȡ��ǰʱ�䣬����ʾ
				DisplayRecordTime();
        if(erro_record==(1<<coinhooperset_empty))
        {
          AbnormalHandle(coinhooperset_empty);
          erro_record&=~(1<<coinhooperset_empty);
          cash_limit_flag=true;
          PageChange(Menu_interface);
          StatusUploadingFun(0xE800); //����󷵻�����
          SaveUserData();
          Current = current_temperature;
        }
        else
        {
          PollAbnormalHandle(); //�쳣���� һֱ�����쳣�������
          PageChange(Logo_interface);
          StatusUploadingFun(0xE800); //����󷵻�����
          SaveUserData();
          while(1);	         
        }          
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

