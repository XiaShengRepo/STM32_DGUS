#include "stm32f4xx.h"
#include "uart5.h"
#include "network_handle.h"
#include "printer.h"
#include "delay.h"
#include "rtc.h"
#include "stdio.h"

static long Batch = 0x00 ;//������ˮ��  	

unsigned char  TID[7] = {0xa2,0x00,0x04,0x10,0x00,0x00,0x04}; /*�ն�TID�� 10000004*/
unsigned char  BRWN[7+3] = {0xa6,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};	 /*������ˮ��*/
unsigned char  BNO[6] = {0xa7,0x00,0x03,0x00,0x00,0x00};               /*���κ�*/
unsigned char  DeviceArea[3+3]={0xac,0x00,0x03,0x17,0x03,0x01};         /*�ն�����������*/
unsigned char  DeviceAreaNO[4+3]={0xad,0x00,0x04,0x17,0x03,0x01,0x06};   /*�ն����ڵ�����*/  
unsigned char  DeviceStatus[2+3]={0xae,0x00,0x02,'0','0'};	   /*�ն�״̬*/
unsigned char  DealData[7]={0xa9,0x00,0x04,0x00,0x00,0x00,0x00};       /*��������*/
unsigned char  DealTime[6]={0xaa,0x00,0x03,0x00,0x00,0x00};       /*����ʱ��*/   
unsigned char  MAC[8+3]={0xc9,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   /*MAC*/
unsigned char  DealBalance[6+3]={0xb0,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};    /*���׽��*/
unsigned char  MealID[4+3] = {0xb1,0x00,0x04,0x10,0x00,0x00,0x20} ;                /*��ƷID  10000020*/
unsigned char  MealNO[1+3] = {0xb2,0x00,0x01,0x00};		   /*��Ʒ��������*/
unsigned char  MealName[20+3] ={0xb3,0x00,0x14,'a','b','c','d','e','a','b','c','d','e','a','b','c','d','e','a','b','c','d','a'};  /*��Ʒ����*/
unsigned char  MealPrice[6+3]={0xbd,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};	  /*��Ʒ�۸�*/
unsigned char  PayType[1+3]={0xbf,0x00,0x01,0x00};        /*֧����ʽ*/
unsigned char  Change[6+3]={0xd7,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00} ;        /*������*/
unsigned char  RemainMealNum[2+3]={0xd8,0x00,0x02,0x00,0x00};  /*ʣ���Ʒ����*/
unsigned char  MName[20+3]={0xa4,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};             /*�̻���MID*/
unsigned char  APPVersion[8+3]={ 0xc2,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};       /*Ӧ�ó���汾*/
unsigned char  ParaFileVersion[8+3]={0xc3,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};  /*�����ļ��汾*/ 
unsigned char  BDataFileVersion[8+3]={0xc4,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};	/*ҵ�������ļ��汾*/ 
unsigned char  ChechStatus[2+3]={0xab,0x00,0x02,0x00,0x00};                                     /*�Լ�״̬*/
unsigned char  MID[3+3]={ 0xa3,0x00,0x03,0x10,0x00,0x01};                                       /*�̻���MID*/
unsigned char  TTCount[3+2]     ={0xb7,0x00,0x02,0x00,0x00};	                                            /*�����ܱ��� */
unsigned char  TNCT[6+3]        ={0xb8,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};                            /*�����ܽ��*/
unsigned char  TNtype[3+2]      ={0xd9,0x00,0x02,0x00,0x00};											    /*�����ܲ�Ʒ�� d9*/
unsigned char  TotalChange[3+6] ={0xd7,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00};							/*�������� d7*/
	
unsigned char  UpdataFlag[4];
unsigned char  ACK[3];  //�����ʲô?
unsigned char  WordKeyCipher[11];
Meal_struction Meal[9]={
	                        /*��ƷID*/				/*��Ʒ����*/  	       /*��Ʒ����*/	     /*��Ʒ�۸�*/			  /*��Ʒ����*/
							      0x10,0x00,0x00,0x20,  {"���ܲ�����          "}, 0x00,0xc8,      0x00,0x00,0x15,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x21,  {"�˸��嵰            "}, 0x00,0x00,      0x00,0x00,0x15,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x22,  {"����ţ��            "}, 0x00,0x00,      0x00,0x00,0x15,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x23,  {"�㹽����            "}, 0x00,0x00,      0x00,0x00,0x20,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x24,  {"�������            "}, 0x00,0x00,      0x00,0x00,0x20,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x25,  {"�������            "}, 0x00,0x00,      0x00,0x00,0x20,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x26,  {"��Ƥ��Ѽ            "}, 0x00,0x00,      0x00,0x00,0x25,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x27,  {"÷�˿���            "}, 0x00,0x00,      0x00,0x00,0x25,0x00,	   {"C005"},
							      0x10,0x00,0x00,0x28,  {"���Ʋ���            "}, 0x00,0x00,      0x00,0x00,0x25,0x00,	   {"C005"},	
	                      };

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
	BRWN[3] =    20 /10 *16 + 20%10 ;	        /*��*/
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
static unsigned int GetCrc16(unsigned char *bufData,unsigned int sizeData)
 {
  	 unsigned int Crc ,i = 0;
	 unsigned char j = 0;
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
void HL_IntToBuffer(const uint16_t int_v, unsigned char *ret_buf);
����: ��һ�� 16bit ������ת���� Buffer ����, ��λ��ǰ
��λ��ǰ
*/
static void HL_IntToBuffer(const uint16_t int_v, unsigned char *ret_buf)
{
    ret_buf[0] = (unsigned char)(int_v >> 8);  // [0] ��Ӧ --> ��λ
    ret_buf[1] = (unsigned char)(int_v & 0xff);

 
}
/*---------------------------------------------------------------------------
 uint16_t HL_BufferToInt(const unsigned char *in_buf);
 ����: ������������ת��������, ��λ��ǰ
 ��λ��ǰ
*/
uint16_t HL_BufferToInit(const unsigned char *in_buf)
{
    uint16_t ret_n;

    ret_n = in_buf[0];           // ȡ��λ
    ret_n = (ret_n << 8);

    ret_n += in_buf[1];          // ȡ��λ

    return ret_n;
}

/*---------------------------------------------------------------------------
uint32_t HL_BufferToLong(const unsigned char *in_buf);
����: �� (unsigned char *) ����ת���� Long ��, ��λ��ǰ
��λ��ǰ
*/
uint32_t HL_BufferToLong00(const unsigned char *in_buf)
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
void LH_LongToBuffer(uint32_t in_n, unsigned char *ret_buf);
����: ��������ת���� unsigned char ������, ��λ��ǰ
*/
void HL_LongToBuffer00(const uint32_t in_n, unsigned char *ret_buf)
{
    ret_buf[0] = (unsigned char)((in_n >> 24) & 0xff);   // [0] ��Ӧ --> ��λ
    ret_buf[1] = (unsigned char)((in_n >> 16) & 0xff);
    ret_buf[2] = (unsigned char)((in_n >> 8) & 0xff);
    ret_buf[3] = (unsigned char)(in_n & 0xff);

    
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
static void mem_set_00(unsigned char *dest, const long s_len)
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
uint16_t mem_copy00(unsigned char *dest, const unsigned char *source, const long s_len)
{
    long j0;

    for(j0 = 0; j0 < s_len; j0++)  dest[j0] = source[j0];

    return s_len;
}
/*********************/
typedef struct
{
 unsigned char  Lenght[2] ;
 unsigned char  MealNO[35*9];    /*��Ʒ������Ŀ*/

}ReturnData_Struction;
ReturnData_Struction  ReturnData;

  /*******************************************************************************
* Function Name  : CheckResponseCode
* Description    : ��鷵���뿴ʲô����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
unsigned char CheckResponseCode(unsigned char Cmd)
{
// unsigned char  i = 0 ;
 switch(Cmd)
 {
 	case 0x00 :  
	              /*���׳ɹ�*/
	             return 0 ;
	case 0x01 :/*��CRC����*/
	             
				  return 1 ;
			 //   break ;
	case 0x02 :  /*��CRC����*/	 
	            
	            break;
	case 0x03 :/*��Ч���ն�����*/
			    break  ;
	case 0x04 :/*Э��汾����*/
	            break ;
	case 0x05 : /*��Ч���ն�SN*/
				break;
	case 0x06 : /*��Ч���̻���(MID)*/
	            break;
	case 0x07 :	/*��Ч�ļ�ƫ�Ƶ�ַ*/
	            break ;
	case 0x08 : /*��Ч�ļ�ƫ�Ƶ�ַ*/
	            break ;
	case 0x09 : /*���������ݴ���*/
				break;
	case 10 :	/*�̻������豸�Ŷ�Ӧ��ϵ����*/
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
	default :   break ;
 
 
 
 
 }
}
 /*******************************************************************************
* Function Name  : GetData
* Description    : �õ���Ӧ���������
* Input          : unsigned char *dest,unsigned char *souce,const long s_len,unsigned char Cmd
* Output         : void
* Return         : void
*******************************************************************************/ 
 long   GetData(unsigned char *dest,unsigned char *souce, long s_len,unsigned char Cmd)
 {
	  long i = 0 ,j=0,Cmdlenght=0;
	  for(i=0;i<s_len;i++)
	  {
	  
	   if(souce[i] == Cmd)	 /*�õ�����*/
	    {
		   Cmdlenght= HL_BufferToInit(&souce[i+1]);    /*�õ����ݳ���*/
//		   printf("Cmdlenght=%d\r\n",Cmdlenght);
		   for(j=0;j<Cmdlenght;j++)				      /*�õ�����  */
		   {
			 dest[j] = souce[i+j+3] ;
//			 printf("dest[%d]=%x\r\n",j,dest[j]);
			 
		   }
		  return Cmdlenght  ;
		}
	 
	  }
  	     return 0 ;
 }
 /*******************************************************************************
* Function Name  : CmdDataSend
* Description    : �������ݺ���
* Input          : unsigned char *p  long Lenght
* Output         : void
* Return         : void
*******************************************************************************/
static void CmdDataSend(unsigned char *p,long Lenght)
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
//	    printf(" p[%d]=%x\r\n", i,p[i]);
	}
   
 
}

 
unsigned char  MealComparefunDemo(long Cmd ,unsigned char *p,unsigned long lenght)
{

 long  CRCValue= 0 ;
// static  int Bno = 0 ;
 unsigned char Waittimeout = 250 ;
 /*�����鸳ֵ����*/
 HL_IntToBuffer(Cmd,&p[1]);
 /*���鳤��*/	 
 HL_IntToBuffer(lenght-8,&p[3]);
 /*�õ����ݵ�CRC*/	    
 CRCValue=GetCrc16(&p[1],lenght-4);
 /*��CRC��ֵ��������*/
 HL_IntToBuffer(CRCValue,&p[lenght-2]);
 /*��������*/
 CmdDataSend(p,lenght);	//��������
 /*�ȴ����ݷ���*/
 while(--Waittimeout)
	{
		   delay_ms(10);
		   if(F_RX1_Right)
		   {
		   F_RX1_Right = 0 ;
		   break;
		   }		   
   }

  if(Waittimeout == 0 )
  return 1 ;
 
  return 0 ;

}


/*******************************************************************************
����ǩ���󣬷��ظ��ն˵Ľṹ������
*******************************************************************************/				

/*ǩ��*/	            //0X0100
unsigned char  SignInFun(void)
{
	 unsigned char i = 0 ;
	 long  Command = 0x0100 ;
	 long  Lenght = 0,j = 0  ;
	 long 	CmdLenght = 0 ;
	 unsigned char 	  Send_Buf[400];
	 mem_set_00(rx1Buf,sizeof(rx1Buf));	 //��������

	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;														
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));      /*�ն˵�TID*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MID,sizeof(MID));  /*�̻���TID ����(1)+���ݳ���(2)+����     100001*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MName,sizeof(MName));  /*�̻�����*/
	GetBRWN();														 /*�õ���ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));   /*������ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));     /*���κ�*/
    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));  /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO)); /*�ն����ڵ�����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],APPVersion,sizeof(APPVersion)); /*�ն����ڵ�����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],ParaFileVersion,sizeof(ParaFileVersion)); /*�ն����ڵ�����*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BDataFileVersion,sizeof(BDataFileVersion)); /*�ն����ڵ�����*/
    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],ChechStatus,sizeof(ChechStatus)); /*�ն����ڵ�����*/
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;
	 /*��������*/
     i = MealComparefunDemo(Command,Send_Buf,CmdLenght);
 	 if(i == 0x01)	 //
 	 return 1;  	  	 
	 Lenght = HL_BufferToInit(&rx1Buf[2]);	 //�ж����ݳ����Ƿ�
	 if(Lenght == 0x00)
	 return  1 ;
//	 if(rx1Buf[7]!=0x00)/*�Ƿ�Ӧ������*/
//	 return 1 ;
//     for(j=0;j<Lenght;j++)
//	 {
//	  CheckInitReturnUnion.CheckInitReturnBuffer[j]= rx1Buf[j];
//	  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);	 
//	 }
//	 printf("\r\nACK: ");
//	 for(i=0;i<4;i++)
//	 {
//	 ACK[i] = rx1Buf[i+0x46];//�õ�
//	 printf("%x ",ACK[i]);
//	 }
//	 if(ACK[3]!=0x00)
//	 {
//	   return 1 ;
//	 }


//	 printf("\r\nMName: ");
	 for(i=0;i<23;i++)
	 {
	 MName[i] = rx1Buf[i+4];//�õ��̻�
//	 printf("%x ",MName[i]);
	 }
//	 printf("\r\nBNO: ");
	 for(i=0;i<3;i++)
	 {
	 BNO[i+3] = rx1Buf[i+27+3];//�õ����κ�
//	 printf("%x ",BNO[i+3]);
	 }										 
//	 printf("\r\nAPPVersion: ");
	 for(i=0;i<11;i++)
	 {
	 APPVersion[i] = rx1Buf[i+0x21];//�õ����κ�
//	 printf("%x ",APPVersion[i]);
	 }
//	 printf("\r\nParaFileVersion: ");
	 for(i=0;i<11;i++)
	 {
	 ParaFileVersion[i] = rx1Buf[i+0x2c];//�õ����κ�
//	 printf("%x ",ParaFileVersion[i]);
	 }
	 printf("\r\nBDataFileVersion: ");
	 for(i=0;i<11;i++)
	 {
	 BDataFileVersion[i] = rx1Buf[i+0x37];//�õ����κ�
//	 printf("%x ",BDataFileVersion[i]); 
	 }
//	 printf("\r\nUpdataFlag: ");
	 for(i=0;i<4;i++)
	 {
	 UpdataFlag[i] = rx1Buf[i+0x42];//�õ����κ�
//	 printf("%x ",UpdataFlag[i]);
	 }

//	 printf("\r\nACK: ");
	 for(i=0;i<4;i++)
	 {
	 ACK[i] = rx1Buf[i+0x46];//�õ�
//	 printf("%x ",ACK[i]);
	 }
//
 //    printf("\r\nWordKeyCipher: ");
	 for(i=0;i<11;i++)
	 {
	 WordKeyCipher[i] = rx1Buf[i+0x4a];//�õ����κ�
//	 printf("%x ",WordKeyCipher[i]);
	 }

//	 printf("\r\nMAC: ");						
     for(i=0;i<11;i++)
	 {
	 MAC[i] = rx1Buf[i+0x55];//�õ���������
//	 printf("%x ",MAC[i]);
	 }
     if(ACK[3]==0x24)
	 {
	   return 1 ;
	 }

   return 0 ;

}
void StructCopyToBuffer(unsigned char *dest)
{
    long j0=0,i=0,k=0;
   	 
    for(j0 = 0; j0 < 9; j0++) 
	{
	// dest[j0] = source[j0];
	  for(i=0;i<4;i++)
	  dest[k++]=Meal[j0].MealID[i];
	  for(i=0;i<20;i++)
	  dest[k++]=Meal[j0].MaelName[i];
	   
	  for(i=0;i<2;i++)
	  dest[k++]=Meal[j0].MealNum[i];
	    
	  for(i=0;i<4;i++)
	  dest[k++]=Meal[j0].MealPreace[i];
	   
	  for(i=0;i<4;i++)
	  dest[k++]=Meal[j0].MealType[i];
	 }
 
}
void BufferToStructCopy(unsigned char *dest,unsigned char Index)
{
      long i=0,k=0;
   	   
	  for(i=0;i<4;i++)
	  Meal[Index].MealID[i]=dest[k++];
	  for(i=0;i<20;i++)
	  Meal[Index].MaelName[i]=dest[k++];
	   
	  for(i=0;i<2;i++)
	  {
	  Meal[Index].MealNum[i]=dest[k++];
	//  printf("Meal[%d].MealNum[%d]=%x\r\n",Index,i,Meal[Index].MealNum[i]);
	   } 
	  for(i=0;i<4;i++)
	  Meal[Index].MealPreace[i]=dest[k++];
	   
	  for(i=0;i<4;i++)
	  Meal[Index].MealType[i]=dest[k++];
	 
 
}

/*��Ʒ�Ա�*/	            //0X0200
unsigned char  MealDataCompareFun(void)
{

 	unsigned char i = 0 ;
  long  Lenght = 0 ,j;
//	unsigned char MealID = 0 ;
	long  CmdLenght = 0 ;
	unsigned char status = 0 ;
	unsigned char 	  Send_Buf[400];
	unsigned char     TempBuffer[35*9]={0};
	unsigned char     Buffer[306]={0};
	mem_set_00(rx1Buf,sizeof(rx1Buf));

	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	GetBRWN(); /*�õ�ˮ����*/
//	Batch ++ ;
//	Batch = 240 ;
//	BRWN[3] =    Batch /10000 ;
//	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
//	BRWN[4] =	 Batch /100 %100 ;
//	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
//	BRWN[5] =	 Batch %100;
//	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));                   /*������ˮ��*/
    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));                     /*���κ�*/
    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));       /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO));   /*�ն����ڵ�����*/
 	Send_Buf[CmdLenght] =  0xbc ;	 //T
	Send_Buf[CmdLenght+1] =  0x01 ;	 //L
	Send_Buf[CmdLenght+2] =  0x32 ;	  //V
	CmdLenght += 3;
	StructCopyToBuffer(Buffer);
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Buffer,sizeof(Buffer));               /*�ն����ڵ�����*/ 
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;									
	
	/*��������*/
    i = MealComparefunDemo(0x0200,Send_Buf,CmdLenght);
 
    /*��Ƭ�Ƿ�ɹ�*/
	if(i==0x01)
	return 1;
	Lenght = HL_BufferToInit(&rx1Buf[2]);		//�õ����ݳ���
	GetData(&ReturnData.Lenght[0],rx1Buf,Lenght,0xc0);	  /*����״̬*/
 	printf("Status=%x\r\n",ReturnData.Lenght[0]);
  if(ReturnData.Lenght[0] == 0x00 )
  return 0;	 /*������ȷ*/
	if(ReturnData.Lenght[0] == 0x24 )
	{
	 Batch ++ ;
     return 1;	 /*������ȷ*/
	}
	   CmdLenght = GetData(TempBuffer,rx1Buf,Lenght,0xBC);/*��Ʒ�Ա�*/
//	   	printf("StatusCmdLenght=%x\r\n",CmdLenght);
	   if(CmdLenght>34)
	   {				 
	      status  = CmdLenght / 35  ;
//		  	printf("Statusstatus=%x\r\n",status);
		  for(i=0;i<status;i++)
		  {
	       if(rx1Buf[45+i*35] ==0x03)
		   {
//		      printf("rx1Buf[%d]=%x\r\n",i,rx1Buf[45+i*35]);
//			  printf("rx1Buf[%d]=%x\r\n",i,rx1Buf[14+i*35]);
	          for(j=0;j<34;j++)
		     {
		      BufferToStructCopy(&TempBuffer[i*35],rx1Buf[14+35*i]-0x20);
		     }
		   }
		  }
		   return 1 ;/*��Ʒ�Աȴ���*/
	   }

    for(j=0;j<Lenght;j++)
    {
//	  CheckInitReturnUnion.CheckInitReturnBuffer[j]= rx1Buf[j];
//	  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
    }

  return 0;

}

	          
/*******************************************************************************
* Function Name  : 2.3.3 ǩ��		 0X0300
* Description    :��ǩ����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
unsigned char SignOutFun()
{

 	unsigned char i = 0 ;
    long  Lenght = 0 ,j;
	long 	CmdLenght = 0 ;
	unsigned char 	  Send_Buf[400];

   	mem_set_00(rx1Buf,sizeof(rx1Buf));
  /*ˮ����++*/
	 
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	Batch ++ ;
	Batch = 239 ;
	BRWN[3] =    Batch /1000000000000 ;
	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
	BRWN[4] =	 Batch /10000000000 %100 ;
	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
	BRWN[5] =	 Batch /100000000%100;
	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
	BRWN[6] =	 Batch /1000000%100;
	BRWN[6] =    BRWN[6] /10 *16 + BRWN[6]%10 ;
	BRWN[7] =	 Batch /10000 %100 ;
	BRWN[7] =    BRWN[7] /10 *16 + BRWN[7]%10 ;
	BRWN[8] =	 Batch/100 %100;
	BRWN[8] =    BRWN[8] /10 *16 + BRWN[8]%10 ;
	BRWN[9] =	 Batch %100;
	BRWN[9] =    BRWN[9] /10 *16 + BRWN[9]%10 ;

	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;
    i = MealComparefunDemo(0x0300,Send_Buf,CmdLenght);

	if(i == 0x01)
	return 1 ;
	 Lenght = HL_BufferToInit(&rx1Buf[2]);
	 for(j=0;j<Lenght+7;j++)
	 {
	 
//	  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	  }
  
  return 0 ;

}


	       
 /*******************************************************************************
* Function Name  : ״̬�ϴ�����		 0X0300
* Description    :��ǩ����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
unsigned char StatusUploadingFun()
{

 	unsigned char i = 0 ;
	long  Lenght = 0 ,j;
	long 	CmdLenght = 0 ;
	unsigned char 	  Send_Buf[400];
	mem_set_00(rx1Buf,sizeof(rx1Buf));
    /*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	Batch ++ ;
	BRWN[3] =    Batch /1000000000000 ;
	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
	BRWN[4] =	 Batch /10000000000 %100 ;
	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
	BRWN[5] =	 Batch /100000000%100;
	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
	BRWN[6] =	 Batch /1000000%100;
	BRWN[6] =    BRWN[6] /10 *16 + BRWN[6]%10 ;
	BRWN[7] =	 Batch /10000 %100 ;
	BRWN[7] =    BRWN[7] /10 *16 + BRWN[7]%10 ;
	BRWN[8] =	 Batch/100 %100;
	BRWN[8] =    BRWN[8] /10 *16 + BRWN[8]%10 ;
	BRWN[9] =	 Batch %100;
	BRWN[9] =    BRWN[9] /10 *16 + BRWN[9]%10 ;


	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(DeviceStatus));  /*�ն˵�״̬*/
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;
    i = MealComparefunDemo(0x0400,Send_Buf,CmdLenght);
	if(i == 0x01)
	return 1 ;
    Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	for(j=0;j<Lenght+7;j++)
	{
	 
//	 printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	
	}
  return 0 ;

}

 /*******************************************************************************
* Function Name  : ״̬�ϴ�����		 0X0300
* Description    :��ǩ����
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
unsigned char EchoFun(void)
{

 	unsigned char i = 0 ;
	long  Lenght = 0 ,j;
	long 	CmdLenght = 0 ;
	unsigned char 	  Send_Buf[400];
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
	Send_Buf[CmdLenght] = 0x03 	;
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
//	 printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	}
	 if(rx1Buf[0]==0x07 && rx1Buf[1]==0x10)	 //��ʾ��ȷ
	 {


		 //  tmp_yy  = rx1Buf[7]/16*10+rx1Buf[7]%16 ;/*��*/
		   tmp_yy  = rx1Buf[8]/16*10+rx1Buf[8]%16 ;/*��*/ 
//		   printf("tmp_yy=%d\r\n",tmp_yy); 
		   tmp_m2  = rx1Buf[9]/16*10+rx1Buf[9]%16 ;/*��*/
//		   printf("tmp_yy=%d\r\n",tmp_m2); 
		   tmp_dd  = rx1Buf[10]/16*10+rx1Buf[10]%16 ;/*��*/
//		   printf("tmp_dd=%d\r\n",tmp_dd); 

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
//		   for(i=0;i<4;i++)
//		   printf("rx1Buf[%d]=%d\r\n",i,rx1Buf[i+7]);

//		   for(i=0;i<3;i++)
//		   printf("rx1Buf[%d]=%d\r\n",i,rx1Buf[i+14]);

		 USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);	
	  return 0 ;  
	 }

	 	 USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
    return 1 ;

}


 /*******************************************************************************
* Function Name  : ״̬�ϴ�����		 0X0300
* Description    :��ǩ����
* Input          : void
* Output         : void
* Return         : void
                                       08 10 00 0f c0 00 01 24 c9 00 08 00 00 00 00 00 00 00 00 03 e8 47 
                                     //Ӧ���� 2λ          MAC 8λ
*******************************************************************************/

CustomerSel__struction CustomerSel;
unsigned char TakeMealsFun(unsigned char *SendBuffer)
//unsigned char TakeMealsFun(unsigned char *Cardbuff,long 	CardLenght)
{

 	unsigned char i = 0 ;
	long  Lenght = 0 ,j=0;
	long  CmdLenght = 0 ;
	unsigned char 	  Send_Buf[1024];  //Ҫ���͵�������
	mem_set_00(rx1Buf,sizeof(rx1Buf));
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
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DealBalance,sizeof(DealBalance)); /*���׽��*/
	for(i=0;i<4;i++)
	{
	 //���︳ֵ��Ʒ��ID
	  MealID[3+i] = CustomerSel.MealID[i] ;
	}
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealID,sizeof(MealID)); /*��ƷID*/
	
	/*���︳ֵ��Ʒ��ID*/
    MealNO[3] = CustomerSel.MealNo; 
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealNO,sizeof(MealNO)); /*��Ʒ��������*/

	 //���︳ֵ��Ʒ������					  
	 for(j=0;j<20;j++)
	 MealName[3+j]=Meal[CustomerSel.MealName-1].MaelName[j];
	 CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealName,sizeof(MealName));             /*��Ʒ����*/
	/*���︳ֵ��Ʒ�ļ۸�*/
	for(i=0;i<6;i++)
    MealPrice[3+i] = CustomerSel.MealPrice[i] ; 

    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MealPrice,sizeof(MealPrice));          /*��Ʒ�۸�*/	
   /*��Ǯ�ķ�ʽ*/
	PayType[3] = CustomerSel.PayType ;										
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],PayType,sizeof(PayType));              /*֧����ʽ*/
	/*������*/
	for(i=0;i<6;i++)
	Change[3+i] = CustomerSel.Change[i] ;	    
   CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Change,sizeof(Change));                /*������*/
	/*ʣ���Ʒ����*/
	for(i=0;i<2;i++)
	{
     RemainMealNum[3+i] = CustomerSel.RemainMealNum[i] ;
//	 printf("CustomerSel.RemainMealNum[i]=%d\r\n",CustomerSel.RemainMealNum[i]);
	}
 	  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],RemainMealNum,sizeof(RemainMealNum));  /*ʣ���Ʒ����*/			 
	  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MAC,sizeof(MAC));                      /*MAC*/
//  if(UserAct.PayType == '2' )	                                                                       /* ��ʾ�����ˢ��*/
//	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],STATUS.PacketData,STATUS.DataLength-17);                /*��¼ˢ����Ϣ*/
//	if(UserAct.PayType == '3')
//  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],STATUS.PacketData,STATUS.DataLength-17);                /*��¼ˢ����Ϣ*/	
	
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;	
	                                
  i = MealComparefunDemo(0x0800,Send_Buf,CmdLenght);
  mem_copy00(SendBuffer, Send_Buf,CmdLenght);
	
	HL_IntToBuffer(CmdLenght,&SendBuffer[1019]);   /*�������ݵĳ���*/
	if(i == 0x01) /*�ж��Ƿ�ʱ*/
	return 1 ;	 
  Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	if(Lenght == 0x00)/*�ж����ݳ���*/
	return 1 ;
	for(j=0;j<Lenght+7;j++)
	{
	 
//	 printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);

	 }
	 if(rx1Buf[0]==0x07 && rx1Buf[1]==0x10)	 //��ʾ��ȷ
	 {
	  	
		if(rx1Buf[7]==0x00)/*�Ƿ�Ӧ������*/
		{
		   return 0 ;
		}
		if(rx1Buf[7]==0x24)/*������ˮ�ţ��ն���ˮ���ظ�*/
		{
		 //   Batch ++ ;   /*��ˮ���Լ�*/
           return 1 ;
		}
	   
	 }
  return 0 ;

}



 /*******************************************************************************
* Function Name  : MealUploadingFun		 0X0300
* Description    : ��Ʒ����
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/
 unsigned char MealUploadingFun(void)
 {
   	unsigned char i = 0 ;
	long  Lenght = 0 ,j=0;
	unsigned char 	  Send_Buf[400];
	long  CmdLenght = 0 ;	 
	/*���ܻ�������0*/
	mem_set_00(rx1Buf,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/

	BRWN[3] =    Batch /1000000000000 ;
	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
	BRWN[4] =	 Batch /10000000000 %100 ;
	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
	BRWN[5] =	 Batch /100000000%100;
	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
	BRWN[6] =	 Batch /1000000%100;
	BRWN[6] =    BRWN[6] /10 *16 + BRWN[6]%10 ;
	BRWN[7] =	 Batch /10000 %100 ;
	BRWN[7] =    BRWN[7] /10 *16 + BRWN[7]%10 ;
	BRWN[8] =	 Batch/100 %100;
	BRWN[8] =    BRWN[8] /10 *16 + BRWN[8]%10 ;
	BRWN[9] =	 Batch %100;
	BRWN[9] =    BRWN[9] /10 *16 + BRWN[9]%10 ;



	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceArea,sizeof(DeviceArea));  /*�ն�����������*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],DeviceAreaNO,sizeof(DeviceAreaNO)); /*�ն����ڵ�����*/


	 /*
	 
	  ��������û���ϴ��Ĳ�Ʒ����
	 
	 */


    CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MAC,sizeof(MAC)); /*MAC*/
	  Send_Buf[CmdLenght] = 0x03 	;
	  CmdLenght+=0x03;


	   i = MealComparefunDemo(0x0400,Send_Buf,CmdLenght);


	if(i == 0x01)
	return 1 ;
  Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	for(j=0;j<Lenght+7;j++)
	{
//	 printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	}
	return 0 ;
 }



 /*******************************************************************************
* Function Name  : MealGroupUploadingFun		 ������
* Description    : ��Ʒ������
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/

typedef struct 
{
  unsigned char  BRWN[3];           /*������ˮ��*/
  unsigned char  MealID[4];         /*��ƷID*/
  unsigned char  DealBalance[6];    /*���׽��*/
  unsigned char  MealNO;		    /*��Ʒ��������*/
  unsigned char	 RemainMealNum[2];  /*ʣ���Ʒ����*/
  unsigned char  MealPrice[6];	    /*��Ʒ�۸�*/
  unsigned char  PayType;           /*֧����ʽ*/
  unsigned char  Change[6];         /*������*/
  unsigned char  TimeData[7];		/*�����ʱ��*/
}MealGroup_struction;
 MealGroup_struction 	 MealGroup[9]={		/*������ˮ��*/ /*��ƷID*/		  /*���׽��*/            /*��Ʒ��������*/    /*ʣ���Ʒ����*/		/*��Ʒ�۸�*/	  /*֧����ʽ*/			/*������*/			  /*�����ʱ��*/
                                         {0x00,0x00,0x01, 0x10,0x00,0x00,0x20, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,   0x00,0x00,   0x00,0x00,0x00,0x00,0x15,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
                                       	 {0x00,0x00,0x02, 0x10,0x00,0x00,0x21, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x15,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x03, 0x10,0x00,0x00,0x22, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x15,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x04, 0x10,0x00,0x00,0x23, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x20,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x05, 0x10,0x00,0x00,0x24, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x20,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x06, 0x10,0x00,0x00,0x25, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x20,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x07, 0x10,0x00,0x00,0x26, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x25,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x08, 0x10,0x00,0x00,0x27, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x25,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
										 {0x00,0x00,0x09, 0x10,0x00,0x00,0x28, 0x00,0x00,0x00,0x00,0x00,0x00,  0x00,    0x00,0x00,  0x00,0x00,0x00,0x00,0x25,0x00,  0x00,  0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00},
									    };

void StructCopyToBuffer00(unsigned char *dest,unsigned char index)
{

    long i=0,k=0;
   	for(i=0;i<3;i++)
	  {		 
	   dest[k++]=MealGroup[index].BRWN[i];
	  }
	  for(i=0;i<4;i++)
	  {
	  dest[k++]=MealGroup[index].MealID[i];
	  }
	  for(i=0;i<6;i++)
	  {
	  dest[k++]=MealGroup[index].DealBalance[i];
	  }
	  dest[k++]=MealGroup[index].MealNO;
	  for(i=0;i<2;i++)
	  {
	   dest[k++]=MealGroup[index].RemainMealNum[i];
	  }
	    
	  for(i=0;i<6;i++)
	  dest[k++]=MealGroup[index].MealPrice[i];
	  dest[k++]=MealGroup[index].PayType;
	  if(MealGroup[index].PayType==0x01)
	  {
	    for(i=0;i<4;i++)
	    dest[k++]=MealGroup[index].Change[i];
	  }
 	  for(i=0;i<6;i++)
	  dest[k++]=MealGroup[index].TimeData[i];
}

unsigned char MealGroupUploadingFun(void)
{
   	unsigned char i = 0 ;
	long  Lenght = 0 ,j=0;
	long  CmdLenght = 0 ;
	 unsigned char 	 Send_Buf[500];
	unsigned char    Buffer[327]={0x00,0x00,0x0a}; 
	/*���ܻ�������0*/
	mem_set_00(rx1Buf,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
	Batch ++ ;
	Batch = 239 ;
	BRWN[3] =    Batch /1000000000000 ;
	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
	BRWN[4] =	 Batch /10000000000 %100 ;
	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
	BRWN[5] =	 Batch /100000000%100;
	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
	BRWN[6] =	 Batch /1000000%100;
	BRWN[6] =    BRWN[6] /10 *16 + BRWN[6]%10 ;
	BRWN[7] =	 Batch /10000 %100 ;
	BRWN[7] =    BRWN[7] /10 *16 + BRWN[7]%10 ;
	BRWN[8] =	 Batch/100 %100;
	BRWN[8] =    BRWN[8] /10 *16 + BRWN[8]%10 ;
	BRWN[9] =	 Batch %100;
	BRWN[9] =    BRWN[9] /10 *16 + BRWN[9]%10 ;

	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
 	Send_Buf[CmdLenght]   =  0xbb ;	   //T
	Send_Buf[CmdLenght+1] =  0x01 ;	   //L
	Send_Buf[CmdLenght+2] =  0x44 ;	   //V
	CmdLenght += 3;

	 StructCopyToBuffer00(&Buffer[3],0);
	 StructCopyToBuffer00(&Buffer[3+0x24],1);
	 StructCopyToBuffer00(&Buffer[3+0x24*2],2);
	 StructCopyToBuffer00(&Buffer[3+0x24*3],3);
	 StructCopyToBuffer00(&Buffer[3+0x24*4],4);
	 StructCopyToBuffer00(&Buffer[3+0x24*5],5);
	 StructCopyToBuffer00(&Buffer[3+0x24*6],6);
	 StructCopyToBuffer00(&Buffer[3+0x24*7],7);
	 StructCopyToBuffer00(&Buffer[3+0x24*8],8);

   printf("%x\r\n",sizeof(MealGroup_struction));
 	 CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/

	 

	 CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],Buffer,sizeof(Buffer));  /*���κ�*/
	 Send_Buf[CmdLenght] = 0x03 	;
	 CmdLenght+=0x03;
	/*
	 ��������û���ϴ��Ĳ�Ʒ����
	*/
	 i = MealComparefunDemo(0x0500,Send_Buf,CmdLenght);
	 if(i == 0x01)
	 return 1 ;
     Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	 for(j=0;j<Lenght+7;j++)
	 {
//	  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	 }

	 return 0 ;

}
  /*******************************************************************************
* Function Name  : ClearingFun		 ������
* Description    : ��ǩ����
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/
unsigned char    ClearingFun(void)
{
  unsigned char i = 0 ;
	long  Lenght = 0 ,j=0;
	unsigned char 	  Send_Buf[400];
	long  CmdLenght = 0 ;	 
	/*���ܻ�������0*/
	mem_set_00(rx1Buf,sizeof(rx1Buf));
	/*ˮ����++*/
	Send_Buf[0] =  0x02 ;
	Send_Buf[1] =  0x00 ;
	Send_Buf[2] =  0x00 ;
	Send_Buf[3] =  0x00 ;
	Send_Buf[4] =  0x00 ;
	CmdLenght = 5 ;
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],TID,sizeof(TID));  /*�ն˵�TID*/
//	BRWN[3] =    Batch /1000000000000 ;
//	BRWN[3] =    BRWN[3] /10 *16 + BRWN[3]%10 ;
//	BRWN[4] =	 Batch /10000000000 %100 ;
//	BRWN[4] =    BRWN[4] /10 *16 + BRWN[4]%10 ;
//	BRWN[5] =	 Batch /100000000%100;
//	BRWN[5] =    BRWN[5] /10 *16 + BRWN[5]%10 ;
//	BRWN[6] =	 Batch /1000000%100;
//	BRWN[6] =    BRWN[6] /10 *16 + BRWN[6]%10 ;
//	BRWN[7] =	 Batch /10000 %100 ;
//	BRWN[7] =    BRWN[7] /10 *16 + BRWN[7]%10 ;
//	BRWN[8] =	 Batch/100 %100;
//	BRWN[8] =    BRWN[8] /10 *16 + BRWN[8]%10 ;
//	BRWN[9] =	 Batch %100;
//	BRWN[9] =    BRWN[9] /10 *16 + BRWN[9]%10 ;

    GetBRWN();

	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BRWN,sizeof(BRWN));  /*��ˮ��*/
	CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],BNO,sizeof(BNO));  /*���κ�*/
  CmdLenght +=mem_copy00(&Send_Buf[CmdLenght],MAC,sizeof(MAC)); /*MAC*/
	Send_Buf[CmdLenght] = 0x03 	;
	CmdLenght+=0x03;
	 i = MealComparefunDemo(0x0500,Send_Buf,CmdLenght);
	 if(i == 0x01)
	 return 1 ;
     Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	 for(j=0;j<Lenght+7;j++)
	 {
//	  printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);
	 }

	 return 0 ;
}

 /*******************************************************************************
* Function Name  : SignInFunction		 
* Description    : ǩ������
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/
 void  SignInFunction(void)
 {
 unsigned char i = 10 ;
    do 
	{
	  if(SignInFun()==0x00)
	  break;
	  delay_ms(500);   
	}while(--i);

	if(i == 0x00)
	return ;
	
	 
 }
 
  /*******************************************************************************
* Function Name  : SignInFunction		 
* Description    : ǩ������
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/
 void  EchoFuntion(void (*fptr)(void))
 {
 unsigned char i = 10 ;
    do 
	{
	  if(EchoFun()==0x00)
	  break;
	  delay_ms(500);   
	}while(--i);

	if(i == 0x00)
	return ;
	(*fptr)() ;
	 
 }
   /*******************************************************************************
* Function Name  : MealGroupUploadingFun		 ������
* Description    : ��Ʒ������
* Input          : void
* Output         : void
* Return         : void	  
*******************************************************************************/
unsigned char MealDataCompareFunction(void)
 {
   unsigned char i = 100;
   while(i)
   {
    if(MealDataCompareFun()==0x00)
	  return 0 ;
	  --i ;
	  delay_ms(100);
   }
    return 1 ;
 }

 
unsigned char 	Resend(unsigned char *p,long lenght)
 {
	 unsigned char Waittimeout = 100 ;
	 long Lenght,j;
	 mem_set_00(rx1Buf,sizeof(rx1Buf));
	 CmdDataSend(p,lenght);	//��������
   /*�ȴ����ݷ���*/
   while(--Waittimeout)
	 {
		   delay_ms(10);
		   if(F_RX1_Right)
		   {
		    F_RX1_Right = 0 ;
		    break;
		   }		   
   }
  if(Waittimeout == 0 )
  return 1 ;
	
	 Lenght = HL_BufferToInit(&rx1Buf[2]) ;
	if(Lenght == 0x00)/*�ж����ݳ���*/
	return 1 ;
	for(j=0;j<Lenght+7;j++)
	{
	 
//	 printf("rx1Buf[%d]=%x\r\n",j,rx1Buf[j]);

	 }
	 if(rx1Buf[0]==0x07 && rx1Buf[1]==0x10)	 //��ʾ��ȷ
	 {
	  	
		if(rx1Buf[7]==0x00)/*�Ƿ�Ӧ������*/
		{
		   return 0 ;
		}
		else
		return 1 ;
//		if(rx1Buf[7]==0x24)/*������ˮ�ţ��ն���ˮ���ظ�*/
//		{	 
//           return 1 ;
//		}
	   
	 }
  return 0 ;

 
  return 0 ;
}
 