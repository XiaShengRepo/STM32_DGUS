#ifndef __network_handle__h
#define __network_handle__h
#include "stdint.h"
#include "MsgHandle.h"

#define Success 0x01
#define Failed 0x02

#define MealDataDatalSize  43

extern   uint8_t   F_RX1_Right ;
extern   uint16_t		  rx1BufIndex ;
extern   uint8_t		  F_RX1_VAILD ;
extern   long  		    CrcValue ;
extern   uint8_t		  F_RX1_SYNC ;
extern   const uint8_t     Discount ;

extern uint8_t  TID[7];
extern uint8_t  BRWN[7+3];
extern uint8_t  BNO[6];
extern uint8_t  DeviceArea[3+3];
extern uint8_t  DeviceAreaNO[4+3];
extern uint8_t  DeviceStatus[2+3];
extern uint8_t  DealData[7];
extern uint8_t  DealTime[6];
extern uint8_t  MAC[8+3];
extern uint8_t  DealBalance[6+3];
extern uint8_t  MealID[4+3];
extern uint8_t  MealNO[1+3];
extern uint8_t  MealName[20+3];
extern uint8_t  MealPrice[6+3];
extern uint8_t  PayType[1+3];
extern uint8_t  Change[6+3];
extern uint8_t  RemainMealNum[2+3];
extern uint8_t  MName[20+3];
extern uint8_t  APPVersion[8+3];
extern uint8_t  ParaFileVersion[8+3];
extern uint8_t  BDataFileVersion[8+3];
extern uint8_t  ChechStatus[2+3];
extern uint8_t  MID[3+3];
extern uint8_t  TTCount[3+2];
extern uint8_t  TNCT[6+3];
extern uint8_t  TNtype[3+2];
extern uint8_t  TotalChange[3+6];
extern uint8_t  TakeMealFlag[3+2];
extern uint8_t  MenuNo[4];
extern uint8_t  UpdataFlag[4];
extern uint8_t  WordKeyCipher[11];
extern uint8_t  PosDevNum[10+3];  //13+18+13+20=64-3*4=
extern uint8_t  PosTenantNum[15+3];
extern uint8_t  PosBatchNum[10+3];
extern uint8_t  PosUserNum[21+3];

/******************************************
����ۿ���Ϣ
*******************************************/
extern uint16_t MealNo_1st;
extern uint16_t MealNo_2nd;
extern uint16_t MealNo_3rd;
extern uint16_t MealNo_4th;

extern uint16_t price_1st; 
extern uint16_t price_2nd;
extern uint16_t price_3rd;
extern uint16_t price_4th;

extern uint8_t cashcut_1st;
extern uint8_t cashcut_2nd;
extern uint8_t cashcut_3rd;
extern uint8_t cashcut_4th;

extern uint8_t sztcut_1st;
extern uint8_t sztcut_2nd;
extern uint8_t sztcut_3rd;
extern uint8_t sztcut_4th;

extern uint8_t gboccut_1st;
extern uint8_t gboccut_2nd;
extern uint8_t gboccut_3rd;
extern uint8_t gboccut_4th;

extern uint8_t vipcut_1st;
extern uint8_t vipcut_2nd;
extern uint8_t vipcut_3rd;
extern uint8_t vipcut_4th;

extern uint8_t vchatcut_1st;
extern uint8_t vchatcut_2nd;
extern uint8_t vchatcut_3rd;
extern uint8_t vchatcut_4th;

extern uint8_t Order_1st;
extern uint8_t Order_2nd;
extern uint8_t Order_3rd;
extern uint8_t Order_4th;

/*******************************************
����ṹ���Ƕ���4�ֲ�Ʒ�Ķ���
���ò�Ʒ��ʱ�����һ���ṹ�� ��������Ա�

********************************/
typedef struct _Meal_struction
{
	uint8_t MealID[4];     //��ƷID
	uint8_t MealName[20];  //��Ʒ�� 
  uint8_t MealCnt[2];    //��Ʒ����
	uint8_t MealPrice[6];  //��Ʒ�۸�
	uint8_t MealCut[2];    //�ֽ��ۿ�  
  uint8_t VipCut[2];     //��Ա�ۿ�
  uint8_t SztCut[2];     //����ͨ�ۿ�
  uint8_t GbocCut[2];    //�����ۿ�
  uint8_t VchatCut[2];   //΢���ۿ�
  uint8_t MealOrder[1];  //��Ʒ˳��
	uint8_t MealType[4];   //��Ʒ����
}Meal_struction;

union _Meal_Union
{
  Meal_struction Meal[MealKindTotoal];
  uint8_t MealBuf[MealKindTotoal*47];
};
extern union _Meal_Union Meal_Union;

/*******************************************
����ṹ�����û�ѡ��ȡ�͵�ʱ����Ҫ�����
********************************/
typedef struct
{
 uint8_t 	 MealID[4] ;        /*��Ʒ��ID*/
 uint8_t   MealName[20];      /*��Ʒ������*/
 uint8_t   MealNo ;           /*��Ʒ������*/
 uint8_t   DealBalance[6];	  /*֧�����*/
 uint8_t   PayType;	          /*֧����ʽ*/
 uint8_t   MealPrice[6];      /*��Ʒ�ļ۸�*/
 uint8_t   Change[6] ;        /*Ӧ���һض���Ǯ*/
 uint8_t   RemainMealNum[2] ; /*ʣ���Ʒ��*/
}CustomerSel__struction ;
extern CustomerSel__struction CustomerSel;

extern uint8_t ReadSdBuff[512];
extern uint8_t SendHostBuff[128];
extern uint8_t ReadBuf[20];

extern uint8_t  F_RX1_Right;
extern uint8_t  rx1Buf[512];	  //�������ݸ����������������������ݴ������buffer

extern uint8_t signin_state;  //�������ͨ�ŵ�״̬λ
extern uint8_t sell_type[4];  //�洢�۲�ID������
extern uint8_t Menu_interface;//��ǰ��ʾ���۲ͽ���
extern uint8_t Menuset_interface; //��ǰ��ʾ����ͽ���
extern uint8_t Menu_interface;
extern uint8_t MealSet_interface;
extern uint16_t price_1st;
extern uint16_t price_2nd;
extern uint16_t price_3rd;
extern uint16_t price_4th;
extern uint8_t mealname_1st[20]; 
extern uint8_t mealname_2nd[20];
extern uint8_t mealname_3rd[20];
extern uint8_t mealname_4th[20];

uint16_t mem_copy01(uint8_t *dest, const uint8_t *source, const long s_len);
uint16_t mem_copy00(uint8_t *dest, const uint8_t *source, const long s_len);
uint8_t TakeMealsFun1(uint8_t *SendBuffer);
void memcpy_02(char *dest,char *source,char type);
uint8_t SignInFun(void);
uint32_t  MealDataCompareFun(void);/*��Ʒ���ݶԱ�*/
uint8_t SignOutFun(void);/*��ǩ*/
uint8_t StatusUploadingFun(uint16_t erro_status);//״̬����
uint8_t TemperatureUploadingFun(uint8_t Temperature_t); //�¶�����
uint8_t EchoFun(void);/*�������*/
uint8_t TakeMealsFun(uint8_t *SendBuffer,uint8_t takeout_flag);/*ȡ������*/
uint8_t MealUploadingFun(void);/*���Ͳ�Ʒ����*/
uint8_t ClearingFun(void);/*��������*/
//void  EchoFuntion(void (*fptr)(void)) ;
uint8_t 	Resend(uint8_t *p,long lenght);
void WriteToSD_data(void);
void DataUpload(uint8_t takemeal_flag);
void StateSend(void);
//void SignInFunction(void);
void GetBRWN(void);
void GetMealDetail(void);

unsigned int GetCrc16(uint8_t *bufData,unsigned int sizeData);
void HL_IntToBuffer(const uint16_t int_v, uint8_t *ret_buf);
bool StringToHexGroup1(uint8_t *OutHexBuffer, char *InStrBuffer, unsigned int strLength);

bool EchoFuntion(void (*fptr)(void));
bool SignInFunction(void);
#endif

