#include "stm32f4xx.h"
#include "uart5.h"
#include "network_handle.h"
#include "printer.h"
#include "delay.h"
#include "rtc.h"
#include "stdio.h"
#include "string.h"
#include "MsgHandle.h"
#include "sd.h"
#include "uart6.h"
#include "serialscreen.h"
#include "szt_gpboc.h"

static long Batch = 0x00 ;//������ˮ��
//__attribute__ ((aligned (4)))
uint8_t  TID[7] = {0xa2,0x00,0x04,0x10,0x00,0x00,0x66}; /*�ն�TID�룺���޸�*/
uint8_t  BRWN[7+3] = {0xa6,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  /*������ˮ��*/
uint8_t  BNO[6] = {0xa7,0x00,0x03,0x00,0x00,0x00};               /*���κţ����޸�*/
uint8_t  DeviceArea[3+3]={0xac,0x00,0x03,0x17,0x03,0x02};         /*�ն����������ţ����޸�*/
uint8_t  DeviceAreaNO[4+3]={0xad,0x00,0x04,0x17,0x03,0x02,0x07};   /*�ն����ڵ����ţ����޸�*/
uint8_t  DeviceStatus[2+3]={0xae,0x00,0x02,0xE0,0x10};     /*�ն�״̬*/
uint8_t  ErroDelect[2+3]={0xae,0x00,0x02,0xE0,0x10};     /*�������飬��ֹ����ָ�볬�����޴�����*/
uint8_t  DealData[7]={0xa9,0x00,0x04,0x00,0x00,0x00,0x00};     /*��������*/
uint8_t  DealTime[6]={0xaa,0x00,0x03,0x00,0x00,0x00};       /*����ʱ��*/
uint8_t  MAC[8+3]={0xc9,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   /*MAC*/
uint8_t  DealBalance[6+3]={0xb0,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};    /*���׽��*/
uint8_t  MealID[4+3] = {0xb1,0x00,0x04,0x10,0x00,0x00,0x20} ;                /*��ƷID  10000020*/
uint8_t  MealNO[1+3] = {0xb2,0x00,0x01,0x00};      /*��Ʒ��������*/
uint8_t  MealName[20+3] ={0xb3,0x00,0x14,'a','b','c','d','e','a','b','c','d','e','a','b','c','d','e','a','b','c','d','a'};  /*��Ʒ����*/
uint8_t  MealPrice[6+3]={0xbd,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};   /*��Ʒ�۸�*/
uint8_t  PayType[1+3]={0xbf,0x00,0x01,0x00};        /*֧����ʽ*/
uint8_t  Change[6+3]={0xd7,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00} ;        /*������*/
uint8_t  RemainMealNum[2+3]={0xd8,0x00,0x02,0x00,0x00};  /*ʣ���Ʒ����*/
uint8_t  MName[20+3]={0xa4,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};             /*�̻���MID*/
uint8_t  APPVersion[8+3]={ 0xc2,0x00,0x08,'0','4','0','0','0','4',0x0D,0x0A};       /*Ӧ�ó���汾  V4.0.3  */ 
uint8_t  ParaFileVersion[8+3]={0xc3,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};   /*�����ļ��汾*/
uint8_t  BDataFileVersion[8+3]={0xc4,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};  /*ҵ�������ļ��汾*/
uint8_t  ChechStatus[2+3]={0xab,0x00,0x02,0x00,0x00};                                     /*�Լ�״̬*/
uint8_t  MID[3+3]={ 0xa3,0x00,0x03,0x10,0x00,0x01};                                       /*�̻���MID*/
uint8_t  TTCount[3+2]     ={0xb7,0x00,0x02,0x00,0x00};                                    /*�����ܱ��� */
uint8_t  TNCT[6+3]        ={0xb8,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};                /*�����ܽ��*/
uint8_t  TNtype[3+2]      ={0xd9,0x00,0x02,0x00,0x00};                              /*�����ܲ�Ʒ�� d9*/
uint8_t  TotalChange[3+6] ={0xd7,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};          /*�������� d7*/
uint8_t  TakeMealFlag[3+2]={0xdb,0x00,0x02,0x00,0x00};                              /*ȡ�ͱ�־ 0x01:�ɹ� 0x02:ʧ�� */
uint8_t  PosDevNum[10+3]=   {0xe0,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};                  /*ˢ�����ն˺�*/
uint8_t  PosTenantNum[15+3]={0xe1,0x00,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; /*ˢ�����̻���*/
uint8_t  PosBatchNum[10+3]= {0xe2,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};      /*������ˮ��*/
uint8_t  PosUserNum[21+3]=  {0xe3,0x00,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  /*�û����п���*/
uint8_t  UpdataFlag[3+1]=  {0xc1,0x00,0x01,0x00};                                     //���±�ʶ
uint8_t  MenuNo[3+1]= {0xdd,0x00,0x01,0x01};                                        /*�˵����,0x01,0x02,0x03,0x04,0x05��������*/
uint8_t  MenuDetail[3+MealKindTotoal*47]={0xdf,0x00,0x00};                          /*�˵�����ϸ��Ϣ*/ 
uint8_t  ACK[3+1]={0xc0,0x00,0x02,0x00};                                            //Ӧ����
uint8_t  WordKeyCipher[11]={0xc7,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//������Կ
//__attribute__ ((aligned (1)))
union _Meal_Union Meal_Union;

uint8_t Record_buffer[254] = {0} ;

const uint8_t  DeviceTemperature[2+3]={0xdc,0x00,0x02,0x00,0x00};    /*�豸�¶ȣ�����һ���ֽ�Ϊ�¶�*/
const uint8_t  DeviceCoinsTotal[2+3] ={0xde,0x00,0x02,0x00,0x00};    /*����Ӳ�������������ֽڴ洢���ǻ���*/
const uint8_t  Discount =10;  //�����ۿ�10��(������),5��(���),0��(��ҪǮ)
/*��Ʒ��ID*/
uint16_t MealID_1st[4]={0};
uint16_t MealID_2nd[4]={0};
uint16_t MealID_3rd[4]={0};
uint16_t MealID_4th[4]={0};

/*��Ʒ������*/
uint16_t MealNo_1st=0;
uint16_t MealNo_2nd=0;
uint16_t MealNo_3rd=0;
uint16_t MealNo_4th=0;

/*��Ʒ�ļ۸���Ϣ*/
uint16_t price_1st= 0; 
uint16_t price_2nd= 0;
uint16_t price_3rd= 0;
uint16_t price_4th= 0;

/*��Ʒ���ֽ��ۿ���Ϣ*/
uint8_t cashcut_1st=0;
uint8_t cashcut_2nd=0;
uint8_t cashcut_3rd=0;
uint8_t cashcut_4th=0;

/*��Ʒ������ͨ�ۿ���Ϣ*/
uint8_t sztcut_1st=0;
uint8_t sztcut_2nd=0;
uint8_t sztcut_3rd=0;
uint8_t sztcut_4th=0;

/*��Ʒ�������ۿ���Ϣ*/
uint8_t gboccut_1st=0;
uint8_t gboccut_2nd=0;
uint8_t gboccut_3rd=0;
uint8_t gboccut_4th=0;

/*��Ʒ�Ļ�Ա���ۿ���Ϣ*/
uint8_t vipcut_1st=0;
uint8_t vipcut_2nd=0;
uint8_t vipcut_3rd=0;
uint8_t vipcut_4th=0;

/*΢���ۿ�*/
uint8_t vchatcut_1st=0;
uint8_t vchatcut_2nd=0;
uint8_t vchatcut_3rd=0;
uint8_t vchatcut_4th=0;

/*��Ʒ��λ����Ϣ*/
uint8_t Order_1st=0;
uint8_t Order_2nd=0;
uint8_t Order_3rd=0;
uint8_t Order_4th=0;

/*��Ʒ����*/
uint8_t MealType_1st[4]={0};
uint8_t MealType_2nd[4]={0};
uint8_t MealType_3rd[4]={0};
uint8_t MealType_4th[4]={0};

/*��Ļ��ʾ�Ĳ�Ʒ����*/
uint8_t  mealname_1st[20]= {'\0'}; 
uint8_t  mealname_2nd[20]= {'\0'};
uint8_t  mealname_3rd[20]= {'\0'};
uint8_t  mealname_4th[20]= {'\0'};

uint8_t Menu_interface= Menu1st_interface;
uint8_t MealSet_interface= MealSet1st_interface;
uint8_t sell_type[4]={0x01,0x02,0x03,0x04};

 /*******************************************************************************
 * ��������:GetMealDetail                                                                   
 * ��    ��:ͨ���ṹ���ȡ�ṹ���еı���                                                                                                                         
 * ��    ��:��                                                                
 * ��    ��:��                                                                    
 * ��    ��:void                                                               
 * �޸�����:2014��11��6��                                                                   
 *******************************************************************************/ 
void GetMealDetail(void)
{
	/*��ȡ��Ʒ����*/
	MealNo_1st= Meal_Union.Meal[0].MealCnt[0]*16 + Meal_Union.Meal[0].MealCnt[1];
	MealNo_2nd= Meal_Union.Meal[1].MealCnt[0]*16 + Meal_Union.Meal[1].MealCnt[1];
	MealNo_3rd= Meal_Union.Meal[2].MealCnt[0]*16 + Meal_Union.Meal[2].MealCnt[1];
	MealNo_4th= Meal_Union.Meal[3].MealCnt[0]*16 + Meal_Union.Meal[3].MealCnt[1];

	/*��ȡ�۸���Ϣ*/
	price_1st=((Meal_Union.Meal[0].MealPrice[4]&0xf0)>>4)*10;
	price_1st+=Meal_Union.Meal[0].MealPrice[4]&0x0f;
	price_2nd= ((Meal_Union.Meal[1].MealPrice[4]&0xf0)>>4)*10;
	price_2nd+=Meal_Union.Meal[1].MealPrice[4]&0x0f;
	price_3rd= ((Meal_Union.Meal[2].MealPrice[4]&0xf0)>>4)*10;
	price_3rd+=Meal_Union.Meal[2].MealPrice[4]&0x0f;
	price_4th= ((Meal_Union.Meal[3].MealPrice[4]&0xf0)>>4)*10;
	price_4th+=Meal_Union.Meal[3].MealPrice[4]&0x0f;
	
	/*��ȡ��ͨ�ۿ���Ϣ*/
	cashcut_1st= ((Meal_Union.Meal[0].MealCut[0]-'0')*10+Meal_Union.Meal[0].MealCut[1]-'0');
	cashcut_2nd= ((Meal_Union.Meal[1].MealCut[0]-'0')*10+Meal_Union.Meal[1].MealCut[1]-'0');
	cashcut_3rd= ((Meal_Union.Meal[2].MealCut[0]-'0')*10+Meal_Union.Meal[2].MealCut[1]-'0');
	cashcut_4th= ((Meal_Union.Meal[3].MealCut[0]-'0')*10+Meal_Union.Meal[3].MealCut[1]-'0');
	
	if(cashcut_1st==0x00) cashcut_1st=100;
	if(cashcut_2nd==0x00) cashcut_2nd=100;
	if(cashcut_3rd==0x00) cashcut_3rd=100;
	if(cashcut_4th==0x00) cashcut_4th=100;
	
	/*��ȡ����ͨ�ۿ���Ϣ*/
	sztcut_1st= ((Meal_Union.Meal[0].SztCut[0]-'0')*10+Meal_Union.Meal[0].SztCut[1]-'0');
	sztcut_2nd= ((Meal_Union.Meal[1].SztCut[0]-'0')*10+Meal_Union.Meal[1].SztCut[1]-'0');
	sztcut_3rd= ((Meal_Union.Meal[2].SztCut[0]-'0')*10+Meal_Union.Meal[2].SztCut[1]-'0');
	sztcut_4th= ((Meal_Union.Meal[3].SztCut[0]-'0')*10+Meal_Union.Meal[3].SztCut[1]-'0');
	if(sztcut_1st==0x00) sztcut_1st=100;
	if(sztcut_2nd==0x00) sztcut_2nd=100;
	if(sztcut_3rd==0x00) sztcut_3rd=100;
	if(sztcut_4th==0x00) sztcut_4th=100;
	
	/*��ȡ�����ۿ���Ϣ*/
	gboccut_1st= ((Meal_Union.Meal[0].GbocCut[0]-'0')*10+Meal_Union.Meal[0].GbocCut[1]-'0');
	gboccut_2nd= ((Meal_Union.Meal[1].GbocCut[0]-'0')*10+Meal_Union.Meal[1].GbocCut[1]-'0');
	gboccut_3rd= ((Meal_Union.Meal[2].GbocCut[0]-'0')*10+Meal_Union.Meal[2].GbocCut[1]-'0');
	gboccut_4th= ((Meal_Union.Meal[3].GbocCut[0]-'0')*10+Meal_Union.Meal[3].GbocCut[1]-'0');  
	if(gboccut_1st==0x00) gboccut_1st=100;
	if(gboccut_2nd==0x00) gboccut_2nd=100;
	if(gboccut_3rd==0x00) gboccut_3rd=100;
	if(gboccut_4th==0x00) gboccut_4th=100; 
	
	/*��ȡ��Ա���ۿ���Ϣ*/
	vipcut_1st= ((Meal_Union.Meal[0].VipCut[0]-'0')*10+Meal_Union.Meal[0].VipCut[1]-'0');
	vipcut_2nd= ((Meal_Union.Meal[1].VipCut[0]-'0')*10+Meal_Union.Meal[1].VipCut[1]-'0');
	vipcut_3rd= ((Meal_Union.Meal[2].VipCut[0]-'0')*10+Meal_Union.Meal[2].VipCut[1]-'0');
	vipcut_4th= ((Meal_Union.Meal[3].VipCut[0]-'0')*10+Meal_Union.Meal[3].VipCut[1]-'0');
	if(vipcut_1st==0x00) vipcut_1st=100;
	if(vipcut_2nd==0x00) vipcut_2nd=100;
	if(vipcut_3rd==0x00) vipcut_3rd=100;
	if(vipcut_4th==0x00) vipcut_4th=100; 
	
	/*��ȡ��Ʒ��λ����Ϣ*/
	Order_1st= (Meal_Union.Meal[0].MealOrder[0]);
	Order_2nd= (Meal_Union.Meal[1].MealOrder[0]);
	Order_3rd= (Meal_Union.Meal[2].MealOrder[0]);
	Order_4th= (Meal_Union.Meal[3].MealOrder[0]);
	
	/*��ȡ��ƷID��Ϣ*/
	memcpy(MealID_1st,Meal_Union.Meal[0].MealID,sizeof(Meal_Union.Meal[0].MealID));
	memcpy(MealID_2nd,Meal_Union.Meal[1].MealID,sizeof(Meal_Union.Meal[1].MealID));
	memcpy(MealID_3rd,Meal_Union.Meal[2].MealID,sizeof(Meal_Union.Meal[2].MealID));
	memcpy(MealID_4th,Meal_Union.Meal[3].MealID,sizeof(Meal_Union.Meal[3].MealID)); 
	
	/*��ȡ��Ʒ��*/
	memcpy(mealname_1st,Meal_Union.Meal[0].MealName,sizeof(Meal_Union.Meal[0].MealName));
	memcpy(mealname_2nd,Meal_Union.Meal[1].MealName,sizeof(Meal_Union.Meal[1].MealName));
	memcpy(mealname_3rd,Meal_Union.Meal[2].MealName,sizeof(Meal_Union.Meal[2].MealName));
	memcpy(mealname_4th,Meal_Union.Meal[3].MealName,sizeof(Meal_Union.Meal[3].MealName));
	
	/*��Ʒ������*/
	memcpy(MealType_1st,Meal_Union.Meal[0].MealType,sizeof(Meal_Union.Meal[0].MealType));
	memcpy(MealType_2nd,Meal_Union.Meal[1].MealType,sizeof(Meal_Union.Meal[1].MealType));
	memcpy(MealType_3rd,Meal_Union.Meal[2].MealType,sizeof(Meal_Union.Meal[2].MealType));
	memcpy(MealType_4th,Meal_Union.Meal[3].MealType,sizeof(Meal_Union.Meal[3].MealType));  
	
}
 /*******************************************************************************
* Function Name  : GetBRWN
* Description    : ����: �õ���ˮ��
* Input          : CRC
* Output         : �����Լ���С
* Return         : CRC
*******************************************************************************/
void GetBRWN(void)
{
	 /*ʱ����Ϊ��ˮ��*/
	RTC_TimeShow();
	BRWN[3] =    20 /10 *16 + 20%10 ;         /*��*/
	BRWN[4] =    TimeDate.Year /10 *16 +  TimeDate.Year%10 ;
	BRWN[5] =    TimeDate.Month /10 *16 + TimeDate.Month%10 ;
	BRWN[6] =    TimeDate.Date /10 *16 +  TimeDate.Date%10 ;
	BRWN[7] =    TimeDate.Hours /10 *16 + TimeDate.Hours%10 ;
	BRWN[8] =    TimeDate.Minutes /10 *16 + TimeDate.Minutes%10 ;
	BRWN[9] =    TimeDate.Senconds /10 *16 + TimeDate.Senconds%10 ;
}

/*******************************************************************************
* Function Name  : GetCrc16
* Description    : ����CRCУ��
* Input          : CRC
* Output         : �����Լ���С
* Return         : CRC
*******************************************************************************/
uint32_t GetCrc16(uint8_t *bufData,uint32_t sizeData)
{
	uint32_t Crc ,i = 0;
	uint8_t j = 0;
	if(sizeData == 0)
		return 1 ;
	Crc = 0 ;
	for(i=0;i<sizeData;i++)
	{
		Crc ^= bufData[i];
		for(j=0;j<8;j++)
		{
			if(Crc&0x0001)
			{
				Crc >>=1 ;
				Crc ^=0x08408 ;
			}
			else
				Crc >>=1 ;
		}
	}
	return Crc ;
}

/*---------------------------------------------------------------------------
void HL_IntToBuffer(const uint16_t int_v, uint8_t *ret_buf);
����: ��һ�� 16bit ������ת���� Buffer ����, ��λ��ǰ
��λ��ǰ
*/
void HL_IntToBuffer(const uint16_t int_v, uint8_t *ret_buf)
{
	ret_buf[0] = (uint8_t)(int_v >> 8);  // [0] ��Ӧ --> ��λ
	ret_buf[1] = (uint8_t)(int_v & 0xff);

}
/*---------------------------------------------------------------------------
 uint16_t HL_BufferToInt(const uint8_t *in_buf);
 ����: ������������ת��������, ��λ��ǰ
 ��λ��ǰ
*/
uint16_t HL_BufferToInit(const uint8_t *in_buf)
{
	uint16_t ret_n;
	ret_n = in_buf[0];           // ȡ��λ
	ret_n = (ret_n << 8);
	ret_n += in_buf[1];          // ȡ��λ
	return ret_n;
}

/*---------------------------------------------------------------------------
uint32_t HL_BufferToLong(const uint8_t *in_buf);
����: �� (uint8_t *) ����ת���� Long ��, ��λ��ǰ
��λ��ǰ
*/
uint32_t HL_BufferToLong00(const uint8_t *in_buf)
{
	uint32_t ret_n;

	ret_n = in_buf[0];     // ���λ
	ret_n = (ret_n << 8);

	ret_n += in_buf[1];    // �θ�λ
	ret_n = (ret_n << 8);

	ret_n += in_buf[2];    // ��λ
	ret_n = (ret_n << 8);

	ret_n += in_buf[3];    // ��λ

	return ret_n;
}

/*---------------------------------------------------------------------------
void LH_LongToBuffer(uint32_t in_n, uint8_t *ret_buf);
����: ��������ת���� uint8_t ������, ��λ��ǰ
*/
void HL_LongToBuffer00(const uint32_t in_n, uint8_t *ret_buf)
{
	ret_buf[0] = (uint8_t)((in_n >> 24) & 0xff);   // [0] ��Ӧ --> ��λ
	ret_buf[1] = (uint8_t)((in_n >> 16) & 0xff);
	ret_buf[2] = (uint8_t)((in_n >> 8) & 0xff);
	ret_buf[3] = (uint8_t)(in_n & 0xff);
}
 //---------------------------------------------------------------------------
// ����: ������������
/*******************************************************************************
* Function Name  : mem_set_00
* Description    : ����: ������������
* Input          : CRC
* Output         : �����Լ���С
* Return         : CRC
*******************************************************************************/
static void mem_set_00(uint8_t *dest, const long s_len)
{
	long j0;

	for(j0 = 0; j0 < s_len; j0++)  dest[j0] = 0;
}


//---------------------------------------------------------------------------
// ����: �ַ�������
/*******************************************************************************
* Function Name  : mem_set_00
* Description    : ����: ������������
* Input          : CRC
* Output         : �����Լ���С
* Return         : CRC
*******************************************************************************/
uint16_t mem_copy01(uint8_t *dest, const uint8_t *source, const long s_len)
{
	long j0;
	for(j0 = 0; j0 < s_len; j0++)  dest[j0] = source[j0];
	dest[j0++]=',';
	return (s_len+1);
}
uint16_t mem_copy00(uint8_t *dest, const uint8_t *source, const long s_len)
{
	long j0;
	for(j0 = 0; j0 < s_len; j0++)  dest[j0] = source[j0];
	return (s_len);
}

/*********************/
typedef struct
{
	uint8_t  Lenght[2] ;
	uint8_t  MealNO[35*4];    /*��Ʒ������Ŀ*/

}ReturnData_Struction;
ReturnData_Struction  ReturnData;

/*******************************************************************************
* Function Name  : CheckResponseCode
* Description    : ��鷵���뿴ʲô����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
uint8_t CheckResponseCode(uint8_t Cmd)
{
// uint8_t  i = 0 ;
	switch(Cmd)
	{
		case 0x00 : /*���׳ɹ�*/
			return 0 ;
		case 0x01 : /*��CRC����*/
			return 1 ;
		case 0x02 : /*��CRC����*/
				break;
		case 0x03 : /*��Ч���ն�����*/
				break  ;
		case 0x04 : /*Э��汾����*/
				break ;
		case 0x05 : /*��Ч���ն�SN*/
				break;
		case 0x06 : /*��Ч���̻���(MID)*/
				break;
		case 0x07 : /*��Ч�ļ�ƫ�Ƶ�ַ*/
				break ;
		case 0x08 : /*��Ч�ļ�ƫ�Ƶ�ַ*/
				break ;
		case 0x09 : /*���������ݴ���*/
				break;
		case 10 : /*�̻������豸�Ŷ�Ӧ��ϵ����*/
				break ;
		case 11 : /*��Ʒ���ݶԱȲ�һ��*/
					 {
					 }
				break ;
		case 12 : /*��̨�������豸����ID�����ڵ�ID��Ϣ*/
				break;
		case 13 : /*�豸���͵�����ID�����ڵ�ID��Ϣ���̨��ƥ��*/
				break ;
		case 14 : /*�豸���Ͳ�Ʒ��Ϣ��ʽ����ȷ*/
				break ;
		case 15 :  /*ȡ�ͽ��ף����κš���ˮ�ţ��ظ�*/
				break;
		case 16 : /*�ն����͵Ĳ�Ʒ������ĳ��Ʒ��Ϣ�������ں�̨���ݿ�*/
				break;
		case 17 : /*��������־��ʽ����ȷ*/
				break;
		case 20 :  /*��Ч���*/
				break;
		case 21 : /*ԭʼ������*/
				break;
		case 22 : /*��Ч��ƷID��*/
				break;
		case 23 : /*����������Ч*/
				break ;
		case 0x24 : Batch ++ ;
				break;
		case 30: /*�����*/
				 break ;
		case 31 : /*�˻�������*/
				 break;
		case 40 : /*�����ն�*/
				 break;
		case 41 : /*�����ն�*/
				 break ;
		case 50 : /*��̨�ļ��汾������*/
				 break;
		case 51 :/*�����ļ��汾���̨�������ļ��汾��һ��*/
					break ;
		case 96 : /*��̨ϵͳ����*/
					break ;
		default :break;
	}
	return 3; /*�����ӵķ�ֹ��waring*/
}
 /*******************************************************************************
* Function Name  : GetData
* Description    : �õ���Ӧ���������
* Input          : uint8_t *dest,uint8_t *souce,const long s_len,uint8_t Cmd
* Output         : void
* Return         : void
*******************************************************************************/
long  GetData(uint8_t *dest,uint8_t *souce, long s_len,uint8_t Cmd)
{
	long i = 0 ,j=0,Cmdlenght=0;
	for(i=0;i<s_len;i++)
	{
		if(souce[i] == Cmd)  /*�õ�����*/
		{
			Cmdlenght= HL_BufferToInit(&souce[i+1]);    /*�õ����ݳ���*/
//    printf("Cmdlenght=%d\r\n",Cmdlenght);
			for(j=0;j<Cmdlenght;j++)              /*�õ�����  */
			{
				dest[j] = souce[i+j+3] ;
//      printf("dest[%d]=%x\r\n",j,dest[j]);
			}
			return Cmdlenght  ;
		}
	}
	return 0 ;
}
 /*******************************************************************************
* Function Name  : CmdDataSend
* Description    : �������ݺ���
* Input          : uint8_t *p  long Lenght
* Output         : void
* Return         : void       ok
*******************************************************************************/
static void CmdDataSend(uint8_t *p,long Lenght)
{
	uint16_t i=0;
	 /*��ʼ��*/
	F_RX1_SYNC = 0;
	rx1BufIndex = 0;
	F_RX1_VAILD = 1;
	CrcValue = 0;
	F_RX1_Right = 0 ;
	for(i=0;i<Lenght;i++)
	{
		UART5->DR = (u8) p[i];
		while((UART5->SR&0X40)==0);//ѭ������,ֱ���������
		//delay_ms(5);
		//printf(" p[%d]=%x\r\n", i,p[i]);
	}
}

uint8_t  MealComparefunDemo(long Cmd ,uint8_t *p,unsigned long lenght)
{
	uint16_t  CRCValue= 0 ;
// static  int Bno = 0 ;
	uint8_t Waittimeout = 250 ;
	/*�����鸳ֵ����*/
	HL_IntToBuffer(Cmd,&p[1]);
	/*���鳤��*/
	HL_IntToBuffer(lenght-8,&p[3]);
	/*�õ����ݵ�CRC*/
	CRCValue=GetCrc16(&p[1],lenght-4);
	/*��CRC��ֵ��������*/
	HL_IntToBuffer(CRCValue,&p[lenght-2]);
	/*��������*/
	CmdDataSend(p,lenght);  //��������
	/*�ȴ����ݷ���*/
	while(--Waittimeout)
	{
		delay_ms(10);
		if(F_RX1_Right) //���յ���������CRC��ȷ
		{
			F_RX1_Right = 0 ;
			break;
		}
	}
	if(Waittimeout == 0 )
		return 1 ;//��ʱ����1
	return 0 ;
}


 /*******************************************************************************
 * ��������:SignInFun
 * ��    ��:�ն�ǩ��                                                      
 *                                                                               
 * ��    ��:��                                                                   
 * ��    ��:��                                                                     
 * ��    ��:   0 --- ǩ��ʧ��
 *             1 --- �޲˵�
							 2 --- �޲�Ʒ����
							 3 --- �ɹ�
 * �޸�����:2014��11��16��                                                                    
 *******************************************************************************/ 
uint8_t SignInFun(void)
{
	long i = 0 ;
	long  Command = 0x0100 ;
	long  Lenght = 0,j = 0  ;
	long  CmdLenght = 0 ;  //�����
	long  data_detail_length =0; //��Ʒ���ݵĳ���
	uint8_t  Send_Buf[400]={0};
	memset(rx1Buf,0,sizeof(rx1Buf));  //��������

	/* ����� ++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));      /*�ն˵�TID*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MID,sizeof(MID));  /*�̻���TID ����(1)+���ݳ���(2)+����     100001*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MName,sizeof(MName));  /*�̻�����*/
	GetBRWN();                             /*�õ���ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));   /*������ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));     /*���κ�*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));  /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO)); /*�ն����ڵ�����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],APPVersion,sizeof(APPVersion)); /*Ӧ�ó���汾��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],ParaFileVersion,sizeof(ParaFileVersion)); /*�����ļ��汾��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BDataFileVersion,sizeof(BDataFileVersion)); /*ҵ�������ļ��汾��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],ChechStatus,sizeof(ChechStatus)); /*�Լ�״̬*/
	Send_Buf[CmdLenght] = 0x03  ;
	CmdLenght+=0x03;
	 /*��������*/
		 i = MealComparefunDemo(Command,Send_Buf,CmdLenght);
		/***************************************************************************/
	if(i == 0x01)  //
		return  0;
	Lenght = HL_BufferToInit(&rx1Buf[2]);  //�ж����ݳ����Ƿ�
	if(Lenght == 0x00)
		return  0 ;
	 for(i=0;i<23;i++)
	 {
		 MName[i] = rx1Buf[i+4];//�õ��̻���
	 }
	 for(i=0;i<3;i++)
	 {
		 BNO[i+3] = rx1Buf[i+27+3];//�õ����κ�
	 }
	 for(i=0;i<11;i++)
	 {
		 APPVersion[i] = rx1Buf[i+0x21];//�õ�����汾��
	 }
	 for(i=0;i<11;i++)
	 {
		 ParaFileVersion[i] = rx1Buf[i+0x2c];//�õ������ļ��汾
	 }
	 for(i=0;i<11;i++)
	 {
		 BDataFileVersion[i] = rx1Buf[i+0x37];//ҵ�����ݰ汾�ļ�
	 }
	 for(i=0;i<4;i++)
	 {
		 UpdataFlag[i] = rx1Buf[i+0x42];//�õ����κ�
	 }
	 for(i=0;i<4;i++)
	 {
			MenuNo[i]= rx1Buf[i+0x46];
			//printf("%X ",MenuNo[i]);
	 }
	 if(MenuNo[0]!=0xdd)  return 1; 
	 if(MenuNo[2]==0x00)  return 2; 
	 for(i=0;i<3;i++)
	 {
			MenuDetail[i]= rx1Buf[i+0x4A];
	 }    
	 if(MenuDetail[2]!=0)
	 {     
		 memcpy(Meal_Union.MealBuf,&rx1Buf[77],MealKindTotoal*47);
	 }
	 else
	 {
		 return 0;
	 }
	 for(i=0;i<4;i++)
	 {
		 ACK[i] = rx1Buf[i+265];//�õ�
	 }
	 for(i=0;i<11;i++)
	 {
		 WordKeyCipher[i] = rx1Buf[i+269];//�õ���������
	 }
	 for(i=0;i<11;i++)
	 {
		 MAC[i] = rx1Buf[i+280];
	 }
	 memset(rx1Buf,0,sizeof(rx1Buf));
	 if(ACK[3]==0x24)
	 {
		 return 3 ;
	 }
	 return 3 ;
}

void StructCopyToBuffer(uint8_t *dest)
{
	long j0=0,i=0,k=0;
//�ϴ�4�ݲ�Ʒ������ 4*(4+20+2+4+4)
	for(j0 = 0; j0 < 4; j0++)
	{
		for(i=0;i<4;i++)
		dest[k++]=Meal_Union.Meal[j0].MealID[i];
		
		for(i=0;i<20;i++)
		dest[k++]=Meal_Union.Meal[j0].MealName[i];

		dest[k++]= 0; //��һ����Ϊ0
		dest[k++]= DefineMeal[j0].MealCount; //�ڶ�����Ϊ��Ʒ������

		for(i=0;i<6;i++) 
		dest[k++]=Meal_Union.Meal[j0].MealPrice[i];

		for(i=0;i<4;i++)
		dest[k++]=Meal_Union.Meal[j0].MealType[i];
	}
}

/*******************************************************************************
* Function Name  : ��Ʒ���ݶԱ�    0X0200
* Description    :��Ʒ���ݶԱ�
* Input          : void
* Output         : void
* Return         : char
* Time          :2014-4-7  MrLao   ���������TVL��ʽ
*******************************************************************************/
uint8_t k = 0 ;
MealCompareDataStruct MealCompareData;
uint32_t  MealDataCompareFun(void)
{
	
	uint8_t i= 0 ;
	long  Lenght = 0 ,j;
//  uint8_t MealID = 0 ;
	long  CmdLenght = 0 ;
	uint8_t status = 0 ;
	uint8_t Send_Buf[300]={0};
	uint8_t TempBuffer[37*4]={0};
	uint8_t Buffer[36*4]={0};
	mem_set_00(rx1Buf,sizeof(rx1Buf));

	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	GetBRWN(); /*�õ�ˮ����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));                   /*������ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));                     /*���κ�*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));       /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO));   /*�ն����ڵ�����*/
	Send_Buf[CmdLenght] =  0xbc ;  //T  ��Ʒ���ݶԱȣ� ���������TVL��ʽ
	Send_Buf[CmdLenght+1] =  0x00 ;  //L ���ݳ���
	Send_Buf[CmdLenght+2] =  36*4 ;   //V ��������  
	CmdLenght += 3;
	StructCopyToBuffer(Buffer);
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Buffer,sizeof(Buffer));

	Send_Buf[CmdLenght] = 0x03  ;
	CmdLenght+=0x03;

	/*��������*/
	i = MealComparefunDemo(0x0200,Send_Buf,CmdLenght);

/***************************************************************************/
	MealCompareData.MealCompareTotoal= 0; 
		/*��Ƭ�Ƿ�ɹ�*/
	if(i==0x01) //��ʱ
	{   
		return MealCompareData.MealCompareTotoal; 
	}
	
	Lenght = HL_BufferToInit(&rx1Buf[2]);   //�õ����ݳ���
	GetData(&ReturnData.Lenght[0],rx1Buf,Lenght,0xc0);    /*����״̬*/
	
	//printf("Status=%x\r\n",ReturnData.Lenght[0]);

	if(ReturnData.Lenght[0] == 0x00 )
	{
		MealCompareData.MealCompareTotoal= 0xFFFFFFFF;  
			return MealCompareData.MealCompareTotoal;  /*������ȷ*/
	} 
	if(ReturnData.Lenght[0] == 0x24 )
	{
		MealCompareData.MealCompareTotoal= 0xFFFFFFFF;  
			return MealCompareData.MealCompareTotoal;  /*������ȷ*/
	}
	CmdLenght = GetData(TempBuffer,rx1Buf,Lenght,0xBC);/*��Ʒ�Ա�*/
	//printf("StatusCmdLenght=%x\r\n",CmdLenght);
	if(CmdLenght>37)
	{
		//status  = CmdLenght / 37  ;
		for(i=0;i<4;i++)
		{
			if(rx1Buf[47+i*37]==0x04)   //��Ʒ�Աȱ�־
			{
				MealCompareData.MealComparePart[i]=0xFF;  //144-36/3  108/3=36
			}         
			else
			{
				MealCompareData.MealComparePart[i]=rx1Buf[36+i*37];
			}       
		}
		return 1 ;  //���ضԱ���ȷ
	}
	else
	{
		MealCompareData.MealCompareTotoal=0;
		return 0;    //���ضԱȴ���
	}
}


/*******************************************************************************
* Function Name  : 2.3.3 ǩ��    0X0300
* Description    :��ǩ����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
uint8_t SignOutFun()
{

	uint8_t i = 0 ;
	long  Lenght = 0 ,j;
	long  CmdLenght = 0 ;
	uint8_t     Send_Buf[400];

	mem_set_00(rx1Buf,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	GetBRWN(); /*�õ�ˮ����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
	Send_Buf[CmdLenght] = 0x03  ;
	CmdLenght+=0x03;
	i = MealComparefunDemo(0x0300,Send_Buf,CmdLenght);

	if(i == 0x01)
	return 1 ;
	 Lenght = HL_BufferToInit(&rx1Buf[2]);
	 for(j=0;j<Lenght+7;j++)
	 {
//   printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	 }
	 return 0 ;
}

 /*******************************************************************************
* Function Name  : ״̬�ϴ�����    0X0400
* Description    :״̬����
* Input          : void
* Output         : void
* Return         : void         ok
*******************************************************************************/
uint8_t StatusUploadingFun(uint16_t erro_status)
{

	uint8_t i = 0 ;
	long  Lenght = 0 ,j;
	long  CmdLenght = 0 ;
	uint8_t Temperature_t=0;
	uint16_t CoinsTotoal_t=0;
	uint8_t  Temperature_tt[5]={0};
	uint8_t  CoinsTotoal_tt[5]={0};
	uint8_t  Send_Buf[100]={0};
	uint8_t  state_temp[2]={0};  
	//sprintf(state_temp,"%x",erro_status); 
	mem_set_00(rx1Buf,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	GetBRWN(); /*�õ�ˮ����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/

	state_temp[1]=(erro_status&0x00ff);
	state_temp[0]=(erro_status>>8)&0x00ff;
	for(i=0;i<2;i++) 
	{
		DeviceStatus[3+i]= state_temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceStatus,sizeof(DeviceStatus));  /*�ն˵�״̬*/
	Temperature_t= Temperature;
	state_temp[1]=(Temperature_t&0x00ff);
	state_temp[0]=(Temperature_t>>8)&0x00ff;
	memcpy(Temperature_tt,DeviceTemperature,sizeof(DeviceTemperature));
	for(i=0;i<2;i++) 
	{
		Temperature_tt[3+i]= state_temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Temperature_tt,sizeof(Temperature_tt));  /*�ն˵��¶�*/
	CoinsTotoal_t =CoinsTotoalMessageWriteToFlash.CoinTotoal;
	state_temp[1]=(CoinsTotoal_t&0x00ff);
	state_temp[0]=(CoinsTotoal_t>>8)&0x00ff;   
	memcpy(CoinsTotoal_tt,DeviceCoinsTotal,sizeof(DeviceCoinsTotal));  
	for(i=0;i<2;i++) 
	{
		CoinsTotoal_tt[3+i]= state_temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],CoinsTotoal_tt,sizeof(CoinsTotoal_tt));  /*�ն˵�Ӳ����*/   
	Send_Buf[CmdLenght] = 0x03  ;
	CmdLenght+=0x03;
	i = MealComparefunDemo(0x0400,Send_Buf,CmdLenght);//0x0400 ״̬����
	if(i == 0x01)
		return 1 ;
	Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	for(j=0;j<Lenght+7;j++)
	{
	//   printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	}
	return 0 ;
}

 /*******************************************************************************
* Function Name  : �������  0X0700
* Description    :�������
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
uint8_t EchoFun(void)
{

	uint8_t i = 0 ;
	long  Lenght = 0 ,j;
	long  CmdLenght = 0 ;
	uint8_t     Send_Buf[100];
	mem_set_00(rx1Buf,sizeof(rx1Buf));
		/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DealData,sizeof(DealData));  /*�ն˵�TID*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DealTime,sizeof(DealTime));  /*�ն˵�TID*/
	Send_Buf[CmdLenght] = 0x03  ;
	CmdLenght+=0x03;
	i = MealComparefunDemo(0x0700,Send_Buf,CmdLenght);
	if(i == 0x01)  // �������ݳ�ʱ
		return 1 ;

	USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);
	Lenght = HL_BufferToInit(&rx1Buf[2]) ;

	if(Lenght == 0x00)
	{
		USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
		return 1 ;
	}
	for(j=0;j<Lenght+7;j++)
	{
//  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	}
	if(rx1Buf[0]==0x07 && rx1Buf[1]==0x10)  //��ʾ��ȷ
	{
		//  tmp_yy  = rx1Buf[7]/16*10+rx1Buf[7]%16 ;/*��*/
		tmp_yy  = rx1Buf[8]/16*10+rx1Buf[8]%16 ;/*��*/
		if(tmp_yy<14) return 1 ;// ������ݳ���
//       printf("tmp_yy=%d\r\n",tmp_yy);
		tmp_m2  = rx1Buf[9]/16*10+rx1Buf[9]%16 ;/*��*/
//       printf("tmp_yy=%d\r\n",tmp_m2);
		tmp_dd  = rx1Buf[10]/16*10+rx1Buf[10]%16 ;/*��*/
//       printf("tmp_dd=%d\r\n",tmp_dd);

		tmp_hh  = rx1Buf[14]/16*10+rx1Buf[14]%16 ;/*ʱ*/
		tmp_mm  = rx1Buf[15]/16*10+rx1Buf[15]%16 ;/*��*/
		tmp_ss  = rx1Buf[16]/16*10+rx1Buf[16]%16 ;/*��*/

		rx1Buf[7]  = rx1Buf[7]/16*10+rx1Buf[7]%16 ;/*��*/
		rx1Buf[8]  = rx1Buf[8]/16*10+rx1Buf[8]%16 ;/*��*/
		rx1Buf[9]  = rx1Buf[9]/16*10+rx1Buf[9]%16 ;/*��*/
		rx1Buf[10]  = rx1Buf[10]/16*10+rx1Buf[10]%16 ;/*��*/

		rx1Buf[14]  = rx1Buf[14]/16*10+rx1Buf[14]%16 ;/*ʱ*/
		rx1Buf[15]  = rx1Buf[15]/16*10+rx1Buf[15]%16 ;/*��*/
		rx1Buf[16]  = rx1Buf[16]/16*10+rx1Buf[16]%16 ;/*��*/
//       for(i=0;i<4;i++)
//       printf("rx1Buf[%d]=%d\r\n",i,rx1Buf[i+7]);

//       for(i=0;i<3;i++)
//       printf("rx1Buf[%d]=%d\r\n",i,rx1Buf[i+14]);
		USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
		return 0 ;
	}
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	return 1 ;
}


 /*******************************************************************************
* Function Name  : ȡ�ͷ�������  OK
* Description    : ȡ�ͷ�������
* Input          : void
* Output         : void
* Return         : void
		08 10 00 0f c0 00 01 24 c9 00 08 00 00 00 00 00 00 00 00 03 e8 47
	 //Ӧ���� 2λ          MAC 8λ
*******************************************************************************/

CustomerSel__struction CustomerSel;
uint8_t TakeMealsFun(uint8_t *SendBuffer,uint8_t takeout_flag)
{
	uint8_t i = 0 ;
	long  Lenght = 0 ,j=0;
	long  CmdLenght = 0 ;
	uint8_t  Send_Buf[512];  //Ҫ���͵�������
	memset(rx1Buf,0,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	GetBRWN();
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));  /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO)); /*�ն����ڵ�����*/
	for(i=0;i<6;i++)
	{
		DealBalance[3+i] = CustomerSel.DealBalance[i] ;
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DealBalance,sizeof(DealBalance)); /*���׽��(֧�����) */

	for(i=0;i<4;i++)
	{
	//���︳ֵ��Ʒ��ID
		MealID[3+i] = CustomerSel.MealID[i] ; //(10000020)
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealID,sizeof(MealID)); /*��ƷID*/

	/*���︳ֵ��Ʒ��ID*/
	MealNO[3] = CustomerSel.MealNo;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealNO,sizeof(MealNO)); /*��Ʒ��������*/

	//���︳ֵ��Ʒ������
	for(j=0;j<20;j++)
	{
		MealName[3+j]=CustomerSel.MealName[j];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealName,sizeof(MealName));        /*��Ʒ����*/

	/*���︳ֵ��Ʒ�ļ۸�*/
	for(i=0;i<6;i++)
		MealPrice[3+i] = CustomerSel.MealPrice[i] ;

	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealPrice,sizeof(MealPrice));      /*��Ʒ�۸�*/
	/*��Ǯ�ķ�ʽ*/
	PayType[3] = CustomerSel.PayType ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PayType,sizeof(PayType));        /*֧����ʽ*/
	/*������*/
	for(i=0;i<6;i++)
		Change[3+i] = CustomerSel.Change[i] ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Change,sizeof(Change));         /*������*/
	/*ʣ���Ʒ����*/
	for(i=0;i<2;i++)
	{
		RemainMealNum[3+i] = CustomerSel.RemainMealNum[i] ;
		//printf("CustomerSel.RemainMealNum[i]=%d\r\n",CustomerSel.RemainMealNum[i]);
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],RemainMealNum,sizeof(RemainMealNum));  /*ʣ���Ʒ����*/
	TakeMealFlag[4]= takeout_flag;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TakeMealFlag,sizeof(TakeMealFlag)); /*ȡ�ͱ��*/ 
	if(UserActMessageWriteToFlash.UserAct.PayType != '1')
	{     
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosDevNum,PosDevNum[2]+3); /*ˢ�����ն˺�*/
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosTenantNum,PosTenantNum[2]+3); /*ˢ�����̻���*/   
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosBatchNum,PosBatchNum[2]+3); /*ˢ����������ˮ��*/
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosUserNum,PosUserNum[2]+3); /*�û����п���*/
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MAC,sizeof(MAC));            /*MAC*/
	Send_Buf[CmdLenght] = 0x03  ;  //�������ݰ���β��ʶ��CRCУ����
	CmdLenght+=0x03;
	i = MealComparefunDemo(0x0800,Send_Buf,CmdLenght); //��������(ȡ�ͽ���)
	mem_copy00(SendBuffer, Send_Buf,CmdLenght);  //�ѷ��͵Ĳ�����������д��SD��
	/***************************************************************************/
	if(i == 0x01) /*�ж��Ƿ�ʱ*/
	return 1 ;
	Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	if(Lenght == 0x00)/*�ж����ݳ���*/
	return 1 ;
	for(j=0;j<Lenght+7;j++)
	{
		//printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	}
	if(rx1Buf[0]==0x08 && rx1Buf[1]==0x10)  //��ʾ��ȷ
	{
		if(rx1Buf[7]==0x00)/*�Ƿ�Ӧ������*/
		{
			return 0 ;
		}
		if(rx1Buf[7]==0x24)/*������ˮ�ţ��ն���ˮ���ظ�*/
		{
		//   Batch ++ ;  /*��ˮ���Լ�*/
			return 1 ;
		}
	}
	return 1 ;
}


 /*******************************************************************************
* Function Name  : SignInFunction
* Description    : ǩ������
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
uint8_t signin_state=0;
bool SignInFunction(void)
{
	uint8_t i = 80 ;
	do
	{
		if((signin_state=SignInFun())>2)
		break;
		delay_ms(100);
	}while(--i);
	if(i == 0x00)
		return false;
	else
		return true;
} 
		

/*******************************************************************************
 * ��������:StateSend
 * ��    ��:״̬����
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:void
 * �޸�����:2014��4��4��    ok
 *******************************************************************************/
uint32_t statesend_record=0;//������ֹ��δ���
uint8_t old_hours=0;
uint8_t old_minutes=0;
extern uint8_t Flag_szt_gpboc_ok;
void StateSend(void)
{
	RTC_TimeShow();//���ʱ��
	if(old_minutes!=TimeDate.Minutes)
	{
		old_minutes= TimeDate.Minutes;
		switch(TimeDate.Minutes)
		{
			case 5:
			case 10:
			case 15:
			case 20:
			case 25:
			case 30:
			case 35:
			case 40:
			case 45:       
			case 50:
			case 55:
			case 0:       
					StatusUploadingFun(0xE800); //״̬����
					if(sellmeal_flag== false)
					{
						if(Flag_szt_gpboc_ok != 1)
						{
							Szt_GpbocAutoCheckIn();
						}
					}
				break;
				default : break;
			} 
	}
	if(old_hours!= TimeDate.Hours)
	{
		old_hours= TimeDate.Hours;
		switch(old_hours)
		{
			case 13:
			case 14:
			case 15:
			case 16:
			SendtoServce();break;
			default:break;			
		}
	}
}

/*******************************************************************************
* Function Name  : SignInFunction
* Description    : ���ݻ���
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
bool EchoFuntion(void (*fptr)(void))
{
	uint8_t i = 100 ;
	do
	{
		if(EchoFun()==0x00)
		break;
		delay_ms(200);
	}while(--i);
	if(i == 0x00)
		return false;
	(*fptr)() ;
	return true;
}
	 /*******************************************************************************
* Function Name  :
* Description    : ��Ʒ���ݶԱ�
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
uint8_t MealDataCompareFunction(void)
{
	uint8_t i = 100;
	while(i)
	{
		if(MealDataCompareFun()==0x00)
		return 0 ;
		--i ;
		delay_ms(100);
	}
	return 1 ;
}



	 /*******************************************************************************
 * ��������:DataUpload
 * ��    ��:�����ϴ�
 *
 * ��    ��:��
 * ��    ��:��
 * ��    ��:void
 * �޸�����:2014��4��4��        ok
 *******************************************************************************/
extern uint8_t Current;
void DataUpload(uint8_t takemeal_flag)
{
	uint8_t cnt_t=0;
	itoa(f_name,TimeDate);    //��ʱ��ת�����ַ�
	if(UserActMessageWriteToFlash.UserAct.MealID==0x00)
	{
		for(cnt_t=0;cnt_t<10;cnt_t++)
		{
			printf("upload/UserAct.MealID == %d\r\n",UserActMessageWriteToFlash.UserAct.MealID);
			delay_ms(200);
		}
		while(1);
	}
	MealArr(UserActMessageWriteToFlash.UserAct.MealID);
	/*����ȡ�����ݸ�������*/
	memset(Record_buffer,0,254);
	if(TakeMealsFun(Record_buffer,takemeal_flag) == 0x01) //��ʾ����ʧ��
	{
		Sd_Write('n',takemeal_flag);//����ʧ��
	}
	else
	{
		Sd_Write('y',takemeal_flag);//�ı䵱ǰ�����λΪN0
	}
	UserActMessageWriteToFlash.UserAct.MealID= 0x00;//�����ϴ���һ�ζ�ID���㣬�����Ϳ���֪�������Ƿ��ϴ���
} 

 /*******************************************************************************
* Function Name  : ȡ�ͷ�������  OK
* Description    : ȡ�ͷ�������
* Input          : void
* Output         : void
* Return         : void
		08 10 00 0f c0 00 01 24 c9 00 08 00 00 00 00 00 00 00 00 03 e8 47
	 //Ӧ���� 2λ          MAC 8λ
*******************************************************************************/

CustomerSel__struction CustomerSel;

uint8_t ReadBuf[20]={0};
uint8_t TakeMealsFun1(uint8_t *SendBuffer)
{
	uint8_t temp[10]={0};
	uint8_t i = 0 ;
	long  Lenght = 0 ,j=0;
	long  CmdLenght = 0 ;
	uint8_t     Send_Buf[256]={0}; //Ҫ���͵�������
	mem_set_00(rx1Buf,sizeof(rx1Buf));
	SearchSeparator(ReadBuf,SendBuffer,1); //��ȡ��ͷ
	StringToHexGroup1(temp,ReadBuf,10);   
	/*ˮ����++*/
	for(i=0;i<5;i++)
	{
		Send_Buf[i] = temp[i] ;
	}
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/

	SearchSeparator(ReadBuf,SendBuffer,3); //��ȡ��ˮ��
	StringToHexGroup1(temp,ReadBuf,14);
	for(i=0;i<7;i++)
	{
		BRWN[3+i]=temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN)); /*��ˮ��*/

	SearchSeparator(ReadBuf,SendBuffer,4);	/*���κ�*/
	StringToHexGroup1(temp,ReadBuf,6);
	for(i=0;i<3;i++)
	{
		BNO[3+i]=temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO)); /*���κ�*/

	SearchSeparator(ReadBuf,SendBuffer,5);	/*�ն�����������*/
	StringToHexGroup1(temp,ReadBuf,6);
	for(i=0;i<3;i++)
	{
		DeviceArea[3+i]=temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea)); /*�ն�����������*/

	SearchSeparator(ReadBuf,SendBuffer,6);	/*�ն����ڵ�����*/
	StringToHexGroup1(temp,ReadBuf,8);
	for(i=0;i<4;i++)
	{
		DeviceAreaNO[3+i]=temp[i];
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO)); /*�ն����ڵ�����*/

	SearchSeparator(ReadBuf,SendBuffer,7); /*���׽��*/
	StringToHexGroup1(temp,ReadBuf,12); 
	for(i=0;i<6;i++)
	{
		DealBalance[3+i]=temp[i];
	}   
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DealBalance,sizeof(DealBalance)); /*���׽��(֧�����) */

	SearchSeparator(ReadBuf,SendBuffer,8); //��ƷID
	StringToHexGroup1(temp,ReadBuf,8); 
	for(i=0;i<4;i++)
	{
		MealID[3+i]=temp[i];
	}   
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealID,sizeof(MealID)); /*��ƷID*/

	SearchSeparator(ReadBuf,SendBuffer,9); //��Ʒ����
	StringToHexGroup1(temp,ReadBuf,2);
	MealNO[3]=temp[0];
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealNO,sizeof(MealNO)); /*��Ʒ��������*/

	SearchSeparator(ReadBuf,SendBuffer,10); //��Ʒ����
	for(i=0;i<20;i++)
	{
		MealName[3+i]=ReadBuf[i];
	}  
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealName,sizeof(MealName)); /*��Ʒ����*/

	SearchSeparator(ReadBuf,SendBuffer,11); //��Ʒ�۸�
	StringToHexGroup1(temp,ReadBuf,12); 
	for(i=0;i<6;i++)
	{
		MealPrice[3+i]=temp[i];
	}   
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealPrice,sizeof(MealPrice));      /*��Ʒ�۸�*/

	SearchSeparator(ReadBuf,SendBuffer,12); //���ʽ
	PayType[3]=ReadBuf[0];  
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PayType,PayType[2]+3);       /*֧����ʽ*/

	SearchSeparator(ReadBuf,SendBuffer,13); //������
	StringToHexGroup1(temp,ReadBuf,12); 
	for(i=0;i<6;i++)
	{
		Change[3+i]=temp[i];
	}     
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Change,sizeof(Change));         /*������*/

	SearchSeparator(ReadBuf,SendBuffer,14); //ʣ���Ʒ����
	StringToHexGroup1(temp,ReadBuf,4); 
	for(i=0;i<4;i++)
	{
		RemainMealNum[3+i]=temp[i];
	}   
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],RemainMealNum,sizeof(RemainMealNum));  /*ʣ���Ʒ����*/

	SearchSeparator(ReadBuf,SendBuffer,15); //ȡ�ͱ��
	StringToHexGroup1(temp,ReadBuf,4);    
	for(i=0;i<2;i++)
	{
		TakeMealFlag[3+i]=temp[i];
	}   
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TakeMealFlag,sizeof(TakeMealFlag));  /*ȡ�ͱ��*/

	if(PayType[3]!= '1')//�ֽ��ʱ���ϴ���������
	{
		memset(ReadBuf,0,sizeof(ReadBuf));
		SearchSeparator(ReadBuf,SendBuffer,16); //ˢ�����ն˺�
		PosDevNum[2]= strlen((char*)ReadBuf);
		for(i=0;i<PosDevNum[2];i++)
			PosDevNum[3+i]=ReadBuf[i];  //�ն˺�
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosDevNum,PosDevNum[2]+3); 

		memset(ReadBuf,0,sizeof(ReadBuf));
		SearchSeparator(ReadBuf,SendBuffer,17); //ˢ�����̺ź�
		PosTenantNum[2]= strlen((char*)ReadBuf);   
		for(i=0;i<PosTenantNum[2];i++)
			PosTenantNum[3+i]=ReadBuf[i];  //�̻���
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosTenantNum,PosTenantNum[2]+3);

		memset(ReadBuf,0,sizeof(ReadBuf));
		SearchSeparator(ReadBuf,SendBuffer,18); //ˢ����������ˮ��    
		PosBatchNum[2]= strlen((char*)ReadBuf); 
		for(i=0;i<PosBatchNum[2];i++)
			PosBatchNum[3+i]=ReadBuf[i];  //��ˮ��
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosBatchNum,PosBatchNum[2]+3);

		memset(ReadBuf,0,sizeof(ReadBuf));     
		SearchSeparator(ReadBuf,SendBuffer,19); //�û�����
		PosUserNum[2]= strlen((char*)ReadBuf);  
		for(i=0;i<PosUserNum[2];i++)
			PosUserNum[3+i]= ReadBuf[i];   //����  
		CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PosUserNum,PosUserNum[2]+3);    
	}  
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MAC,sizeof(MAC));            /*MAC*/  
	Send_Buf[CmdLenght] = 0x03  ;  //�������ݰ���β��ʶ��CRCУ����
	CmdLenght+=0x03;
	i = MealComparefunDemo(0x0800,Send_Buf,CmdLenght); //��������(ȡ�ͽ���)
	/***************************************************************************/

	if(i == 0x01) /*�ж��Ƿ�ʱ*/
		return 1 ;
	Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	if(Lenght == 0x00)/*�ж����ݳ���*/
		return 1 ;
	if(rx1Buf[0]==0x08 && rx1Buf[1]==0x10)  //��ʾ��ȷ
	{
		if(rx1Buf[7]==0x00)/*�Ƿ�Ӧ������*/
		{
			return 0 ;
		}
		if(rx1Buf[7]==0x24)/*������ˮ�ţ��ն���ˮ���ظ�*/
		{
			return 1 ;
		}
	}
	return 1 ;
}

uint8_t ReadSdBuff[512]={0};
//uint8_t SendHostBuff[128]={0};
//void DataUpload(void)
//{
//  //��ȡSD���е��������
//  //��ȡ��16��','���������
//  //�ж��Ƿ�Ϊ'N',����������
//  //��ȡ��16��','���������
//  uint8_t DelteFlag = 0 ;//��������Ƿ�ȫ���ϴ������ݵĺ���
//  uint8_t Times  = 0  ;
//  RTC_TimeShow();
//  itoa(f_name,TimeDate);    //��ʱ��ת�����ַ�

//    do
//    {
//       if(Fread(ReadSdBuff)== 0x00)               //�����¼��Ȼ��������ʾ����
//       break ;
//    }while(1);
//  SearchSeparator(ReadBuf,ReadSdBuff,17);
//  if(ReadBuf[0]=='N')
//  {
//     if(TakeMealsFun1(ReadSdBuff)==0);
//     //�ڵ�ʮ�������ź�дY
//     
////    SearchSeparator(ReadBuf,ReadSdBuff,FH); //��ȡ��ͷ
////    memcpy_02(SendHostBuff,ReadBuf,6);
////    SearchSeparator(SendHostBuff,ReadSdBuff,TID); //��ȡTID
////    //��ȡBuffer
////    SendDataToHost();
//  }
//  if(ReadBuf[0]=='Y')
//  {
//        
//  }
//}



 /*******************************************************************************
 * ��������:StringToHexGroup1                                                                    
 * ��    ��:���ַ�����ת��Ϊhex����,���������޸�                                                                  
 *                                                                               
 * ��    ��:                                                                   
 * ��    ��:bool                                                                      
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��  
 *******************************************************************************/
bool StringToHexGroup1(uint8_t *OutHexBuffer, uint8_t *InStrBuffer, unsigned int strLength)
{
	unsigned int i, k=0;
	uint8_t HByte,LByte;
//  if(InStrBuffer[strLength-1]!=0x0A)
//    return false;
	for(i=0; i<strLength; i=i+2)
	{
		if(InStrBuffer[i]>='0' && InStrBuffer[i]<='9')
		{
			HByte=InStrBuffer[i]-'0';
		}
		else if(InStrBuffer[i]>='A' && InStrBuffer[i]<='F')
		{
			HByte=InStrBuffer[i]-'A' +10;
		}
		else
		{
			HByte=InStrBuffer[i];
			return false;
		}
		HByte=HByte <<4;
		HByte = HByte & 0xF0;
		if(InStrBuffer[i+1]>='0' && InStrBuffer[i+1]<='9')
		{
			LByte=InStrBuffer[i+1]-'0';
		}
		else if(InStrBuffer[i+1]>='A' && InStrBuffer[i+1]<='F')
		{
			LByte=InStrBuffer[i+1]-'A' +10;
		}
		else
		{
			LByte=InStrBuffer[i];
			return false;
		}
		OutHexBuffer[k++]=HByte |LByte;   
	}
	return true;
}
