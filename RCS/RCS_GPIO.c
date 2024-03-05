//@filename: RCS_GPIO.c
//@date: 2023-4-25
//@author: 胡兴国
//@brief: 常规GPIO配置

#include "RCS_GPIO.h"


/**
	@name: RCS_GPIO_Output_Init
	@brief: 管脚输出初始化
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
	@eg:RCS_GPIO_Output_Init(GPIOA,GPIO_Pin_1);
**/
void RCS_GPIO_Output_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	//推挽输出模式，速度100MHZ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	  //推挽输出模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHZ
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/**
	@name: RCS_GPIO_Input_Init
	@brief: 管脚输入初始化
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
**/
void RCS_GPIO_Input_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(GPIOx), ENABLE);
	//推挽输出模式，速度100MHZ
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	  //上拉输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;	  //上拉输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHZ
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/**
	@name: RCS_GPIO_Set
	@brief: 管脚置高
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
**/
void RCS_GPIO_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		GPIO_SetBits(GPIOx, GPIO_Pin);
}


/**
	@name: RCS_GPIO_Reset
	@brief: 管脚置低
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
**/
void RCS_GPIO_Reset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}



/**
	@name: RCS_GPIO_Toggle
	@brief: 反置管脚
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
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
	@brief: 读取管脚电平高低(返回值 高1、低0)
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
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