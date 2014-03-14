#ifndef __MSGHANDLE__
#define __MSGHANDLE__

extern uint8_t Temperature;
#define FloorMealNum  15				  /*�����ж��ٸ���*/
typedef struct MealAt
{
    
	uint8_t MealPrice;         //�ͼ۸�
	uint8_t Position[15][3];   //�ͷ��õ�λ��
	uint8_t MealCount;         //��ʣ������
	
}MealAttribute;	  ////����9����ϵ�����ԡ�

extern MealAttribute DefineMeal[9];	//����9����ϵ




typedef struct UserAction
{
  unsigned char MealID;           //�û�ѡ��Ĳ�ID
	unsigned char MealPrice;        //�û�ѡ��Ĳͼ۸�
	unsigned char MealCount;        //�û�ѡ��Ĳͷ���
	unsigned char PayShould;        //�û�Ӧ�����ܶ�
	uint16_t PayForBills;           //�û�Ͷ���ֽ����
	uint16_t PayForCards;           //�û�Ӧ��ˢ������
	uint16_t PayForCoins;           //�û�Ͷ���Ӳ����
	uint16_t PayAlready;            //�û��Ѿ������ܶ�
	unsigned char MoneyBack;        //�û�����
	uint16_t RechxargeCounter;      //��ֵ���
	uint16_t ICCardID;		          //IC����ID
	uint16_t IC_Card_Balance;       //IC�������
	unsigned char  PrintTick;       //�Ƿ��ӡСƱ��־��
	unsigned char  PayType ;        //֧����ʽ
}UserSelection;

extern UserSelection UserAct;


typedef struct FloorMeal
{
    uint8_t MealID;	 //��ǰ���Ų͵�ID
	uint8_t MealCount;	 //��ǰ���Ų͵�����
	uint8_t FCount;     //��һ�зŲ͵�����
	uint8_t SCount;	 //�ڶ��зŲ͵�����
	uint8_t TCount;	 //�����зŲ͵�����
	uint8_t FloorNum;	 //��ǰ�ǵڼ���
}FloorAttribute;

		  

typedef union _FloorMealMessage
{
  FloorAttribute      FloorMeal[FloorMealNum];
  uint8_t	          FlashBuffer[FloorMealNum*6]; /*�����Ҹı���һ��*/
}FloorMealMessage;
extern 	FloorMealMessage FloorMealMessageWriteToFlash;
 /*
 ������Ļ��ʾ�¶ȣ�ֻ�����������ʱ����ʾ�¶�
 */
typedef struct LCD
{
  unsigned char LCD_Temp;
  				
}LCDStruction ;
extern 	 LCDStruction  	 LCDstructure;

/****************/
void cancel(void) ;
uint8_t MsgHandle(uint8_t cmd);	
void StatisticsTotal(void);
extern uint8_t WaitTime ;
void MealArr(unsigned char index) ;/*ͳ�Ʋ�Ʒ������*/

void LCD_DisTemp(void);
void  LCD_En_Temp(void);



#endif
