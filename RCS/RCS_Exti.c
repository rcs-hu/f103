//@filename: RCS_Exti.c
//@date: 2023-4-27
//@author: 胡兴国
//@brief: 外部中断

#include "RCS_Exti.h"                  // Device header


//@name:Exti_Init
//@brief:   初始化外部中断功能
//@param:GPIO_TypeDef *  _port: 端口
//@param:uint32_t _pin: 端口序号
//@param:EXTITrigger_TypeDef _trigger
//      @args:EXTI_Trigger_Rising 上升沿触发
//      @args:EXTI_Trigger_Falling 下降沿触发
//      @args:EXTI_Trigger_Rising_Falling 上升沿下降沿
//@param:uint8_t _priority ：前4位为抢占优先级，后4位为响应优先级
//@note: 清零中断函数  EXTI_ClearITPendingBit(GetRCS_EXTI_Line(_pin));
//eg;Exti_Init(GPIOA,GPIO_Pin_12,EXTI_Trigger_Falling,0x11);
void Exti_Init(GPIO_TypeDef* GPIOx, uint32_t _pin, EXTITrigger_TypeDef _trigger, uint8_t _priority)
{
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GetRCS_GPIO_PortSourceGPIO(GPIOx), GetRCS_GPIO_PinSource(_pin));
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = GetRCS_EXTI_Line(_pin);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = _trigger;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_EXTI_IRQn(_pin);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_priority >> 4) & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _priority & 0x0f;
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
uint8_t GetRCS_GPIO_PortSourceGPIO(GPIO_TypeDef *_g)
{

	if (_g == GPIOA)         return GPIO_PortSourceGPIOA;
	else if (_g == GPIOB)    return GPIO_PortSourceGPIOB;
	else if (_g == GPIOC)    return GPIO_PortSourceGPIOC;
	else if (_g == GPIOD)    return GPIO_PortSourceGPIOD;
	else if (_g == GPIOE)    return GPIO_PortSourceGPIOE;
	else if (_g == GPIOF)    return GPIO_PortSourceGPIOF;
	else if (_g == GPIOG)    return GPIO_PortSourceGPIOG;
	return -1;
}


uint8_t GetRCS_GPIO_PinSource(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return GPIO_PinSource0;
	case GPIO_Pin_1: return GPIO_PinSource1;
	case GPIO_Pin_2: return GPIO_PinSource2;
	case GPIO_Pin_3: return GPIO_PinSource3;
	case GPIO_Pin_4: return GPIO_PinSource4;
	case GPIO_Pin_5: return GPIO_PinSource5;
	case GPIO_Pin_6: return GPIO_PinSource6;
	case GPIO_Pin_7: return GPIO_PinSource7;
	case GPIO_Pin_8: return GPIO_PinSource8;
	case GPIO_Pin_9: return GPIO_PinSource9;
	case GPIO_Pin_10: return GPIO_PinSource10;
	case GPIO_Pin_11: return GPIO_PinSource11;
	case GPIO_Pin_12: return GPIO_PinSource12;
	case GPIO_Pin_13: return GPIO_PinSource13;
	case GPIO_Pin_14: return GPIO_PinSource14;
	case GPIO_Pin_15: return GPIO_PinSource15;
	}
	return -1;
}


uint32_t GetRCS_EXTI_Line(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return EXTI_Line0;
	case GPIO_Pin_1: return EXTI_Line1;
	case GPIO_Pin_2: return EXTI_Line2;
	case GPIO_Pin_3: return EXTI_Line3;
	case GPIO_Pin_4: return EXTI_Line4;
	case GPIO_Pin_5: return EXTI_Line5;
	case GPIO_Pin_6: return EXTI_Line6;
	case GPIO_Pin_7: return EXTI_Line7;
	case GPIO_Pin_8: return EXTI_Line8;
	case GPIO_Pin_9: return EXTI_Line9;
	case GPIO_Pin_10: return EXTI_Line10;
	case GPIO_Pin_11: return EXTI_Line11;
	case GPIO_Pin_12: return EXTI_Line12;
	case GPIO_Pin_13: return EXTI_Line13;
	case GPIO_Pin_14: return EXTI_Line14;
	case GPIO_Pin_15: return EXTI_Line15;
	}
	return -1;
}

uint16_t GetRCS_EXTI_IRQn(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return EXTI0_IRQn;
	case GPIO_Pin_1: return EXTI1_IRQn;
	case GPIO_Pin_2: return EXTI2_IRQn;
	case GPIO_Pin_3: return EXTI3_IRQn;
	case GPIO_Pin_4: return EXTI4_IRQn;
	case GPIO_Pin_5: return EXTI9_5_IRQn;
	case GPIO_Pin_6: return EXTI9_5_IRQn;
	case GPIO_Pin_7: return EXTI9_5_IRQn;
	case GPIO_Pin_8: return EXTI9_5_IRQn;
	case GPIO_Pin_9: return EXTI9_5_IRQn;
	case GPIO_Pin_10: return EXTI15_10_IRQn;
	case GPIO_Pin_11: return EXTI15_10_IRQn;
	case GPIO_Pin_12: return EXTI15_10_IRQn;
	case GPIO_Pin_13: return EXTI15_10_IRQn;
	case GPIO_Pin_14: return EXTI15_10_IRQn;
	case GPIO_Pin_15: return EXTI15_10_IRQn;
	}
	return -1;
}
