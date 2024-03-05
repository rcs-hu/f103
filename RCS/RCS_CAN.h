/*@filename: RCS_CAN.h
 *@author     胡兴国       
 *@brief:     can通讯
 *@date: 2023-5-2
*/

#ifndef _RCS_CAN_H_
#define _RCS_CAN_H_

#include "rcs.h"

void Motor_Init(void);
void RCS_CAN1_Init(void);
void Motor_Send(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current);
void RCS_CANx_Send(CAN_TypeDef *CANx,uint32_t Id, uint8_t Length, uint8_t* sendData);
float Get_M3508_Angle(uint8_t ID);
float Get_Motor_Float_Angle(uint8_t Motor_ID);
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID);
int16_t Get_Motor_Speed(uint8_t Motor_ID);

void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data);
//数据拆解为高八位和低八位
__inline void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data)
{
	*High_Data = (Current_Data & 0xff00) >> 8;
	*Low_Data = (Current_Data & 0x00ff);
}



#endif

