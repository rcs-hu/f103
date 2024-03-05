//@filename: RCS_Timer.c
//@date: 2023-4-27
//@author: ���˹�
//@brief: ��ʱ���ж�

#include "RCS_Timer.h"                  // Device header


//@name: Timer_Init
//@brief: ��ʱ���ڲ��ж�
//@param:TIM_TypeDef * _TIM ��ʱ����
//@param:uint32_t _time ����һ���жϵ�ʱ��
//@param:uint8_t _priority ��ǰ4λΪ��ռ���ȼ�����4λΪ��Ӧ���ȼ�
//eg:Timer_Init(TIM2, 1, 0x11);
void Timer_Init(TIM_TypeDef * _TIM, uint32_t _time, uint8_t _priority)
{
	if(_TIM == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
		TIM_InternalClockConfig(TIM2);
		
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
		TIM_TimeBaseInitStructure.TIM_Period = 10000 * _time-1;   //ARR�Զ���װֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   //��TIM2Ϊ����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_priority >> 4) & 0x0f;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _priority & 0x0f;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM2, ENABLE);
	}
	
	if(_TIM == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
		TIM_InternalClockConfig(TIM3);
		
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
		TIM_TimeBaseInitStructure.TIM_Period = 10000 * _time-1;   //ARR�Զ���װֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
		
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_priority >> 4) & 0x0f;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _priority & 0x0f;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM3, ENABLE);
	}
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/


/*
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
*/