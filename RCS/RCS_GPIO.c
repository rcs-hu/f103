//@filename: RCS_GPIO.c
//@date: 2023-4-25
//@author: ���˹�
//@brief: ����GPIO����

#include "RCS_GPIO.h"


/**
	@name: RCS_GPIO_Output_Init
	@brief: �ܽ������ʼ��
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
	@eg:RCS_GPIO_Output_Init(GPIOA,GPIO_Pin_1);
**/
void RCS_GPIO_Output_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	//����GPIOʱ��
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	//�������ģʽ���ٶ�100MHZ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	  //�������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHZ
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/**
	@name: RCS_GPIO_Input_Init
	@brief: �ܽ������ʼ��
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
**/
void RCS_GPIO_Input_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	//����GPIOʱ��
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	//�������ģʽ���ٶ�100MHZ
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	  //��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;	  //��������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHZ
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/**
	@name: RCS_GPIO_Set
	@brief: �ܽ��ø�
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
**/
void RCS_GPIO_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		GPIO_SetBits(GPIOx, GPIO_Pin);
}


/**
	@name: RCS_GPIO_Reset
	@brief: �ܽ��õ�
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
**/
void RCS_GPIO_Reset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}



/**
	@name: RCS_GPIO_Toggle
	@brief: ���ùܽ�
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
**/
void RCS_GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) == Bit_SET)
		RCS_GPIO_Reset(GPIOx, GPIO_Pin);
	else 
		RCS_GPIO_Set(GPIOx, GPIO_Pin);
}


/**
	@name: RCS_GPIO_Read
	@brief: ��ȡ�ܽŵ�ƽ�ߵ�(����ֵ ��1����0)
	@param:GPIO_TypeDef *GPIOx GPIO��
	@param:uint32_t   GPIO_Pin ����ܽź�
**/
uint8_t RCS_GPIO_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}


uint32_t GetRCS_RCC_APB2Periph_GPIO(GPIO_TypeDef *_g)
{

	if (_g == GPIOA)         return RCC_APB2Periph_GPIOA;
	else if (_g == GPIOB)    return RCC_APB2Periph_GPIOB;
	else if (_g == GPIOC)    return RCC_APB2Periph_GPIOC;
	else if (_g == GPIOD)    return RCC_APB2Periph_GPIOD;
	else if (_g == GPIOE)    return RCC_APB2Periph_GPIOE;
	else if (_g == GPIOF)    return RCC_APB2Periph_GPIOF;
	else if (_g == GPIOG)    return RCC_APB2Periph_GPIOG;
	return -1;
}