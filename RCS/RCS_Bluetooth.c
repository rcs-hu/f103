/*@filename: RCS_Bluetooth.c
 *@author     胡兴国       
 *@brief:     蓝牙遥控
 *@date: 2023-5-7
*/

#include "RCS_Bluetooth.h" 

/***************全局变量*****************/
static uint8_t receive_char;
static uint8_t key[8] = {0};
/****************************************/

/**
	@name: RCS_Bluetooth_Init
	@brief: 蓝牙遥控初始化
**/
void RCS_Bluetooth_Init()
{
	RCS_GPIO_Input_Init(GPIOA, GPIO_Pin_2);
	RCS_GPIO_Input_Init(GPIOA, GPIO_Pin_6);
	RCS_USART_Config(USART1, GPIOA, GPIO_Pin_9, GPIO_Pin_10, 9600, 0x01);
}

/**
	@name: RCS_Key_Send
	@brief: 返回按键总和
**/
uint8_t RCS_Key_Send(void)
{
	uint8_t key_sum = 0;
	if(RCS_GPIO_Read(GPIOA, GPIO_Pin_6))
	{
		key_sum += 0x01; 
	}
	else
	{
		key_sum += 0x00; 
	}
	
	if(RCS_GPIO_Read(GPIOA, GPIO_Pin_2))
	{
		key_sum += 0x02; 
	}
	else
	{
		key_sum += 0x00; 
	}
	
	return key_sum;
}

/**
	@name: RCS_Key_Judge
	@brief: 判断按键
**/
void RCS_Key_Judge(void)
{		
	if((receive_char&0x01) == 0x01)
	{
		key[0] = 1;
	}
	else
	{
		key[0] = 0;
	}
	
	if((receive_char&0x02) == 0x02)
	{
		key[1] = 1;
	}
	else
	{
		key[1] = 0;
	}
	
	if((receive_char&0x04) == 0x04)
	{
		key[2] = 1;
	}
	else
	{
		key[2] = 0;
	}
	
	if((receive_char&0x08) == 0x08)
	{
		key[3] = 1;
	}
	else
	{
		key[3] = 0;
	}
	
	if((receive_char&0x10) == 0x10)
	{
		key[4] = 1;
	}
	else
	{
		key[4] = 0;
	}
	
	if((receive_char&0x20) == 0x20)
	{
		key[5] = 1;
	}
	else
	{
		key[5] = 0;
	}
	
	if((receive_char&0x40) == 0x40)
	{
		key[6] = 1;
	}
	else
	{
		key[6] = 0;
	}
	
	if((receive_char&0x80) == 0x80)
	{
		key[7] = 1;
	}
	else
	{
		key[7] = 0;
	}
}

//获取按键状态
uint8_t Get_Key0(void)
{
	return key[0];
}

uint8_t Get_Key1(void)
{
	return key[1];
}

uint8_t Get_Key2(void)
{
	return key[2];
}

uint8_t Get_Key3(void)
{
	return key[3];
}

uint8_t Get_Key4(void)
{
	return key[4];
}

uint8_t Get_Key5(void)
{
	return key[5];
}

uint8_t Get_Key6(void)
{
	return key[6];
}

uint8_t Get_Key7(void)
{
	return key[7];
}

//USART1中断函数
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		receive_char = RCS_USART_Accept_Char(USART1);
		RCS_Key_Judge();
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}