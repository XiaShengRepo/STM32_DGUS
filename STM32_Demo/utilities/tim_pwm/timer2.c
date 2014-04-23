 #include "stm32f4xx.h"
#include "timer2.h"



/*******************************************************************************
 * ��������:TIM2_Configuration                                                                    
 * ��    ��:TIM2��ʼ������   5ms�ж�һ��                                                                
 *                                                                               
 * ��    ��:����Ƶ��                                                                     
 * ��    ��:��                                                                     
 * ��    ��:void                                                               
 * �޸�����:2013��4��20��                                                                    
 *******************************************************************************/


void TIM2_Init(void)				   //���ڲɼ�ADC�õĶ�ʱ��
  {

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

  /* GPIOC clock enable */
  
  /* --------------------------NVIC Configuration -------------------------------*/
  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* ------------------- TIM1 Configuration:Output Compare Timing Mode ---------*/
    
   

  /*��Ƶ�����ڼ��㹫ʽ��
  Prescaler = (TIMxCLK / TIMx counter clock) - 1;
  Period = (TIMx counter clock / TIM3 output clock) - 1 
  TIMx counter clockΪ������Ҫ��TXM�Ķ�ʱ��ʱ�� 
  */


  TIM_DeInit(TIM2);

  TIM_TimeBaseStructure.TIM_Period =10000-1  ;	
  TIM_TimeBaseStructure.TIM_ClockDivision = 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;;					  //
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, DISABLE);
//   TIM_Cmd(TIM2, ENABLE);
  }



