//@filename:RCS_usart.c
//@author:  胡兴国
//@date:    2023-4-28
//@brief:   USART串口通信

#include "RCS_Usart.h"

//@name: USART_Config
//@brief: 配置USART的串口通信功能
//@param: _USARTx          需要绑定的USART串口
//@param: _GPIOx           需要绑定的GPIO组
//@param: _GPIO_PinX_T     需要绑定的串口发送管脚
//@param:_GPIO_PinX_R      需要绑定的串口接收管脚
//@param:_baudRate         传送的波特率
//@param: _pri             优先级
//@note:清除中断标志位
//USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
//eg:RCS_USART_Config(USART1, GPIOA, GPIO_Pin_9, GPIO_Pin_10, 9600, 0x01);
void RCS_USART_Config(USART_TypeDef* _USARTx, GPIO_TypeDef* _GPIOx, uint32_t _GPIO_PinX_T,
                      uint32_t _GPIO_PinX_R, uint32_t _baudRate, uint8_t _pri)
{
	if(_USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	}
	if(_USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	}
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_GPIO(_GPIOx), ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = _GPIO_PinX_T;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_GPIOx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = _GPIO_PinX_R;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_GPIOx, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = _baudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No;     //无校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //1个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(_USARTx, &USART_InitStructure);
	
	USART_ITConfig(_USARTx, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	if(_USARTx == USART1)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_pri >> 4 ) & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _pri & 0x0f;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(_USARTx, ENABLE);
}

//uint8_t receive_char;
////USART1中断函数
//void USART1_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		receive_char = RCS_USART_Accept_Char(USART1);
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}

//USART2中断函数
void USART2_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
	   USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

//@name: RCS_USART_Send_Char
//@brief: 通过已定义的串口发送一个字符
//@param:_USARTx     已经绑定的USART串口
//@param:_character  要发送的字符
//eg:RCS_USART_Send_Char(USART1,'1'); RCS_USART_Send_Char(USART1,0x01);   
void RCS_USART_Send_Char( USART_TypeDef *_USARTx, uint8_t _character)
{
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	USART_SendData(_USARTx, (uint16_t)_character & 0x00ff);
}


//@name: RCS_USART_Send_Str
//@brief: 通过已定义的串口发送一个字符串
//@param:_USARTx   已经绑定的USART串口
//@param:_TxBuffer 要发送的字符串数组
//eg:RCS_USART_Send_Str(USART1, "abcd");
void RCS_USART_Send_Str(USART_TypeDef *_USARTx, uint8_t _TxBuffer[])
{
	uint8_t i = 0;
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	for (; _TxBuffer[i] != '\0'; i++)
	{
		USART_SendData(_USARTx, (uint16_t)_TxBuffer[i]);
		while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}

//@name: RCS_USART_Send_Data
//@brief: 通过已定义的串口发送一串数据
//@param:_USARTx 已经绑定的USART串口
//@param:address 数据起始地址
//@param:length  数据长度
//eg:RCS_USART_Send_Data(USART1, a, 8);  (a[8])
void RCS_USART_Send_Data(USART_TypeDef *_USARTx, uint8_t *address, uint8_t length)
{
	uint8_t i = 0;
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	for (; i < length ; i++)
	{
		USART_SendData(_USARTx, (uint16_t) * (address + i));
		while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}


//@name: RCS_USART_Accept_Char
//@brief: 通过已定义的串口接收一个字符
//@param:_USARTx 已经绑定的USART串口
//@retval: uint16位的字符
uint16_t RCS_USART_Accept_Char(USART_TypeDef *_USARTx)
{
	return USART_ReceiveData(_USARTx);
}