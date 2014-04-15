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
	p0[5]=UserAct.PayShould/100+'0';
	p0[6]=UserAct.PayShould%100/10+'0';
	p0[7]=UserAct.PayShould%100%10+'0';

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

}

 /*******************************************************************************
 * ��������:CheckPrintStatus                                                                     
 * ��    ��:����ӡ����״̬�Ƿ���ֽ��                                                                   
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/

 P_stuction Print_Struct;
void  SPRT(uint8_t flag)
{
  unsigned char   SendStc[3]={0x1d,0x56,0x01};		//��ֽ����
	unsigned char  huan3[]={0x1b,0x64,0x06};
	unsigned char  p0[]={"Ӧ��:201����:   �һ�:   \r\n"};
	unsigned char  p1[]={"ʱ��:2013-12-15-12:30\r\n"};
	unsigned char  p2[]={"���ܲ�����1\t15\t015\r\n"};
	unsigned char  p3[]={"���׼嵰1\t15\t015\r\n"};
	unsigned char  p4[]={"����ţ��1\t15\t015\r\n"};
	unsigned char  p5[]={"�㹽����1\t20\t020\r\n"};
	unsigned char  p6[]={"�������1\t20\t020\r\n"};
	unsigned char  p7[]={"�������1\t20\t020\r\n"};
	unsigned char  p8[]={"��Ƥ��Ѽ1\t25\t025\r\n"};
	unsigned char  p9[]={"÷�˿���1\t25\t025\r\n"};
	unsigned char  p10[]={"���Ʋ���1\t25\t025\r\n"};	  

  printf("\r\n");//��ӡ�س�����
  printf("��Ʒ\t����\t����\t���\r\n"); 	
  RTC_TimeShow();//�õ���ǰ��ʱ��
	COPY(TimeDate,p0,p1);
	switch(flag)
	 {
	   case 0:	     
	          p2[10]= Print_Struct.P_Number%10 +'0'; 
			    	p2[15]=UserAct.PayShould/100+'0';
	          p2[16]=UserAct.PayShould%100/10+'0';
	          p2[17]=UserAct.PayShould%100%10+'0';
					  printf("%s",p2);
	        break;   	     	          
     case	1 :	
	          p3[8]= Print_Struct.P_Number%10 +'0'; 
			    	p3[13]=UserAct.PayShould/100+'0';
	          p3[14]=UserAct.PayShould%100/10+'0';
	          p3[15]=UserAct.PayShould%100%10+'0';
	          printf("%s",p3);
	      	break;      	
	   case 2 :	
	          p4[8]= Print_Struct.P_Number%10 +'0'; 
			    	p4[13]=UserAct.PayShould/100+'0';
	          p4[14]=UserAct.PayShould%100/10+'0';
	          p4[15]=UserAct.PayShould%100%10+'0';
					  printf("%s",p4);
					break;  
	   case 3:	
	          p5[8]= Print_Struct.P_Number%10 +'0'; 
			    	p5[13]=UserAct.PayShould/100+'0';
	          p5[14]=UserAct.PayShould%100/10+'0';
	          p5[15]=UserAct.PayShould%100%10+'0';
	   	   	  printf("%s",p5);
					break;   	          
     case	4 :	
	          p6[8]= Print_Struct.P_Number%10 +'0'; 
			    	p6[13]=UserAct.PayShould/100+'0';
	          p6[14]=UserAct.PayShould%100/10+'0';
	          p6[15]=UserAct.PayShould%100%10+'0';
	   	   	  printf("%s",p6);
					break;       	
	   case 5 : 
	          p7[8]= Print_Struct.P_Number%10 +'0'; 
			      p7[13]=UserAct.PayShould/100+'0';
	          p7[14]=UserAct.PayShould%100/10+'0';
	          p7[15]=UserAct.PayShould%100%10+'0';
				    printf("%s",p7);
					break;  
	   case 6:	
	          p8[8]= Print_Struct.P_Number%10 +'0'; 
			      p8[13]=UserAct.PayShould/100+'0';
	          p8[14]=UserAct.PayShould%100/10+'0';
	          p8[15]=UserAct.PayShould%100%10+'0';
	   	   	  printf("%s",p8);
				 break;   	          
    case	7 :
	          p9[8]= Print_Struct.P_Number%10 +'0'; 
			      p9[13]=UserAct.PayShould/100+'0';
	          p9[14]=UserAct.PayShould%100/10+'0';
	          p9[15]=UserAct.PayShould%100%10+'0';
	     		  printf("%s",p9);
					break;  
	   case 8 :	
	          p10[8]= Print_Struct.P_Number%10 +'0'; 
			      p10[13]=UserAct.PayShould/100+'0';
	          p10[14]=UserAct.PayShould%100/10+'0';
	          p10[15]=UserAct.PayShould%100%10+'0';
			    	printf("%s",p10);
					break;  
	   default :break;
	 } 
     printf("%s",p0);
		 printf("%s",p1);
		 if(UserAct.PayType == '2' )
		 {
		    printf("֧����ʽ�����п�֧��\r\n");
		 }
		 	 if(UserAct.PayType == '1')
		 {
		    printf("֧����ʽ���ֽ�֧��\r\n");
		 }
		 	 if(UserAct.PayType == '3')
		 {
		    printf("֧����ʽ������֧ͨ��\r\n");
		 }
		 Uart1_Card(huan3,sizeof(huan3)); 
		 Uart1_Card(SendStc,sizeof(SendStc));//	��ֽ
}
