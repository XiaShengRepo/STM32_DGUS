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
void Uart1_Card(const uint8_t *p,uint8_t sizeData)
{
    uint8_t i;		   
	  for(i=0; i<sizeData; i++)
	  {
        USART_SendData(USART1, p[i]);//����1����һ���ַ�
		    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ȴ��������
 	  }
}								   	
										


/*  ����
 ��Ʒ       ����  ����  ���ܼ�
���ܲ�����    1    15     15
�㹽����      1    20     20
�������      1    25     25
��Ƥ��Ѽ      1    30     30
���ʽ�����п�֧��/����֧ͨ��/�ֽ�֧��
Ӧ��:90  ����:100  �һ�:10
ʱ��:2014-04-16 23:19
*/

const uint8_t PrintInitCmd[2]={0x1b,0x40};          //��ӡ����ʼ��
const uint8_t SetEntryPrintCHinese[2]={0x1c,0x26};	//���뺺�Ӵ�ӡģʽ
const uint8_t SetExitPrintCHinese[2]={0x1c,0x2e};	  //�˳���ӡģʽ
const uint8_t SendStc[3]={0x1d,0x56,0x01};		      //��ֽ����
const uint8_t huan3[3]={0x1b,0x64,0x06};            //������

unsigned char  p0[36]={0};
unsigned char  p1[28]={"ʱ��:2013-12-15 12:30:00\r\n"};
unsigned char  p2[32]={"              1     20    20 \r\n"};
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
  uint8_t CurrentPoint = 0;
	uint8_t Cmd[3]={0x10,0x04,0x04};
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
void COPY(Struct_TD  a,unsigned char *p10,unsigned char *p11)
{
  char temp[10]={0};
  char length=0,length_totoal=0;
  length=sprintf(temp,"Ӧ��:%0dԪ ",UserActMessageWriteToFlash.UserAct.PayShould);
  memcpy(p10,temp,length);
  length_totoal+=length;
  length=sprintf(temp,"�Ѹ�:%0dԪ ",Print_Struct.P_paymoney);
  memcpy(p10+length_totoal,temp,length);
  length_totoal+=length;
  length=sprintf(temp,"�һ�:%0dԪ\r\n",Print_Struct.P_MoneyBack);
  memcpy(p10+length_totoal,temp,length);
			
	p11[5]  ='2';
	p11[6] = '0';
	p11[7] = a.Year     / 10+'0';
	p11[8] = a.Year     % 10+'0';
	p11[10] = a.Month    / 10+'0';
	p11[11] = a.Month    % 10+'0';
	p11[13] = a.Date     / 10+'0';
	p11[14] = a.Date     % 10+'0';
	p11[16] = a.Hours    / 10+'0';
	p11[17] = a.Hours    % 10+'0';
	p11[19] = a.Minutes  / 10+'0';
	p11[20] = a.Minutes  % 10+'0';
	p11[22] = a.Senconds / 10+'0';
  p11[23] = a.Senconds % 10+'0';
}
 /*******************************************************************************
 * ��������:SearchPrintMealID
 * ��    ��:����ID������ֵ��p2, ��ֵ                                                             
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2014��9��19�� 
 *******************************************************************************/
void SearchPrintMealID(uint8_t MealID)
{
	/*�������ֵ��صĲ�ƷID������*/
		switch(MealID)
		{
			case 0x00:break;
			case 0x01:
			{
				memcpy(p2,mealname_1st,10);
			}break;	
			case 0x02:
			{
				memcpy(p2,mealname_2nd,10);
			}break;	
			case 0x03:
			{
				memcpy(p2,mealname_3rd,14);
			}break;	
			case 0x04:
			{
				memcpy(p2,mealname_4th,10);
			}break;	
			case 0x05:
			{
				memcpy(p2,mealname_5th,10);
			}break;	
			case 0x06:
			{
				memcpy(p2,mealname_6th,12);
			}break;	    
			case 0x07:
			{
				memcpy(p2,mealname_7th,8);
			}break;	
			case 0x08:
			{
				memcpy(p2,mealname_8th,10);
			}break;	
			case 0x09:
			{
				memcpy(p2,mealname_9th,10);
			}break;
			case 0x0A:
			{
				memcpy(p2,mealname_10th,10);
			}break;	
			case 0x0B:
			{
				memcpy(p2,mealname_11th,10);
			}break;	
			case 0x0C:
			{
				memcpy(p2,mealname_12th,10);
			}break;	
      case 0x0D:
			{
				memcpy(p2,mealname_13th,10);
			}break;	
      case 0x0E:
			{
				memcpy(p2,mealname_14th,8);
			}break;	
      case 0x0F:
			{
				memcpy(p2,mealname_15th,8);
			}break;	
      case 0x10:
			{
				memcpy(p2,mealname_16th,10);
			}break;	
      case 0x11:
			{
				memcpy(p2,mealname_17th,10);
			}break;	
			default:break;			
		}
}
	

 /*******************************************************************************
 * ��������:SPRT                                                                     
 * ��    ��:��ӡ�û���СƱ                                                                
 *                                                                               
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:unsigned char                                                               
 * �޸�����:2014��9��19�� 
 *******************************************************************************/

P_stuction Print_Struct;
void  SPRT(void)
{
	char num_t[10]={0};
  RTC_TimeShow();//�õ���ǰ��ʱ��
	COPY(TimeDate,p0,p1);
/*--------------��Ʊ����ʼ��----------------------/
  Uart1_Card(PrintInitCmd,sizeof(PrintInitCmd));
  Uart1_Card(SetEntryPrintCHinese,sizeof(SetEntryPrintCHinese));
/--------------��Ʊ������ʹ��--------------------*/
  printf("��Ʒ         ����  ����  ���\r\n");
	if(Print_Struct.P_Number1st>0)
  {
    memset(p2,0x20,34-4);
	  SearchPrintMealID(Print_Struct.P_Type1st);
		sprintf(num_t,"%1d",Print_Struct.P_Number1st);
		memcpy(p2+14,num_t,1);
		sprintf(num_t,"%2d",Print_Struct.P_Price1st*10/Discount);
		memcpy(p2+20,num_t,2);
    sprintf(num_t,"%3d",Print_Struct.P_Cost1st*10/Discount);
		memcpy(p2+25,num_t,3);
		printf("%s",p2);
  }
	if(Print_Struct.P_Number2nd>0)
	{
    memset(p2,0x20,34-4);
	  SearchPrintMealID(Print_Struct.P_Type2nd);
		sprintf(num_t,"%1d",Print_Struct.P_Number2nd);
		memcpy(p2+14,num_t,1);
		sprintf(num_t,"%2d",Print_Struct.P_Price2nd*10/Discount);
		memcpy(p2+20,num_t,2);
    sprintf(num_t,"%3d",Print_Struct.P_Cost2nd*10/Discount);
		memcpy(p2+25,num_t,3);
		printf("%s",p2);
	}
  if(Print_Struct.P_Number3rd>0)
	{
    memset(p2,0x20,34-4);
	  SearchPrintMealID(Print_Struct.P_Type3rd);
		sprintf(num_t,"%1d",Print_Struct.P_Number3rd);
		memcpy(p2+14,num_t,1);
		sprintf(num_t,"%2d",Print_Struct.P_Price3rd*10/Discount);
		memcpy(p2+20,num_t,2);
    sprintf(num_t,"%3d",Print_Struct.P_Cost3rd*10/Discount);
		memcpy(p2+25,num_t,3);
		printf("%s",p2);
	}
	if(Print_Struct.P_Number4th>0)
	{	
    memset(p2,0x20,34-4);    
	  SearchPrintMealID(Print_Struct.P_Type4th);
		sprintf(num_t,"%1d",Print_Struct.P_Number4th);
		memcpy(p2+14,num_t,1);
		sprintf(num_t,"%2d",Print_Struct.P_Price4th*10/Discount);
		memcpy(p2+20,num_t,2);
    sprintf(num_t,"%3d",Print_Struct.P_Cost4th*10/Discount);
		memcpy(p2+25,num_t,3);
		printf("%s",p2);
	}
  //printf("�Ż���Ϣ:%d��\r\n",Discount);  
  printf("%s",p0);
	printf("%s",p1);
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
  printf("�������ߣ�400-0755-677\r\n");
	Uart1_Card(huan3,sizeof(huan3)); 
	Uart1_Card(huan3,sizeof(huan3)); 
	Uart1_Card(SendStc,sizeof(SendStc));//	��ֽ
	Uart1_Card(huan3,sizeof(huan3));  
  printf("����������ʳ�����������޹�˾\r\n");
  printf("\r\n");//��ֽ���У�����ֽ�����϶��	
	printf("\r\n");//��ֽ���У�����ֽ�����϶��	
}

