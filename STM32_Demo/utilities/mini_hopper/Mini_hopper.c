#include "bsp.h"
#include "stm32f4xx.h"

/*�����˱һ��Ķ˿ڣ����ڴ򿪵�Դ���ر��˱һ�ʹ�乤��*/	  	 
#define COIN_PIN                         GPIO_Pin_0
#define COIN_GPIO_PORT                   GPIOA
#define COIN_GPIO_CLK                    RCC_AHB1Periph_GPIOA  
/*�����˱һ��ж϶˿��źſ�*/
#define COIN_INT_PIN                         GPIO_Pin_1
#define COIN_GPIO_INT_PORT                   GPIOB
#define COIN_GPIO_INT_CLK                    RCC_AHB1Periph_GPIOB

/*
PE11Ϊ���ҿ����ź�
P10Ϊ�Աҵ������
PE9Ϊ�ձ��ź����
PE8Ϊ�Ʊ���������ݲ��ã���
*/
void InitMiniGPIO(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;//�ⲿ�жϳ�ʼ���ṹ��
   
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOE,&GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource10);
	EXTI_InitStructure.EXTI_Line=EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOE,&GPIO_InitStructure);
 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : OpenCoins
* Description    : �����˱һ�
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void OpenCoins(void)	//���Ƽ̵����ṩ220v
{
      GPIO_SetBits(COIN_GPIO_PORT, COIN_PIN);
}
	
/*******************************************************************************
* Function Name  : CloseCoins
* Description    : �ر��˱һ�
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void CloseCoins(void) //���Ƽ̵����Ͽ�220v��ѹ
{										   
    GPIO_ResetBits(COIN_GPIO_PORT, COIN_PIN); 
}

 /*******************************************************************************
* Function Name  : SendOutN_Coin
* Description    : �˱�
* Input          : Ҫ�˵ı���
* Output         : δ�˵ı������޸�
* Return         : 
*******************************************************************************/
uint8_t ErrorType;
u8 SendOutN_Coin(int num)
{
  int i;
  ErrorType = 0;
  for(i=0;i<num;i++)
  {
    if( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9) != 0)
	  {
		  GPIO_ResetBits(GPIOE,GPIO_Pin_11);
		  delay_ms(15);
		  GPIO_SetBits(GPIOE,GPIO_Pin_11);
		  delay_ms(15);
		  GPIO_ResetBits(GPIOE,GPIO_Pin_11);
		  delay_ms(70);
	   }
	   else
	   {
	     ErrorType = 1;
	     return num-i;
	   }
  }
  return 0;
}

    /*******************************************************************************
* Function Name  : StatisticalCoin
* Description    : ͳ��Ӳ�ҵĸ���
* Input          : void
* Output         : uint16_t
* Return         : uint16_t
*******************************************************************************/
uint16_t   CoinNum;
uint8_t    StatisticalCoinFlag ;
uint8_t   StatisticalCoin(void)
{
   static uint8_t CurrentPoint=0;
   switch(CurrentPoint)
   { 
   case 0 :	  
              CoinNum = 0 ;
              //OpenCoinMachine();
			  OpenCoins();
//			  printf("CoinNum = %d\r\n",CoinNum);
//			  printf("nihao = %d\r\n",CoinNum);
			  StatisticalCoinFlag = 1 ;
	          CurrentPoint =1 ;
			  WaitTime = 0 ;
			  OpenTIM5(); 
         	  break;
   case 1 :	//��ʾǮ��
             // printf("nihao = %d\r\n",CoinNum);
         	  DisplayMealPrice1(CoinNum);  
		  	  if(WaitTime > 8)
			  {
			   CloseTIM5();
			   CloseCoins();
			   StatisticalCoinFlag =  0 ;			  
			   CurrentPoint = 2 ;
			   printf("CoinNum = %d\r\n",CoinNum);
			  }
		      break; 
  case 2 :    
              delay_ms(3000);		 //3s
              PictrueDisplay(1);
		      DisplayMealCount();
			  CurrentPoint = 0 ;
			 // Current = 0 ; 
			  return 0 ;    
//		      break;
 
  default :   break;
	}
              return 1 ;
 }

