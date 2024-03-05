/*@filename: RCS_ADC.h
 *@author     胡兴国       
 *@brief:     ADC模数转换器
 *@date: 2023-5-6
*/

#include "rcs.h"	

#ifndef _RCS_ADC_H_
#define _RCS_ADC_H_

void RCS_ADC_Init(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
uint16_t RCS_Get_ADC(GPIO_TypeDef* _GPIOx, uint32_t _GPIO_Pin);
uint8_t GetRCS_ADC_Channel(uint32_t _p);

#endif
