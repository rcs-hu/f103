//@filename: RCS_PWM.c
//@date: 2023-4-25
//@author: 胡兴国
//@brief: 常规PWM配置

//计算公式
//  TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);TimClk 频率等于PCLK2的频率
//  PrescalerValue = (uuint32_t16_t) (TimClk / _CLKHZ ) - 1; 计数器频率由分频控制
//  ARR = (uint32_t)(_CLKHZ / _PWMHZ); ARR 控制输出频率
//  CCRValue = (uint32_t)(_percent * ARR / 100); CCR 控制占空比

#include "RCS_Pwm.h"
/**
@name: PWMInit,周期必须是20ms
@brief:PWM驱动舵机初始化,仅用于TIM2和TIM3,调节ARR和PSC调节频率
@param:TIM_TypeDef * _TIM : 定时器号
@param:uint8_t _ch  :定时器 pwm输出 通道号
@param:GPIO_TypeDef* GPIOx :pwm输出的GPIO组
@param:uint32_t _pin : pwm输出管脚
@eg:PWMInit(TIM2, 2, GPIOA, GPIO_Pin_1);
**/
void PWMInit(TIM_TypeDef *_TIM, uint8_t _ch, GPIO_TypeDef* GPIOx, uint32_t _pin)
{	
    RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);

    if (_TIM == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			  TIM_InternalClockConfig(TIM2);
    }
		if (_TIM == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			  TIM_InternalClockConfig(TIM3);
    }

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = _pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOx, &GPIO_InitStructure);
		

		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR  驱动舵机
		TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC  驱动舵机
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(_TIM, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCStructInit(&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;		//CCR

    //选择输出通道，不同通道对应不同管脚
    if (_ch == 1)
    {
        TIM_OC1Init(_TIM, &TIM_OCInitStructure);
    }
    if (_ch == 2)
    {
        TIM_OC2Init(_TIM, &TIM_OCInitStructure);
    }
    if (_ch == 3)
    {
        TIM_OC3Init(_TIM, &TIM_OCInitStructure);
    }
    if (_ch == 4)
    {
        TIM_OC4Init(_TIM, &TIM_OCInitStructure);
    }

    TIM_Cmd(_TIM, ENABLE);
}


/**
	@name: PWMOutput
	@brief:PWM驱动舵机
	@param:TIM_TypeDef * _TIM 定时器号
	@param:uint8_t _ch,定时器的PWM的输出通道号
  @param:double _percent ,舵机角度，范围0~180°
  @eg:PWMOutput(TIM2, 1, 90);
**/
void PWMOutput(TIM_TypeDef *_TIM, uint8_t _ch, float _percent)
{
		if (_ch == 1)
		{
			TIM_SetCompare1(_TIM, (uint16_t)(_percent / 180 * 2000 + 500));
		}
		if (_ch == 2)
		{
			TIM_SetCompare2(_TIM, (uint16_t)(_percent / 180 * 2000 + 500));
		}
		if (_ch == 3)
		{
			TIM_SetCompare3(_TIM, (uint16_t)(_percent / 180 * 2000 + 500));
		}
		if (_ch == 4)
		{
			TIM_SetCompare4(_TIM, (uint16_t)(_percent / 180 * 2000 + 500));
		}
}

/**
	@name: PWM_Motor_Init
	@brief:PWM驱动单相电机初始化
	@param:TIM_TypeDef * _TIM : 定时器号
	@param:uint8_t _ch  :定时器 pwm输出 通道号
	@param:GPIO_TypeDef* GPIOx :pwm输出的GPIO组
	@param:uint32_t _pin : pwm输出管脚
**/
void PWM_Motor_Init(TIM_TypeDef *_TIM, uint8_t _ch, GPIO_TypeDef* GPIOx, uint32_t _pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);

	if (_TIM == TIM2)
	{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			TIM_InternalClockConfig(TIM2);
	}
	if (_TIM == TIM3)
	{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			TIM_InternalClockConfig(TIM3);
	}

//	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR  驱动电机，消除噪音
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC  驱动电机
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(_TIM, &TIM_TimeBaseInitStructure);


	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR

	//选择输出通道，不同通道对应不同管脚
	if (_ch == 1)
	{
			TIM_OC1Init(_TIM, &TIM_OCInitStructure);
	}
	if (_ch == 2)
	{
			TIM_OC2Init(_TIM, &TIM_OCInitStructure);
	}
	if (_ch == 3)
	{
			TIM_OC3Init(_TIM, &TIM_OCInitStructure);
	}
	if (_ch == 4)
	{
			TIM_OC4Init(_TIM, &TIM_OCInitStructure);
	}

	TIM_Cmd(_TIM, ENABLE);
}

/**
	@name: PWM_Motor_send
	@brief:PWM驱动单相电机
	@param:TIM_TypeDef * _TIM 定时器号
	@param:uint8_t _ch,定时器的PWM的输出通道号
  @param:int8_t Speed 	电机转速，范围是-100~100
**/
void PWM_Motor_Send(TIM_TypeDef *_TIM, uint8_t _ch,int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		if (_ch == 1)
		{
				TIM_SetCompare1(_TIM, (uint16_t)(Speed));
		}
		if (_ch == 2)
		{
				TIM_SetCompare2(_TIM, (uint16_t)(Speed));
		}
		if (_ch == 3)
		{
				TIM_SetCompare3(_TIM, (uint16_t)(Speed));
		}
		if (_ch == 4)
		{
				TIM_SetCompare4(_TIM, (uint16_t)(Speed));
		}
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		if (_ch == 1)
		{
				TIM_SetCompare1(_TIM, (uint16_t)(-Speed));
		}
		if (_ch == 2)
		{
				TIM_SetCompare2(_TIM, (uint16_t)(-Speed));
		}
		if (_ch == 3)
		{
				TIM_SetCompare3(_TIM, (uint16_t)(-Speed));
		}
		if (_ch == 4)
		{
				TIM_SetCompare4(_TIM, (uint16_t)(-Speed));
		}
	}
}


uint32_t GetRCS_RCC_APB1Periph_TIM(TIM_TypeDef *_t)
{
	if ( TIM2  == _t) return RCC_APB1Periph_TIM2;
	else if ( TIM3  == _t) return RCC_APB1Periph_TIM3;
	else if ( TIM4  == _t) return RCC_APB1Periph_TIM4;
	else if ( TIM5  == _t) return RCC_APB1Periph_TIM5;
	else if ( TIM6  == _t) return RCC_APB1Periph_TIM6;
	else if ( TIM7  == _t) return RCC_APB1Periph_TIM7;
	else if ( TIM12 == _t) return RCC_APB1Periph_TIM12;
	else if ( TIM13 == _t) return RCC_APB1Periph_TIM13;
	else if ( TIM14 == _t) return RCC_APB1Periph_TIM14;
	return -1;
}

