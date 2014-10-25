#ifndef __MSGHANDLE__
#define __MSGHANDLE__
#include "stdint.h"
#include "stm32f4xx.h"
#define FloorMealNum    15				   /*�����ж��ٸ���*/
#define MealKindTotoal  17           /*�����м�����Ʒ*/
typedef struct MealAt
{
    
	uint8_t MealPrice;         //�ͼ۸�
	uint8_t Position[15][3];   //�ͷ��õ�λ��
	uint8_t MealCount;         //��ʣ������
	
}MealAttribute;	  ////���嵥����ϵ�����ԡ�

extern MealAttribute DefineMeal[MealKindTotoal];	//����MealKindTotoal����ϵ

#define UserActiontotoaDatalSize  37
/*˼·������ܶ��Ʒ��*/
typedef struct UserAction
{
	uint32_t MealType_1st;       //�û�ѡ���һ�ݲ�Ʒ��ID
	uint32_t MealType_2nd;       //�û�ѡ��ڶ��ݲ�Ʒ��ID
	uint32_t MealType_3rd;       //�û�ѡ������ݲ�Ʒ��ID
	uint32_t MealType_4th;       //�û�ѡ����ķݲ�Ʒ��ID
  uint32_t MealCnt_1st_t;      //�û�ѡ���һ�ֲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint32_t MealCnt_2nd_t;      //�û�ѡ��ڶ��ֲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint32_t MealCnt_3rd_t;      //�û�ѡ������ֲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����
	uint32_t MealCnt_4th_t;      //�û�ѡ������ֲ�Ʒ������δ���·��빺�ﳵ��ʱ����ʱ����	
  uint32_t MealCnt_1st;        //�û�ѡ���һ�ݲ�Ʒ������
	uint32_t MealCnt_2nd;        //�û�ѡ��ڶ��ݲ�Ʒ������
	uint32_t MealCnt_3rd;        //�û�ѡ������ݲ�Ʒ������
	uint32_t MealCnt_4th;        //�û�ѡ����ķݲ�Ʒ������
  uint32_t MealPrice_1st;      //�û�ѡ���һ�ݲ�Ʒ�ĵ���
	uint32_t MealPrice_2nd;      //�û�ѡ��ڶ��ݲ�Ʒ�ĵ���
	uint32_t MealPrice_3rd;      //�û�ѡ������ݲ�Ʒ�ĵ���
	uint32_t MealPrice_4th;      //�û�ѡ����ķݲ�Ʒ�ĵ���   
  uint32_t MealCost_1st;      //�û�ѡ���һ�ݲ�Ʒ���ܼ�
	uint32_t MealCost_2nd;      //�û�ѡ��ڶ��ݲ�Ʒ���ܼ�
	uint32_t MealCost_3rd;      //�û�ѡ������ݲ�Ʒ���ܼ�
	uint32_t MealCost_4th;      //�û�ѡ����ķݲ�Ʒ���ܼ�
	uint32_t PayShould;          //�û�Ӧ�����ܶ�
	uint32_t MealID;             //�û���ǰѡ��Ĳ�ƷID 
	uint32_t Meal_totoal;        //�û�ѡ�͵�����
	uint32_t Meal_takeout;       //�û���ȡ���Ĳ�Ʒ��
	uint32_t PayForCoins;        //�û�Ͷ���Ӳ����	
	uint32_t PayForBills;        //�û�Ͷ���ֽ����
	uint32_t PayForCards;        //�û�Ӧ��ˢ������  
	uint32_t PayAlready;         //�û��Ѿ������ܶ�
	uint32_t MoneyBack;          //�û�����֮��Ҫ�˵�Ǯ��Ҳ���ǻ��ж���ǮҪ�Ҹ��ͻ�
  uint32_t MoneyBackShould;    //Ӧ��Ǯ����ʾ����
  uint32_t MoneyBackAlready;   //����Ǯ����ʾ����
	uint32_t PrintTick;          //�Ƿ��ӡСƱ��־
	uint32_t PayType;            //֧����ʽ                   
	uint32_t Cancle;             //�û�ȡ������
  uint32_t MoneyPayBack_Already ;     //�����ϴ���                
  uint32_t MoneyPayBack_Already_1st;  //��Ǯ����ұ�    
  uint32_t MoneyPayBack_Already_2nd;  //ȡ�ͳ������ұ�           
}UserSelection;

typedef union _UserActMessage
{
  UserSelection   UserAct;
  uint8_t         FlashBuffer[UserActiontotoaDatalSize*4];
}UserActMessage;
extern UserActMessage UserActMessageWriteToFlash;//�û�����д��flash

typedef struct FloorMeal
{
  uint8_t MealID;	     //��ǰ���Ų͵�ID
	uint8_t MealCount;	 //��ǰ���Ų͵�����
	uint8_t FCount;      //��һ�зŲ͵�����
	uint8_t SCount;	     //�ڶ��зŲ͵�����
	uint8_t TCount;	     //�����зŲ͵�����
	uint8_t FloorNum;	   //��ǰ�ǵڼ���
}FloorAttribute;

typedef union _FloorMealMessage
{
  FloorAttribute      FloorMeal[FloorMealNum];
  uint8_t	            FlashBuffer[FloorMealNum*6]; /*�����Ҹı���һ��*/
}FloorMealMessage;
extern 	FloorMealMessage FloorMealMessageWriteToFlash;/*������д��flash�Ľṹ*/
extern  FloorAttribute FloorMeal[FloorMealNum], CurFloor;
extern uint8_t WaitTime ;
extern uint8_t WaitMealTime ;
extern uint32_t payfor_meal ;

typedef union _CoinsTotoalMessage
{
  uint16_t CoinTotoal;
	uint8_t  CoinsCnt[2];
}CoinsTotoalMessage;
extern CoinsTotoalMessage CoinsTotoalMessageWriteToFlash;/*Ӳ�����ṹ��*/ 

typedef union _MealCompareData
{
	uint32_t MealCompareTotoal;
	uint8_t  MealComparePart[4];
}MealCompareDataStruct;
extern MealCompareDataStruct MealCompareData; /*��Ʒ�Ա�������ʾ*/


void StatisticsTotal(void);
void MealArr(unsigned char index);
void InitSetting(void);
bool SearchMeal(uint32_t MealID ,uint32_t MealNum);

#endif
