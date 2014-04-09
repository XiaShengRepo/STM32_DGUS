#include "stm32f4xx.h"										 
#include "bsp.h"

unsigned char CmdNum=0;

void InitBills(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//����ʱ��ʹ�� 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);//���Ӹ�������  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ������1��GPIO   
  	USART_InitStructure.USART_BaudRate = 9600;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����ģʽ
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
  	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ���������
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//˫��ģʽ
	  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
  	USART_Init(UART4, &USART_InitStructure);
  	USART_Cmd(UART4, ENABLE);
  	USART_ClearFlag(UART4, USART_FLAG_TC);//�崫����ɱ�־
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ƕ�����ȼ�����Ϊ1
	  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//Ƕ��ͨ��ΪUSART6_IRQn
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ�� 
	  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Uart4_Bill
* Description    : ����4���ͺ���
* Input          : �����Լ���С
* Output         : void
* Return         : void
*******************************************************************************/
void Uart4Send(uint8_t *p,uint8_t sizeData)
{
  uint8_t i=0;		   
	for(i=0;i<sizeData;i++)
	{
	  printf("p[%d]=%x\r\n",i,p[i]);
    USART_SendData(UART4, p[i]);//����1����һ���ַ�
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
 	}
}

void Uart4_Bill(uint8_t *p,uint8_t sizeData)
{
  uint8_t i;  
	for(i=0; i<sizeData; i++)
	{									
    USART_SendData(UART4, p[i]);//����1����һ���ַ�
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
 	}
}

    /*******************************************************************************
* Function Name  : GetCrc16
* Description    : ����CRCУ��
* Input          : CRC
* Output         : �����Լ���С
* Return         : CRC
*******************************************************************************/
static unsigned int GetCrc16Fun(unsigned char *bufData,unsigned int sizeData)
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
 
 /*******************************************************************************
 * ��������:SetBills                                                                     
 * ��    ��:���ý���10  20 5  ��Ǯ                                                                  
 *           ���ý��յ��ݴ������ڳ����������ѡ���Ǯ����Ǯ                                                                     
 * ��    ��:��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��8��28��                                                                    
 *******************************************************************************/ 
void SetBills(void)
{
    unsigned char bufCmd[5] = {0x34, 0x00, 0x1e, 0x00, 0x00};
	uint8_t i;
	for(i = 0; i < 5; i++)
	{	
	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
	    USART_SendData(UART4, bufCmd[i]);//����1����һ���ַ�	
	}
	CmdNum=1;
}
void DisableBills(void)
{
    unsigned char bufCmd[5] = {0x34, 0x00, 0x00, 0x00, 0x00};
	uint8_t i;
	for(i = 0; i < 5; i++)
	{	
	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);//�ȴ��������
	    USART_SendData(UART4, bufCmd[i]);//����1����һ���ַ�	
	}
    CmdNum=2;	
}

bool StringToHexGroup(unsigned char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
{
  unsigned int i, k=0;
  unsigned char HByte,LByte;
	if(InStrBuffer[strLength-1]!=0x0A)
		return false;
//   if(strLength%2 !=0) //�ж�ĩβ�Ƿ�ΪLF,CR �޾�return
//     return FALSE;
 
  for(i=0; i<strLength; i=i+3)
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
		if(InStrBuffer[i+2]==0x20)
		{			
			 OutHexBuffer[k++]=HByte |LByte;
		}
		if(InStrBuffer[i+2]==0x0D)
		{
			OutHexBuffer[k++]=HByte |LByte;
			return true ;
		}
		
 }
 return true;
}

