#ifndef _bsp_H
#define _bsp_H

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
//#include "MsgHandle.h"	           //��������ʱ��
//#include "vioce.h"			       //��������
//#include "serialscreen.h"	       //����������
//#include "led.h"			       //led�ƺ���
//#include "ICcard.h"			       //ic������
//#include "timer2.h" 				//���ڵ���ʱ
//#include "timer4.h"			       //��ʱ��4 ������ʾ�¶ȵ� û���ٴη���һ���źŸ��ӻ��õ��¶ȡ�Ȼ������ʾ��
//#include "timer3.h"		           //�������
//#include "timer6.h"				   //���ڲɼ��¶�
#include "delay.h"			       //��ʱ
#include "uart3.h"				   //������
//#include "led.h"				   //led��
//#include "coin.h"				   //Ӳ�һ�
//#include "MS_d245.h"			   //��ӡ��
#include "stdio.h"				   //��׼�⺯��	 
#include "string.h"				   //��׼�⺯��	 
//#include "bill.h"				   //ֽ�һ�ͷ�ļ�
//#include "printf.h"				   //��Ӧ����
//#include "vioce.h"				   //������ʼ��
#include "ff.h"
#include "integer.h"
#include "diskio.h"
	
#include "rtc.h"
#include "sd.h"
//#include "eeprom.h"
//#include "can.h"
//#include "ds18b20.h"
//#include "japan.h"
//#include "uart5.h"
//#include "spi_flash.h"            //w25q64ͷ�ļ�
//#include "szt_gpboc.h"       //����ͨ������


//   typedef enum
//   {
//      Status_OK= 0 ,	   //���������ɹ� ���� 0
//	    Status_Error ,	  // ��������  ����1
//	    Status_Action 	  // ������ִ�� ���� 2 
//
//   } status ;
//
//
//
// typedef struct 
//{
//  uint8_t   F_RX_Length ;   //�������ݳ��ȱ�־
//  uint8_t   Response[6];     //ACK����
//  uint8_t   F_RX1_SYNC;     //�Ƿ�ʼ������Ч����
//  uint8_t   PacketData[1000]; //���ܵ�����
//  uint8_t   PacketDCS;       //У����
//  uint16_t  DataLength;    //���ݳ���
//  uint16_t   RX_NUM ;	    //��λ
//  uint8_t   Status   ;		 //״̬
//  uint8_t   Checkoutright ;	  //����У���Ƿ�ɹ�
//  uint8_t   ACKStatus;
//}USART_StatusTypeDef;
//
//extern  USART_StatusTypeDef  STATUS ;



//extern uint8_t   CoinCountFlag;
//extern uint8_t   TemperatureFlag  ;
//extern uint8_t   BillActionFlag;      //��ʾֽ�һ�����Ǯ�С����ʱ���ܹر�ֽ�һ�
//extern uint8_t   CurrentPoint ;
//extern uint8_t   CoinFlag;			 //�����˱һ��ı�־
//extern uint8_t   TemperatureCur;
////extern uint8_t   CANStatus;
//extern uint8_t   Current ;
//extern uint8_t   LedCmd  ;
//extern  unsigned int  CoinsCount;	 //���ڽyӋͶ��ûӲ����
// #define CAN_MOVE 1
//#define CAN_GETMEAL 2
//
// #define USER_NO_ACTION   0
//#define USER_WAIT_PAY     1
//#define USER_WAIT_MEAL    2
//#define USER_IC_RECHARGE  3
//
//#define CAN_NOACTION        0   
//#define CAN_SNDCOMMAND      1
//#define CAN_READYGETMEAL    2     //֪ͨȥ����ȡ�͵ķ���
//#define CAN_REGETMEAL       3	  //֪ͨȡ��������ִ��
//#define CAN_MEALARRIVE      4	  //���Ѿ�������Ϳ�
//#define CAN_RELAYDOWNMEAL   5	  //�Ų�֪ͨ����
//#define CAN_RESETCOMPLETION 6	  //��ʼ����λ���
//#define CAN_RESTSTART1      7     //֪ͨ�����嵽���ԭ�������
//#define CAN_RESTSTART2      8     //֪ͨ�����嵽����ԭ�������
//#define CAN_TEMPERATURE     9     //�¶ȿ��ط���
//#define CAN_RESTSTART0		10    //���ǳ����ˣ��˹���λ��
//#define CAN_RESETSUCCESS	11


// void CloseMoneySystem(void);
// void OpenMoneySystem(void);
// uint8_t FindMeal(MealAttribute *DefineMeal) ;
// uint8_t CanRecvCmd(uint8_t p);
// unsigned char  WaitPayMoney(void);
// uint8_t WaitMeal(void);
//void hardfawreInit(void);
// void  WaitTimeInit(uint8_t *Time);
//  void LcdHandler(void);
//  void  TempHandler(void);
//  void ClearingFuntion(void) ;//��ǩ����
#endif
