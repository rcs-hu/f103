//@filename: RCS_Timer.c
//@date: 2023-4-27
//@author: 胡兴国
//@brief: 定时器中断

#include "RCS_Timer.h"                  // Device header


//@name: Timer_Init
//@brief: 定时器内部中断
//@param:TIM_TypeDef * _TIM 定时器号
//@param:uint32_t _time 进入一次中断的时间
//@param:uint8_t _priority ：前4位为抢占优先级，后4位为响应优先级
//eg:Timer_Init(TIM2, 1, 0x11);
void Timer_Init(TIM_TypeDef * _TIM, uint32_t _time, uint8_t _priority)
{
	if(_TIM == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
		TIM_InternalClockConfig(TIM2);
		
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
		TIM_TimeBaseInitStructure.TIM_Period = 10000 * _time-1;   //ARR自动重装值
		TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
		
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   //以TIM2为主体
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
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
		TIM_TimeBaseInitStructure.TIM_Period = 10000 * _time-1;   //ARR自动重装值
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