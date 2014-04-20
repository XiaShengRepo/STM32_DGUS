#ifndef __network_handle__h
#define __network_handle__h
#include "stdint.h"

//#define FH        0
//#define TID       1
//#define BRWN      2
//#define BNO       3
//#define DevArea   4
//#define DevAreaNo 5
//#define DealBalance 6
//#define MealID    7
//#define MealNO    8
//#define MealName  9
//#define MealPrice 10
//#define PayType   11
//#define PayBack   12
//#define LeftMeal  13
//#define MAC       14
//#define CardInfo  15
//#define FL        16
//#define SendFlag  17

extern   unsigned char   F_RX1_Right ;
extern   uint16_t		  rx1BufIndex ;
extern   uint8_t		  F_RX1_VAILD ;
extern   long  		    CrcValue ;
extern   uint8_t		  F_RX1_SYNC ;

extern unsigned char  TID[7];
extern unsigned char  BRWN[7+3];
extern unsigned char  BNO[6];
extern unsigned char  DeviceArea[3+3];
extern unsigned char  DeviceAreaNO[4+3];
extern unsigned char  DeviceStatus[2+3];
extern unsigned char  DealData[7];
extern unsigned char  DealTime[6];
extern unsigned char  MAC[8+3];
extern unsigned char  DealBalance[6+3];
extern unsigned char  MealID[4+3];
extern unsigned char  MealNO[1+3];
extern unsigned char  MealName[20+3];
extern unsigned char  MealPrice[6+3];
extern unsigned char  PayType[1+3];
extern unsigned char  Change[6+3];
extern unsigned char  RemainMealNum[2+3];
extern unsigned char  MName[20+3];
extern unsigned char  APPVersion[8+3];
extern unsigned char  ParaFileVersion[8+3];
extern unsigned char  BDataFileVersion[8+3];
extern unsigned char  ChechStatus[2+3];
extern unsigned char  MID[3+3];
extern unsigned char  TTCount[3+2];
extern unsigned char  TNCT[6+3];
extern unsigned char  TNtype[3+2];
extern unsigned char  TotalChange[3+6];

extern unsigned char  UpdataFlag[4];
extern unsigned char  WordKeyCipher[11];

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

uint16_t mem_copy01(unsigned char *dest, const unsigned char *source, const long s_len);
uint16_t mem_copy00(unsigned char *dest, const unsigned char *source, const long s_len);
unsigned char TakeMealsFun1(unsigned char *SendBuffer);
void memcpy_02(char *dest,char *source,char type);
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
void GetBRWN(void);

unsigned int GetCrc16(unsigned char *bufData,unsigned int sizeData);
void HL_IntToBuffer(const uint16_t int_v, unsigned char *ret_buf);
bool StringToHexGroup1(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);


#endif

