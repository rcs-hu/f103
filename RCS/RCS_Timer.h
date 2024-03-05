//@filename: RCS_Timer.h
//@date: 2023-4-27
//@author: 胡兴国
//@brief: 定时器内部中断

#ifndef _TIMER_H_
#define _TIMER_H_

#include "rcs.h"

void Timer_Init(TIM_TypeDef * _TIM, uint32_t _time, uint8_t _priority);

#endif
