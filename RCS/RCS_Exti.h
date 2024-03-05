//@filename: RCS_Exti.h
//@date: 2023-4-27
//@author: 胡兴国
//@brief: 外部中断

#ifndef _RCS_Exti_H_
#define _RCS_Exti_H_

#include "rcs.h"  

void Exti_Init(GPIO_TypeDef* GPIOx, uint32_t _pin, EXTITrigger_TypeDef _trigger, uint8_t _priority);
uint8_t GetRCS_GPIO_PortSourceGPIO(GPIO_TypeDef *_g);
uint8_t GetRCS_GPIO_PinSource(uint32_t _p);
uint32_t GetRCS_EXTI_Line(uint32_t _p);
uint16_t GetRCS_EXTI_IRQn(uint32_t _p);

#endif
