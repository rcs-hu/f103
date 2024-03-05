//@filename: RCS_Encoder.h
//@date: 2023-4-27
//@author: ºúÐË¹ú
//@brief: ±àÂëÆ÷

#ifndef _RCS_Encoder_H_
#define _RCS_Encoder_H_

#include "rcs.h"  

void Encoder_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_A, uint16_t GPIO_Pin_B, EXTITrigger_TypeDef _trigger);
int16_t Encoder_Get(void);
void Encoder_Timer_Init(void);
int16_t Encoder_Timer_Get(void);
	
#endif
