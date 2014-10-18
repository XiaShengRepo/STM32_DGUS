#ifndef __network_handle__h
#define __network_handle__h
#include "stdint.h"
#include "MsgHandle.h"

#define Success 0x01
#define Failed 0x02


extern   unsigned char   F_RX1_Right ;
extern   uint16_t		  rx1BufIndex ;
extern   uint8_t		  F_RX1_VAILD ;
extern   long  		    CrcValue ;
extern   uint8_t		  F_RX1_SYNC ;
extern   const uint8_t     Discount ;

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
extern unsigned char  TakeMealFlag[3+2];
extern unsigned char  MenuNo[4];
extern unsigned char  UpdataFlag[4];
extern unsigned char  WordKeyCipher[11];
extern unsigned char  PosDevNum[10+3];  //13+18+13+20=64-3*4=
extern unsigned char  PosTenantNum[15+3];
extern unsigned char  PosBatchNum[10+3];
extern unsigned char  PosUserNum[21+3];

/*******************************************
����ṹ���Ƕ���4�ֲ�Ʒ�Ķ���
���ò�Ʒ��ʱ�����һ���ṹ�� ��������Ա�

********************************/
typedef struct
{
  const unsigned char MealNum[2]; //�����õ����ݷ��ڽṹ���ͷ����Щ���ݾͲ���ı�
	const unsigned char MealID[4];
	const unsigned char MealName[20];
	const unsigned char MealPrice[6];
	const unsigned char MealType[4];

}Meal_struction;
extern	const Meal_struction  	Meal[MealKindTotoal];


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

extern char ReadSdBuff[512];
extern char SendHostBuff[128];
extern char ReadBuf[20];

extern unsigned char  F_RX1_Right;
extern unsigned char  rx1Buf[512];	  //�������ݸ����������������������ݴ������buffer

extern uint8_t sell_type[4];  //�洢�۲�ID������
extern const uint8_t sell_type_1st[4]; 
extern const uint8_t sell_type_2nd[4]; 
extern const uint8_t sell_type_3rd[4]; 
extern const uint8_t sell_type_4th[4]; 
extern const uint8_t sell_type_5th[4];
extern uint8_t Menu_interface;//��ǰ��ʾ���۲ͽ���
extern uint8_t Menuset_interface; //��ǰ��ʾ����ͽ���
extern uint8_t Menu_interface;
extern uint8_t MealSet_interface;
extern const char price_1st;
extern const char price_2nd;
extern const char price_3rd;
extern const char price_4th;
extern const char price_5th;
extern const char price_6th;
extern const char price_7th;
extern const char price_8th;
extern const char price_9th;
extern const char price_10th;
extern const char    mealname_1st[12]; 
extern const char    mealname_2nd[12];
extern const char    mealname_3rd[18];
extern const char    mealname_4th[12];
extern const char    mealname_5th[12];
extern const char    mealname_6th[14];
extern const char    mealname_7th[10];
extern const char    mealname_8th[12];
extern const char    mealname_9th[12];
extern const char    mealname_10th[12];

uint16_t mem_copy01(unsigned char *dest, const unsigned char *source, const long s_len);
uint16_t mem_copy00(unsigned char *dest, const unsigned char *source, const long s_len);
unsigned char TakeMealsFun1(unsigned char *SendBuffer);
void memcpy_02(char *dest,char *source,char type);
unsigned char SignInFun(void);
uint32_t  MealDataCompareFun(void);/*��Ʒ���ݶԱ�*/
unsigned char SignOutFun(void);/*��ǩ*/
unsigned char StatusUploadingFun(uint16_t erro_status);//״̬����
unsigned char TemperatureUploadingFun(uint8_t Temperature_t); //�¶�����
unsigned char EchoFun(void);/*�������*/
unsigned char TakeMealsFun(unsigned char *SendBuffer,unsigned char takeout_flag);/*ȡ������*/
unsigned char MealUploadingFun(void);/*���Ͳ�Ʒ����*/
unsigned char ClearingFun(void);/*��������*/
//void  EchoFuntion(void (*fptr)(void)) ;
unsigned char 	Resend(unsigned char *p,long lenght);
void WriteToSD_data(void);
void DataUpload(unsigned char takemeal_flag);
void StateSend(void);
//void SignInFunction(void);
void GetBRWN(void);

unsigned int GetCrc16(unsigned char *bufData,unsigned int sizeData);
void HL_IntToBuffer(const uint16_t int_v, unsigned char *ret_buf);
bool StringToHexGroup1(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength);

bool EchoFuntion(void (*fptr)(void));
bool SignInFunction(void);
#endif

