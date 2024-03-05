//@filename: RCS_Encoder.c
//@date: 2023-4-27
//@author: 胡兴国
//@brief: 编码器

#include "RCS_Encoder.h"                  // Device header

//@name:Encoder_Init
//@brief: 编码器外部中断初始化
//@param:GPIO_TypeDef * GPIOx: 端口
//@param:uint32_t _pin: 端口序号
//@param:EXTITrigger_TypeDef _trigger
//      @args:EXTI_Trigger_Rising 上升沿触发
//      @args:EXTI_Trigger_Falling 下降沿触发
//      @args:EXTI_Trigger_Rising_Falling 上升沿下降沿
//@param:uint8_t _priority ：前4位为抢占优先级，后4位为响应优先级
//@note: 清零中断函数  EXTI_ClearITPendingBit(GetRCS_EXTI_Line(_pin));
//eg;Encoder_Init(GPIOA,GPIO_Pin_0,GPIO_Pin_1,EXTI_Trigger_Falling);
void Encoder_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_A, uint16_t GPIO_Pin_B, EXTITrigger_TypeDef _trigger)
{
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_A | GPIO_Pin_B;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GetRCS_GPIO_PortSourceGPIO(GPIOx), GetRCS_GPIO_PinSource(GPIO_Pin_A));
	GPIO_EXTILineConfig(GetRCS_GPIO_PortSourceGPIO(GPIOx), GetRCS_GPIO_PinSource(GPIO_Pin_B));
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = GetRCS_EXTI_Line(GPIO_Pin_A) | GetRCS_EXTI_Line(GPIO_Pin_B);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = _trigger;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_EXTI_IRQn(GPIO_Pin_A);  //某引脚外部中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_EXTI_IRQn(GPIO_Pin_B);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

////GPIO_Pin_10~GPIO_Pin_15中断函数名
//void EXTI15_10_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line12) == SET)
//	{
//		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
//		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
//		{
////			主程序
//		}
//		EXTI_ClearITPendingBit(EXTI_Line12);
//	}
//}

/*
EXTI9_5_IRQHandler //GPIO_Pin_5~GPIO_Pin_9中断函数名
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
*/
int16_t Encoder_Count;
int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}


void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
			{
				Encoder_Count --;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
			{
				Encoder_Count ++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

//@name:Encoder_Timer_Init
//@brief: 编码器定时器计数/测速初始化（TIM3）
void Encoder_Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}

//获取计数值
int16_t Encoder_Timer_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
//	TIM_SetCounter(TIM3, 0);   //计数值清零，测速用
	return Temp;
}