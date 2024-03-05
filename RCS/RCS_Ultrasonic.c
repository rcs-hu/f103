/*@filename: RCS_Ultrasonic.c
 *@author     ���˹�       
 *@brief:     ���������
 *@date: 2023-5-6
*/

#include "RCS_Ultrasonic.h" 

/***************ȫ�ֱ���*****************/
static float flytime;
static float distance;
/****************************************/

/**
	@name: RCS_Ultrasonic_Init
	@brief: ���������
	@eg:RCS_ADC_Init();
**/
void RCS_Ultrasonic_Init()
{
	RCS_GPIO_Output_Init(ULTR_GPIO, ULTR_TRIG);  //��ʼ��Trig�������
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
	//����GPIOʱ��
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(ULTR_GPIO), ENABLE);
	//��ʼ��Echo��������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;	  //��������ģʽ
	GPIO_InitStruct.GPIO_Pin = ULTR_ECHO;						//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHZ
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
	TIM_TimeBaseInitStructure.TIM_Period = 50000 - 1;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(ULTR_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_Cmd(ULTR_TIM,ENABLE); //ʹ�ܶ�ʱ��
}

//����һ�γ�����
void Send_Ultrasonic(void)
{
	GPIO_SetBits(ULTR_GPIO,ULTR_TRIG);
	delay_us(20);
	GPIO_ResetBits(ULTR_GPIO,ULTR_TRIG);
	delay_ms(5);
}

//���س�����̽�����
float Get_Ultr_Distance(void)
{
	Send_Ultrasonic();
	distance = flytime;
	return (float)distance/2.0*340*100/10000;
}

//�жϻ�ȡ����������
void EXTI15_10_IRQHandler(void)
{
	//�жϻ�ȡ����������
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