#include"bsp.h"

 uint8_t PrintBuf[10];
 uint8_t PrintIndex;

 /*******************************************************************************
 * ��������:Uart1_Card                                                                      
 * ��    ��:����ͨ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
void Uart1_Card(uint8_t *p,uint8_t sizeData)
{
    uint8_t i;		   
	  for(i=0; i<sizeData; i++)
	  {
        USART_SendData(USART1, p[i]);//����1����һ���ַ�
		    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ��������
 	  }
}								   	
										
 unsigned char PrintInitCmd[]={0x1b,0x40}; //��ӡ����ʼ��
 unsigned char SetEntryPrintCHinese[]={0x1c,0x26};	  //���뺺�Ӵ�ӡģʽ
 const unsigned char SetExitPrintCHinese[]={0x1c,0x2e};	  //�˳���ӡģʽ
 unsigned char PrintMessage0[]    ={" ���ڷ�Ī˹�������ܲ������޹�˾"};	  //�˳���ӡģʽ
 unsigned char PrintMessage1[]    ={"��Ʒ     ����     ����     ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename0[]={"���ܲ�����   ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename1[]={"�㹽����     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename2[]={"��Ƥ��Ѽ     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessagename3[]={"�������     ����   ����   ���"};	  //�˳���ӡģʽ
 unsigned char PrintMessage2[]    ={"ˢ����             �ֽ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage3[]    ={"Ӧ�գ�             �һأ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage4[]    ={"ˢ����             Ӧ�գ�       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage5[]    ={"     лл�ݹˣ��뱣������СƱ   "};	  //�˳���ӡģʽ
 unsigned char PrintMessage6[]    ={"          ��ӭ�´λݹ�          "};	  //�˳���ӡģʽ
 unsigned char PrintMessage7[]    ={"   ʱ�䣺2013.08.12 12:30       "};	  //�˳���ӡģʽ
 unsigned char PrintMessage8[]    ={"   �绰��0755-XXXXXXXXX         "};	  //�˳���ӡģʽ


 /*******************************************************************************
 * ��������:CheckPrintStatus                                                                     
 * ��    ��:����ӡ����״̬�Ƿ���ֽ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
uint8_t CheckPrintStatus(void)
{
  uint8_t Cmd[]={0x10,0x04,0x04};
  uint8_t CurrentPoint = 0;
  switch(CurrentPoint)
  {
    case 0:
	  {			
			PrintIndex = 0;
	    Uart1_Card(Cmd,sizeof(Cmd));
	    CurrentPoint = 1;
			delay_ms(100);
	  }break;	  
	  case 1: 
	  {
	    switch(PrintBuf[0])
		  {
			  case 0x00 :	break ;
			  case 0x02 :	break ;
			  case 0x0c :	break ;
			  case 0x10 :	break ;
			  case 0x60 :	return 1;     //break ;  //��ʾû��ֽ��
				default: break;
		  }
    }  
    return 0;//��ʾ����
  }
	return 1;
}
 /*******************************************************************************
 * ��������:COPY                                                                    
 * ��    ��:��ȡʱ���Ǯ������������Ϊ��ӡ������                                                                  
 *                                                                               
 * ��    ��: a , *p0 , *p1                                                                   
 * ��    ��:��                                                                     
 * ��    ��:��                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/
void COPY(Struct_TD  a,unsigned char *p0,unsigned char *p1)
{										
	p0[5]=UserActMessageWriteToFlash.UserAct.PayShould/100+'0';
	p0[6]=UserActMessageWriteToFlash.UserAct.PayShould%100/10+'0';
	p0[7]=UserActMessageWriteToFlash.UserAct.PayShould%100%10+'0';

	p0[13]=Print_Struct.P_paymoney /100+'0';
	p0[14]=Print_Struct.P_paymoney %100/10+'0';
	p0[15]=Print_Struct.P_paymoney %100%10+'0';

	p0[21]=Print_Struct.P_MoneyBack/100+'0';
	p0[22]=Print_Struct.P_MoneyBack%100/10+'0';
	p0[23]=Print_Struct.P_MoneyBack%100%10+'0';
			
	p1[5]  ='2';
	p1[6] = '0';
	p1[7] = a.Year     / 10+'0';
	p1[8] = a.Year     % 10+'0';
	p1[10] = a.Month    / 10+'0';
	p1[11] = a.Month    % 10+'0';
	p1[13] = a.Date     / 10+'0';
	p1[14] = a.Date     % 10+'0';
	p1[16] = a.Hours    / 10+'0';
	p1[17] = a.Hours    % 10+'0';
	p1[19] = a.Minutes  / 10+'0';
	p1[20] = a.Minutes  % 10+'0';
	p1[22] = a.Senconds / 10+'0';
  p1[23] = a.Senconds % 10+'0';
}

 /*******************************************************************************
 * ��������:CheckPrintStatus                                                                     
 * ��    ��:����ӡ����״̬�Ƿ���ֽ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2013��8��28�� 


 ��Ʒ       ����  ����  ���ܼ�
���ܲ�����    1    15     15
�㹽����      1    20     20
�������      1    25     25
��Ƥ��Ѽ      1    30     30
���ʽ�����п�֧��/����֧ͨ��/�ֽ�֧��
Ӧ��:90  ����:100  �һ�:10
ʱ��:2014-04-16 23:19

 *******************************************************************************/


 P_stuction Print_Struct;
void  SPRT(void)
{
  unsigned char   SendStc[3]={0x1d,0x56,0x01};		//��ֽ����
	unsigned char  huan3[]={0x1b,0x64,0x06};
	unsigned char  p0[]={"Ӧ��:201����:   �һ�:   \r\n"};
	unsigned char  p1[]={"ʱ��:2013-12-15-12:30:00\r\n"};
	unsigned char  p2[]={"�������ַ�  1\t016\t016\r\n"};
	unsigned char  p3[]={"����±���ȷ�1\t016\t016\r\n"};
	unsigned char  p4[]={"��ɫ�����ⷹ1\t016\t016\r\n"};
	unsigned char  p5[]={"�ڽ���Ƿ�  1\t016\t016\r\n"};
  unsigned char  p6[]={"�������㷹  1\t018\t018\r\n"};
  unsigned char  p7[]={"�����ﵶ�㷹1\t018\t018\r\n"};
  unsigned char  p8[]={"��଼��Ƿ�  1\t016\t016\r\n"};
  unsigned char  p9[]={"÷�˿��ⷹ  1\t016\t016\r\n"};
  //printf("@\r\n");//��ӡ�س�����
  //Uart1_Card(huan3,sizeof(huan3)); 
  printf("��Ʒ\t   ����\t����\t���\r\n"); 	
  RTC_TimeShow();//�õ���ǰ��ʱ��
	COPY(TimeDate,p0,p1);
  if(Print_Struct.P_Number1st>0)
  {	
		p2[12]= Print_Struct.P_Number1st%10 +'0'; 
		p2[14]= (price_1st*10/Discount)/100+'0';
		p2[15]= (price_1st*10/Discount)%100/10+'0';
		p2[16]= (price_1st*10/Discount)%100%10+'0';
		p2[18]= (UserActMessageWriteToFlash.UserAct.MealCost_1st*10/Discount)/100+'0';
		p2[19]= (UserActMessageWriteToFlash.UserAct.MealCost_1st*10/Discount)%100/10+'0';
		p2[20]= (UserActMessageWriteToFlash.UserAct.MealCost_1st*10/Discount)%100%10+'0';  
		printf("%s",p2);
  }
	if(Print_Struct.P_Number2nd>0)
	{
		p3[12]= Print_Struct.P_Number2nd%10 +'0'; 
		p3[14]= (price_2nd*10/Discount)/100+'0';
		p3[15]= (price_2nd*10/Discount)%100/10+'0';
		p3[16]= (price_2nd*10/Discount)%100%10+'0';
		p3[18]= (UserActMessageWriteToFlash.UserAct.MealCost_2nd*10/Discount)/100+'0';
		p3[19]= (UserActMessageWriteToFlash.UserAct.MealCost_2nd*10/Discount)%100/10+'0';
		p3[20]= (UserActMessageWriteToFlash.UserAct.MealCost_2nd*10/Discount)%100%10+'0';
		printf("%s",p3);
	}
  if(Print_Struct.P_Number3rd>0)
	{
		p4[12]= Print_Struct.P_Number3rd%10 +'0'; 
		p4[14]= (price_3rd*10/Discount)/100+'0';
		p4[15]= (price_3rd*10/Discount)%100/10+'0';
		p4[16]= (price_3rd*10/Discount)%100%10+'0';
		p4[18]= (UserActMessageWriteToFlash.UserAct.MealCost_3rd*10/Discount)/100+'0';
		p4[19]= (UserActMessageWriteToFlash.UserAct.MealCost_3rd*10/Discount)%100/10+'0';
		p4[20]= (UserActMessageWriteToFlash.UserAct.MealCost_3rd*10/Discount)%100%10+'0';
		printf("%s",p4);
	}
	if(Print_Struct.P_Number4th>0)
	{		
		p5[12]= Print_Struct.P_Number4th%10 +'0'; 
		p5[14]= (price_4th*10/Discount)/100+'0';
		p5[15]= (price_4th*10/Discount)%100/10+'0';
		p5[16]= (price_4th*10/Discount)%100%10+'0';
		p5[18]= (UserActMessageWriteToFlash.UserAct.MealCost_4th*10/Discount)/100+'0';
		p5[19]= (UserActMessageWriteToFlash.UserAct.MealCost_4th*10/Discount)%100/10+'0';
		p5[20]= (UserActMessageWriteToFlash.UserAct.MealCost_4th*10/Discount)%100%10+'0';
		printf("%s",p5);
	} 
	if(Print_Struct.P_Number5th>0)
	{		
		p6[12]= Print_Struct.P_Number5th%10 +'0'; 
		p6[14]= (price_5th*10/Discount)/100+'0';
		p6[15]= (price_5th*10/Discount)%100/10+'0';
		p6[16]= (price_5th*10/Discount)%100%10+'0';
    p6[18]= (UserActMessageWriteToFlash.UserAct.MealCost_5th*10/Discount)/100+'0';
		p6[19]= (UserActMessageWriteToFlash.UserAct.MealCost_5th*10/Discount)%100/10+'0';
		p6[20]= (UserActMessageWriteToFlash.UserAct.MealCost_5th*10/Discount)%100%10+'0';
		printf("%s",p6);
	} 
	if(Print_Struct.P_Number6th>0)
	{		
		p7[12]= Print_Struct.P_Number6th%10 +'0'; 
		p7[14]= (price_6th*10/Discount)/100+'0';
		p7[15]= (price_6th*10/Discount)%100/10+'0';
		p7[16]= (price_6th*10/Discount)%100%10+'0';
    p7[18]= (UserActMessageWriteToFlash.UserAct.MealCost_6th*10/Discount)/100+'0';
		p7[19]= (UserActMessageWriteToFlash.UserAct.MealCost_6th*10/Discount)%100/10+'0';
		p7[20]= (UserActMessageWriteToFlash.UserAct.MealCost_6th*10/Discount)%100%10+'0';
		printf("%s",p7);
	}
	if(Print_Struct.P_Number7th>0)
	{		
		p8[12]= Print_Struct.P_Number7th%10 +'0'; 
		p8[14]= (price_7th*10/Discount)/100+'0';
		p8[15]= (price_7th*10/Discount)%100/10+'0';
		p8[16]= (price_7th*10/Discount)%100%10+'0';
    p8[18]= (UserActMessageWriteToFlash.UserAct.MealCost_7th*10/Discount)/100+'0';
		p8[19]= (UserActMessageWriteToFlash.UserAct.MealCost_7th*10/Discount)%100/10+'0';
		p8[20]= (UserActMessageWriteToFlash.UserAct.MealCost_7th*10/Discount)%100%10+'0';
		printf("%s",p8);
	}
	if(Print_Struct.P_Number8th>0)
	{		
		p9[12]= Print_Struct.P_Number8th%10 +'0'; 
		p9[14]= (price_8th*10/Discount)/100+'0';
		p9[15]= (price_8th*10/Discount)%100/10+'0';
		p9[16]= (price_8th*10/Discount)%100%10+'0';
    p9[18]= (UserActMessageWriteToFlash.UserAct.MealCost_8th*10/Discount)/100+'0';
		p9[19]= (UserActMessageWriteToFlash.UserAct.MealCost_8th*10/Discount)%100/10+'0';
		p9[20]= (UserActMessageWriteToFlash.UserAct.MealCost_8th*10/Discount)%100%10+'0';
		printf("%s",p9);
	}  
  printf("%s",p0);
	printf("%s",p1);
  printf("�ۿ���Ϣ������\r\n");
	if(UserActMessageWriteToFlash.UserAct.PayType == '2' )
	{
		 printf("֧����ʽ�����п�֧��\r\n");
	}
  if(UserActMessageWriteToFlash.UserAct.PayType == '1')
	{
		 printf("֧����ʽ���ֽ�֧��\r\n");
	}
	if(UserActMessageWriteToFlash.UserAct.PayType == '3')
	{
		printf("֧����ʽ������֧ͨ��\r\n");
	}
  printf("�������ߣ�400-0755-677");
	Uart1_Card(huan3,sizeof(huan3)); 
	Uart1_Card(huan3,sizeof(huan3)); 
	Uart1_Card(SendStc,sizeof(SendStc));//	��ֽ
	Uart1_Card(huan3,sizeof(huan3));  
  printf("����������ʳ�����������޹�˾\r\n");
  printf("\r\n");//��ֽ���У�����ֽ�����϶��	
	printf("\r\n");//��ֽ���У�����ֽ�����϶��	
}
