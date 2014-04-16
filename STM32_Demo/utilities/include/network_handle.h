#ifndef __network_handle__h
#define __network_handle__h
#include "stdint.h"

extern   unsigned char   F_RX1_Right ;
extern   uint16_t		  rx1BufIndex ;
extern   uint8_t		  F_RX1_VAILD ;
extern   long  		    CrcValue ;
extern   uint8_t		  F_RX1_SYNC ;


/*******************************************
����ṹ���Ƕ���9�ֲ�Ʒ�Ķ���
���ò�Ʒ��ʱ�����һ���ṹ�� ��������Ա�

********************************/
typedef struct
{
	
	unsigned char MealID[4];
	unsigned char MaelName[20];
	unsigned char MealNum[2];
	unsigned char MealPreace[4];
	unsigned char MealType[4];
	
}Meal_struction;
extern	Meal_struction  	Meal[4];


/*******************************************
����ṹ�����û�ѡ��ȡ�͵�ʱ����Ҫ�����
********************************/
typedef struct
{
 unsigned char 	 MealID[4] ;        /*��Ʒ��ID*/
 unsigned char   MealNo ;           /*��Ʒ������*/
 unsigned char   DealBalance[6];	/*֧�����*/
 unsigned char   PayType;	        /*֧����ʽ*/
 unsigned char   MealPrice[6];       /*��Ʒ�ļ۸�*/
 unsigned char   Change[6] ;        /*Ӧ���һض���Ǯ*/
 unsigned char   RemainMealNum[2] ; /*ʣ���Ʒ��*/
 unsigned char   MealName; 

}CustomerSel__struction ;
extern CustomerSel__struction CustomerSel;


extern unsigned char  F_RX1_Right;
extern unsigned char  rx1Buf[512];	  //�������ݸ����������������������ݴ������buffer


unsigned char SignInFun(void);
unsigned char MealDataCompareFun(void);
unsigned char SignOutFun(void);/*��ǩ*/
unsigned char StatusUploadingFun(void);/*״̬����*/
unsigned char EchoFun(void);/*�������*/
unsigned char TakeMealsFun(unsigned char *SendBuffer);/*ȡ������*/
unsigned char MealUploadingFun(void);/*���Ͳ�Ʒ����*/
unsigned char ClearingFun(void);/*��������*/
void  EchoFuntion(void (*fptr)(void)) ;
unsigned char 	Resend(unsigned char *p,long lenght);
void WriteToSD_data(void);
void DataUpload(void);
void StateSend(void);
void SignInFunction(void);



#endif

