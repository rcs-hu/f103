//@filename:RCS_usart.h
//@author:  胡兴国
//@date:    2023-4-28
//@brief:   USART串口通信

#ifndef _RCS_USART_H_
#define _RCS_USART_H_

#include "rcs.h"

void RCS_USART_Config(USART_TypeDef* _USARTx, GPIO_TypeDef* _GPIOx, uint32_t _GPIO_PinX_T,
                      uint32_t _GPIO_PinX_R, uint32_t _baudRate, uint8_t _pri);
void RCS_USART_Send_Char( USART_TypeDef *_USARTx, uint8_t _character);
void RCS_USART_Send_Str(USART_TypeDef *_USARTx, uint8_t _TxBuffer[]);
void RCS_USART_Send_Data(USART_TypeDef *_USARTx, uint8_t *address, uint8_t length);
uint16_t RCS_USART_Accept_Char(USART_TypeDef *_USARTx);
              
//数据拆解为高八位和低八位
//data[0] = (char)((view_data[0] & 0xff00)>>8);
//data[1] = (char)(view_data[0] & 0x00ff);
//__inline void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data)
//{
//	*High_Data = (Current_Data & 0xff00) >> 8;
//	*Low_Data = (Current_Data & 0x00ff);
//}

#endif

