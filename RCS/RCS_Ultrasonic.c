/*@filename: RCS_Ultrasonic.c
 *@author     胡兴国       
 *@brief:     超声波测距
 *@date: 2023-5-6
*/

#include "RCS_Ultrasonic.h" 

/***************全局变量*****************/
static float flytime;
static float distance;
/****************************************/

/**
	@name: RCS_Ultrasonic_Init
	@brief: 超声波测距
	@eg:RCS_ADC_Init();
**/
void RCS_Ultrasonic_Init()
{
	RCS_GPIO_Output_Init(ULTR_GPIO, ULTR_TRIG);  //初始化Trig输出引脚
	GPIO_ResetBits(ULTR_GPIO, ULTR_TRIG);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	if(ULTR_TIM==TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}
	if(ULTR_TIM==TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	if(ULTR_TIM==TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(ULTR_GPIO), ENABLE);
	//初始化Echo输入引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;	  //下拉输入模式
	GPIO_InitStruct.GPIO_Pin = ULTR_ECHO;						//引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHZ
	GPIO_Init(ULTR_GPIO, &GPIO_InitStruct);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = GetRCS_EXTI_Line(ULTR_ECHO);
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_EXTI_IRQn(ULTR_ECHO);
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Period = 50000 - 1;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(ULTR_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_Cmd(ULTR_TIM,ENABLE); //使能定时器
}

//发出一次超声波
void Send_Ultrasonic(void)
{
	GPIO_SetBits(ULTR_GPIO,ULTR_TRIG);
	delay_us(20);
	GPIO_ResetBits(ULTR_GPIO,ULTR_TRIG);
	delay_ms(5);
}

//返回超声波探测距离
float Get_Ultr_Distance(void)
{
	Send_Ultrasonic();
	distance = flytime;
	return (float)distance/2.0*340*100/10000;
}

//中断获取超声波距离
void EXTI15_10_IRQHandler(void)
{
	//中断获取超声波距离
	if(EXTI_GetITStatus(GetRCS_EXTI_Line(ULTR_ECHO)))
	{
		if(GPIO_ReadInputDataBit(ULTR_GPIO,ULTR_ECHO))
		{
			ULTR_TIM->CNT = 0;
			TIM_Cmd(ULTR_TIM,ENABLE);
		}
		else
		{
			TIM_Cmd(ULTR_TIM,DISABLE);
			flytime = ULTR_TIM->CNT;
			ULTR_TIM->CNT = 0;
		}
		EXTI_ClearITPendingBit(GetRCS_EXTI_Line(ULTR_ECHO));
	}
}