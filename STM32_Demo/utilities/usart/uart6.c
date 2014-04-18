#include "stm32f4xx.h"
#include "protocol.h"
#include "uart6.h"
#include "string.h"

uint8_t Temperature=0;

void Uart6_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  
  /* Connect USART pins to AF7 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    /* Enable USART */
  USART_Cmd(USART6, ENABLE);
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
  USART_ClearFlag(USART6, USART_FLAG_TC);     // ���־
}

 /*******************************************************************************
* Function Name	: UART6_IRQHandler
* Description	: ���е��ͨѸ��������
* Input			: ��
* Output 		: ��
* Return 		: void
*�޸�ʱ�䣺2014.2.14
*�޸���	�� yao
*******************************************************************************/
uint8_t Usart6Buff[100];
uint8_t Usart6Index;

uint8_t Usart6DataFlag =0;  //�˱�־��Ϊ�Ƿ������ݽ��գ�0�ޣ�1�����ݣ�ƽʱ��0
uint8_t TemperSign ;

void ManipulatorHandler(void)
{
	char temp=0,ack=0,i=0;
	while(UART6_GetCharsInRxBuf()>=6) //��ȡ��������С��ֱ�������������� ���ĸ���������
	{
		while(USART6_GetChar(&temp)==1)
		{
			Usart6Buff[i]= temp ;//ȡ��һ������
			if((temp=='\r'))
			{
				if(USART6_GetChar(&temp)==1)
				{
					if(temp=='\n')
					{
						i=0;
						break;
					}						
				}
			}
			else i++;
		}
     switch (Usart6Buff[0])
	   {
				case 0x06 :  //ACK
				{
					machinerec.reack =1;
					machinerec.renack = 0;
				}break;
				case 0x15 :  //NACK
				{
					//����ط�����ԭ����
					machinerec.reack =0;
					machinerec.renack = 1;
				}break;
				case 'E':  //�����־
				{
					SendAck();
					if( Usart6Buff[1] == 1)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E101 = true;
							}
							else if( Usart6Buff[3] == 2 )
							{
								ErFlag.E102 = true;
							}
							else if( Usart6Buff[3] == 3 )
							{
								ErFlag.E103 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==2)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E201 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==3)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E301 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==4)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E401 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==5)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E501 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==6)
					{
						if( Usart6Buff[2] == 0 )
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E601 = true;
							}
						}
					}
					else if( Usart6Buff[1] ==7)
					{
						if(Usart6Buff[2] == 1 ) 
						{
							if( Usart6Buff[3] == 1 )
							{
								ErFlag.E711 = true;
							}
						}
					}
					else
					{
						SendNack();
					}
				//mem_set_00(Usart6Buff,6);
				memset(Usart6Buff,0,6);
				Usart6Index =0;
			}break;
		
		    case 'D': //��״̬
				{
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0'))
							{
								//����
								machinerec.redoor = 0;
							}
							else  if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '1'))
							{
								//����
								machinerec.redoor = 1;
							}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;  
					}
				}break;
		    case 'N':
				{				
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== 'O')&&(Usart6Buff[2]== 'R')&&(Usart6Buff[3]== '0'))
								{
									//�������ԭ�㣬������λ��
									machinerec.rerelative = 1;
								}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;
					}
				}break;
		    case 'S':
				{				
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0'))
							{
								//����ȡ�͵�
								machinerec.regoal = 1;
							}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;  
					}
				}break;  	
		    case 'O':
				{				
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== 'U')&&(Usart6Buff[2]== 'T')&&(Usart6Buff[3]== '0'))
							{
								//���ѵ�����Ϳ�
								machinerec.retodoor = 1;
							}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;	  
					}
				}break;
		    case 'F':
				{				
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== 'I')&&(Usart6Buff[2]== 'N')&&(Usart6Buff[3]== '0'))
							{
								//���ѱ�ȡ��
								machinerec.remealaway = 1;
							}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;
					}
				}break;  
		    case 'T': //����ȡ�Ϳ�δ��ȡ�ߣ���ͼ��ϵĲ�ȡ����
				{
					if(Usart6Index >= 6)
					{
						if((Usart6Buff[4]==0x0d) &&(Usart6Buff[5] == 0x0a))
						{
							SendAck();
							if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
							{
								//����ȡ�Ϳڹ���20�뻹δ��ȡ��
								machinerec.remealnoaway = 1;
							}
							else if((Usart6Buff[1]== '0')&&(Usart6Buff[2]== '0')&&(Usart6Buff[3]== '0')&&(Usart6Buff[4]== 0x0d)&&(Usart6Buff[5]== 0x0a))
							{
								//ȡ��5���˻�δȡ����
								machinerec.reenablegetmeal = 1;
							}
						}
						else
						{
							SendNack();
						}
						memset(Usart6Buff,0,6);
						Usart6Index =0;
					}
				}break;
		    case 'A': //��е�ַ��ص��¶����ݣ���ʽ ��A��+-���¶ȸ�λ���¶ȵ�λ��CR��LF
				{
					SendAck();
					if(Usart6Buff[1] == '+')
					{
						TemperSign =0;
					}
					else if( Usart6Buff[1] == '-' )
					{
						TemperSign =1;
					}
					Temperature =  (Usart6Buff[2] - 48)*10 + (Usart6Buff[3] - 48);
				}break;
		    default:
			  {				
		      memset(Usart6Buff,0,100);	
		    }break;
		  }		
	 } 
}


/************************************************************************/
void Uart6_Send(const uint8_t *p,uint8_t length)
{
   uint8_t i=0;
	 for(i=0;i<length;i++)
	 {	  
	    USART6->DR = (u8) p[i];       
    	while((USART6->SR&0X40)==0);//ѭ������,ֱ���������   
    	    
	}
}
void Uart6_Sent(const char *p,uint8_t length)
{
   uint8_t i=0;
	 for(i=0;i<length;i++)
	 {	  
	    USART6->DR = (u8) p[i];       
    	while((USART6->SR&0X40)==0);//ѭ������,ֱ���������   
    	    
	}
}

#define RX6BUF_SIZE 200				 /*����FIFO���������*/  
unsigned char RX6_BUFF[RX6BUF_SIZE];	 /*����FIFO����������*/
unsigned char UART6_InpLen =0;        /*����FIFO�ڴ���ȡ���ֽ���*/
unsigned int  RX6_IndexR =0;          /*����FIFO�Ķ�ָ��*/
unsigned int  RX6_IndexW =0;          /*����FIFO�Ķ�ָ��*/
 /*******************************************************************************
 * ��������: USART3_GetChar()
 * ��    ��: �Ӵ��ڶ�ȡһ���ֽ����ݣ��ӻ�����ж�ȡ1�ֽ��ѽ��ܵ����ݣ�
 *
 * ��    ��: *chr����ȡ��������ŵĵ�ַָ��
 * ��    ��: ����1��ʾ��ȡ�ɹ�������0��ʾ��ȡʧ��
 * ��    ��: char
 * �޸�����: 2014��3��13��   
*******************************************************************************/
char USART6_GetChar(unsigned char *Chr)
{
	if(UART6_InpLen==0)return(0);    //���FIFO�������ݣ�����0
	//_DINT();											//�漰FIFO����ʱ�������жϣ�����ָ�����
	UART6_InpLen--;                  //����ȡ�����ֽ�����1
	*Chr =RX6_BUFF[RX6_IndexR];			//����ȡָ���ȡһ���ֽ���Ϊ����ֵ
	if(++RX6_IndexR>=RX6BUF_SIZE)    //��ȡָ����������ж��Ƿ��±�Խ��
	{
		RX6_IndexR =0;                 //���Խ����дָ���0(ѭ������)
	}
	//_EINT();                      //FIFO������ϣ��ָ��ж�����
	return(1); 											//���سɹ���־	
}
 /*******************************************************************************
 * ��������: USART3_GetCharInRxBuf()
 * ��    ��: ��ȡFIFO���ѽ��ܵ������ֽ���
 *
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: FIFO�����ݵ��ֽ���
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
unsigned int UART6_GetCharsInRxBuf(void)
{
	return(UART6_InpLen);           //����FIFO�����ݵ��ֽ���
}

 /*******************************************************************************
 * ��������: UART0_ClrRxBuf()
 * ��    ��: �������FIFO��
 *
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: ��
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
void UART6_ClrRxBuf(void)
{
	//_DINT();
	UART6_InpLen =0;
	RX6_IndexW =0;
	//_EINT();

}



 /*******************************************************************************
 * ��������: USART3_IRQHandler
 * ��    ��: �жϴ���������ѹ�뻺��
 *           �򵥵�����֡����
 * ��    ��: ��
 * ��    ��: ��
 * ��    ��: ��
 * �޸�����: 2014��3��13�� 
*******************************************************************************/
void USART6_IRQHandler(void)
{	
	if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)//���ݽ���ɨ��
	{		
	  UART6_InpLen++;
	  RX6_BUFF[RX6_IndexW]=USART_ReceiveData(USART6);
	  if(++RX6_IndexW >=RX6BUF_SIZE)
	{
		RX6_IndexW =0;
	}
	}
}
