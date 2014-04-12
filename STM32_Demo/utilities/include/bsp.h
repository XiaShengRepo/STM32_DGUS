#ifndef _bsp_H
#define _bsp_H

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "MsgHandle.h"	         //������ڵ�͵�����
#include "vioce.h"			       //��������
#include "serialscreen.h"	       //����������
//#include "led.h"			       //led�ƺ���
//#include "ICcard.h"			       //ic������
//#include "timer2.h" 				//���ڵ���ʱ
//#include "timer4.h"			       //��ʱ��4 ������ʾ�¶ȵ� û���ٴη���һ���źŸ��ӻ��õ��¶ȡ�Ȼ������ʾ��
//#include "timer3.h"		           //�������
//#include "timer6.h"				   //���ڲɼ��¶�
#include "delay.h"			       //��ʱ
#include "uart3.h"				   //�������ӿ�
#include "uart4.h"           //ֽ�һ��ӿ�

//#include "led.h"				   //led��
#include "coin.h"				     //Ӳ�һ�
#include "mini_hopper.h"     //�˱һ�
#include "stdio.h"				   //��׼�⺯��	 
#include "string.h"				   //��׼�⺯��	
#include "stdint.h"          //��������ͱ���
#include "bill.h"				     //ֽ�һ�
#include "printer.h"				 //��ӡ��
#include "uart1.h"				   //��ӡ���ӿ�
#include "ff.h"
#include "integer.h"
#include "diskio.h"
	
#include "rtc.h"
#include "sd.h"
#include "uart5.h"
#include "spi_flash.h"  
//#include "szt_gpboc.h"       //����ͨ������

#include "network_handle.h"    //��̨�������ݵĴ���
#include "uart5.h"				     //�������ݴ���ӿ�

typedef enum
{
  Status_OK= 0 ,	   //���������ɹ� ���� 0
  Status_Error ,	  // ��������  ����1
	Status_Action 	  // ������ִ�� ���� 2 
} status ;



typedef struct 
{
  uint8_t   F_RX_Length ;   //�������ݳ��ȱ�־
  uint8_t   Response[6];     //ACK����
  uint8_t   F_RX1_SYNC;     //�Ƿ�ʼ������Ч����
  uint8_t   PacketData[1000]; //���ܵ�����
  uint8_t   PacketDCS;       //У����
  uint16_t  DataLength;    //���ݳ���
  uint16_t   RX_NUM ;	    //��λ
  uint8_t   Status   ;		 //״̬
  uint8_t   Checkoutright ;	  //����У���Ƿ�ɹ�
  uint8_t   ACKStatus;
}USART_StatusTypeDef;

extern  USART_StatusTypeDef  STATUS ;



extern uint8_t   CoinCountFlag;
extern uint8_t   TemperatureFlag  ;
extern uint8_t   BillActionFlag;      //��ʾֽ�һ�����Ǯ�С����ʱ���ܹر�ֽ�һ�
extern uint8_t   CurrentPoint ;
extern uint8_t   CoinFlag;			 //�����˱һ��ı�־
extern uint8_t   TemperatureCur;


bool CloseCashSystem(void);
bool OpenCashSystem(void);	
uint8_t FindMeal(MealAttribute *DefineMeal) ;
// uint8_t CanRecvCmd(uint8_t p);
unsigned char  WaitPayMoney(void);
uint8_t WaitMeal(void);
void hardfawreInit(void);
void  WaitTimeInit(uint8_t *Time);
// void LcdHandler(void);
// void  TempHandler(void);
// void ClearingFuntion(void) ;//��ǩ����
#endif
