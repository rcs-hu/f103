/*@filename: RCS_ADC.c
 *@author     胡兴国       
 *@brief:     ADC模数转换器
 *@date: 2023-5-6
*/

#include "RCS_ADC.h" 

/**
	@name: RCS_ADC_Init
	@brief: 模数转换初始化,使用ADC1
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
	@eg:RCS_ADC_Init(GPIOA, GPIO_Pin_0);
**/
void RCS_ADC_Init(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	ADC_RegularChannelConfig(ADC1, GetRCS_ADC_Channel(GPIO_Pin), 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}


/**
	@name: RCS_Get_ADC
	@brief: 获取通道模拟值
	@param:GPIO_TypeDef *GPIOx GPIO组
	@param:uint32_t   GPIO_Pin 具体管脚号
	@eg:RCS_Get_ADC(GPIOA, GPIO_Pin_0);
**/
uint16_t RCS_Get_ADC(GPIO_TypeDef* _GPIOx, uint32_t _GPIO_Pin)
{
	ADC_RegularChannelConfig(ADC1, GetRCS_ADC_Channel(_GPIO_Pin), 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}


uint8_t GetRCS_ADC_Channel(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return ADC_Channel_0;
	case GPIO_Pin_1: return ADC_Channel_1;
	case GPIO_Pin_2: return ADC_Channel_2;
	case GPIO_Pin_3: return ADC_Channel_3;
	case GPIO_Pin_4: return ADC_Channel_4;
	case GPIO_Pin_5: return ADC_Channel_5;
	case GPIO_Pin_6: return ADC_Channel_6;
	case GPIO_Pin_7: return ADC_Channel_7;
	}
}