//@filename: RCS_GPIO.h
//@date: 2023-4-25
//@author: 胡兴国
//@brief: 常规GPIO配置

#ifndef  _RCS_GPIO_H_
#define  _RCS_GPIO_H_

#include "rcs.h"

void RCS_GPIO_Output_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void RCS_GPIO_Input_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void RCS_GPIO_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void RCS_GPIO_Reset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void RCS_GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t RCS_GPIO_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GetRCS_RCC_APB2Periph_GPIO(GPIO_TypeDef *_g);

#endif
