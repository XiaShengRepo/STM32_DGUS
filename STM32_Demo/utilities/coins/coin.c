#include "bsp.h"
#include "stm32f4xx.h"
/*******************************************************************************
 ˵�������ļ�ΪͶ�һ�����ģ��
*******************************************************************************/

/*******************************************************************************
* Function Name  : InitCoins
* Description    :Ӳ�һ���ʼ����Ͷ�ң�;������Դ���ƿ�
* Input          : void
* Output         : void						 
* Return         : void
*******************************************************************************/
void InitCoins(void)
{									 
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ���ṹ��
	  NVIC_InitTypeDef NVIC_InitStructure;//�ж�������ʼ���ṹ��
	  EXTI_InitTypeDef EXTI_InitStructure;//�ⲿ�жϳ�ʼ���ṹ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//����ʱ��ʹ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//���������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = 	GPIO_PuPd_UP ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//Ͷ�һ���Դ���ƽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = 	GPIO_PuPd_DOWN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource12); //�ж���
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : OpenCoinMachine
* Description    : ��Ͷ�һ���Դ
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void OpenCoinMachine(void)		 
{
  GPIO_SetBits(GPIOE, GPIO_Pin_13);
}

/*******************************************************************************
* Function Name  : OpenCoinMachine
* Description    : �ر�Ͷ�һ���Դ
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void CloseCoinMachine(void)	
{
  GPIO_ResetBits(GPIOE, GPIO_Pin_13);
}

/*******************************************************************************
* Function Name  : TIM5_Init
* Description    :��ʱ��5��ʼ��
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
 void TIM5_Init(void)//TIM_PeriodΪ16λ����	   //����1�����ж�һ��
{ 
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//����ʱ��ʹ��  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;					
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ����
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = 19999;//��������װֵ
  	TIM_TimeBaseStructure.TIM_Prescaler = 4199;
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ʽ
  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM5, (4200-1), TIM_PSCReloadMode_Immediate);//����16λʱ�ӷ�Ƶϵ��,��������ģʽ
	  TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* �������жϱ�־ */
  	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//�����ж�
  	TIM_Cmd(TIM5, DISABLE);

}

    /*******************************************************************************
* Function Name  : OpenTIM5
* Description    : �򿪶�ʱ��5
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void OpenTIM5(void)
{
   TIM_Cmd(TIM5, ENABLE);
}

    /*******************************************************************************
* Function Name  : CloseTIM5
* Description    : �رն�ʱ��5
* Input          : void
* Output         : void
* Return         : void
*******************************************************************************/
void CloseTIM5(void)
{
   TIM_Cmd(TIM5, DISABLE);
}
