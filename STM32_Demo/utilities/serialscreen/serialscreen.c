 /**
  ******************************************************************************
  * �ļ�: serialscreen.c
  * ����: jackey
  * �汾: V1.0.0
  * ����: DWIN ������ͨ�ų���
  *  
  ******************************************************************************
	**/
#include <string.h>
#include "stm32f4xx.h"
#include "serialscreen.h"
#include "uart3.h"

/*֡��һЩ�ֽڶ���*/
#define FH0 0xA5 
#define FH1 0x5A
#define RD_LEN 0x00
#define ADR 0x00

/*�Ĵ����ռ䶨��*/
#define Version 0x00  /*DGUS�汾��*/
#define LED_NOW 0X01  /*LED���ȿ��ƼĴ�����0x00~0x40*/
#define BZ_IME  0X02  /*���������п��ƼĴ�������λ10ms*/
#define PIC_ID  0x03  /*2�ֽ� ������ǰ��ʾҳ��ID д���л���ָ��ҳ��(ҳ�����)*/
#define TP_Flag 0x05  /*0x5a=�����������и��£���������������δ����*/
#define TP_Status 0x06  /*0x01=��һ�ΰ��� 0x03һֱ��ѹ�� 0x02=̧�� ����=��Ч*/
#define TP_Position 0x07 /*4�ֽ� ��������ѹ����λ�ã�X_H:L Y_H:L*/
#define TPC_Enable 0x0B /*0x00=���ز����� ����=����*/

/*ʣ���Ʒ������ַ*/
#define carrot     0x0000 /*���ܲ�����*/
#define egg 	   0x0001 /*�˸��嵰*/
#define potato     0x0002 /*����ţ��*/
#define mushroom   0x0003 /*�㹽����*/
#define fish       0x0004 /*�������*/
#define chicken    0x0005 /*�㹽����*/
#define duck       0x0006 /*��Ƥ��Ѽ*/
#define pork       0x0007 /*÷�˿���*/
#define roast      0x0008 /*���Ʋ���*/

/*������16λ�洢����ַ�ֽ�Ϊ2��8λ����*/
union ScreenRam
{
		int16_t	 	adress ;
		int8_t  	adr[2] ;
}myunion;


/*дָ���Ĵ���֡*/
const unsigned char RegisterWrite[]={ 
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x00 , /*���� �������������*/
              0x80 , /*ָ����ַд�Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							0x00 , /*����*/
							0x00 , 0x00	/*CRCУ����β*/
							};

/*��ָ���Ĵ���֡*/
const unsigned char RegisterRead[8]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x03 , /*���� �������������*/
              0x81 ,  /*ָ����ַ���Ĵ�������*/
							ADR  , /*�Ĵ�����ַ*/
							RD_LEN , /*����*/
							0x00 , 0x00	/*CRCУ����β*/	
							};
	
/*д�����洢��֡*/							
const unsigned char VariableWrite[]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x00 , /*���� �������������*/	
							0x82 , /*ָ����ַ��ʼд�����ݴ�(������)�������洢��*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							0x00 , /*����*/
							0x00 , 0x00  	/*CRCУ����β*/
							}; 

/*�������洢��֡*/	
const unsigned char VariableRead[9]={
							FH0 , FH1 , /*֡ͷ2�ֽ� */
							0x04 , /*���� �������������*/
							0x83 , /*�ӱ����洢��ָ����ַ��ʼ����RD_LEN����������*/
							0x00 , 0x00 ,	/*�����洢�ռ�2�ֽ�*/
							RD_LEN , /*����*/
							0x00 , 0x00  	/*CRCУ����β*/
							};										

 /*******************************************************************************
 * ��������:PageChange                                                                     
 * ��    ��:�����л�ͼƬ����                                                              
 *                                                                               
 * ��    ��:page                                                                    
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 							
void PageChange(char page)
{
		unsigned char *temp;
		memcpy(temp,RegisterWrite,sizeof(RegisterWrite));
		temp[2]=	3;		
		temp[4]=	PIC_ID;	
	    temp[5]=  page;
		Uart3_Send(temp,sizeof(temp));
}	

 /*******************************************************************************
 * ��������:DispLeftMeal                                                                     
 * ��    ��:�ڲ˵�ѡ�������ʾ����Ʒ��ʣ���                                                           
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                    
 *******************************************************************************/ 	

void DispLeftMeal(void)
{
		uint8_t i;
	    unsigned char *temp;  //��Ŵ������ݵ���ʱָ��
		for(i=0;i<9;i++){		
		memcpy(temp,VariableWrite,sizeof(VariableWrite));
		temp[2]= 4;
		myunion.adress= carrot+i; //�ڻ���ַ����λ��
		temp[4]= myunion.adr[0];
		temp[5]= myunion.adr[1];
		Uart3_Send(temp,sizeof(temp));	
		}

}

 /*******************************************************************************
 * ��������:DealSeriAceptData                                                                   
 * ��    ��:����ӵ�����Ļ���ص�����                                                        
 *          ���ʵ���������������ݣ�0xA5 0XA5 0X5A                                                                           
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2014��3��13��                                                                   
 *******************************************************************************/ 	
void DealSeriAceptData(void)
{
	unsigned char i;
	unsigned char temp,temp1,length,checkout;
	char RegisterAdress=0;
	int16_t VariableAdress=0;
	unsigned char Rx3DataBuff[30];/*����һ�������С��?����Խ��(out of bounds),?�ɱ䳤������*/
	char RegisterData[]={0};  //?
	char VariableData[]={0};  //?
	while(UART3_GetCharsInRxBuf()>0) //��ȡ��������С��ֱ��������������
	{	
		if(USART3_GetChar(&temp)==1)
		{	
			if(temp==FH0)	//ƥ��֡ͷ��һ���ֽ�
			{ 
loop:			if(USART3_GetChar(&temp1)==1)  
				{
					if(temp1==FH1)  //ƥ��֡ͷ�ڶ����ֽڣ�
					{
						if(USART3_GetChar(&length)==1)  //��ȡ�����ֽڳ���
						{
							checkout =1;//��λ��ʼ��ȡ���������ݱ�־	
						}	
					}
					else if(temp1==FH0)	goto loop; //�����������0xA5������ж���һ���ֽ��Ƿ�Ϊ0x5A
				}
			}
			
		}
	 if(checkout==1) //ֱ����ȡ���ݳ��ȣ�Ȼ�������
	 {
	 	checkout =0;  //��λ��־
		for(i=0;i<length;i++) USART3_GetChar(Rx3DataBuff+i);
		if(Rx3DataBuff[0]==0x81)  //���Ĵ�����������
		{
			RegisterAdress =Rx3DataBuff[1];
			for(i=0;i<(length-2);i++)
			RegisterData[i]=Rx3DataBuff[2+i];
			//�����޸�������ݵĹ���
		}
		else if(Rx3DataBuff[0]==0x83) //�����ݴ洢����������
		{
			myunion.adr[0] =Rx3DataBuff[1];
			myunion.adr[1] =Rx3DataBuff[2];
			VariableAdress =myunion.adress;
			for(i=0;i<(length-3);i++)
			VariableData[i]=Rx3DataBuff[3+i];
			////�����޸ı�����ַ���ݵĹ���
		}
	  for(i=0;i<length;i++) Rx3DataBuff[i]=0x00; //����������������Ϳ��Բ�Ҫ�˶Σ����û�м��ϾͿ��Խ�����������

	}
	}		
}

