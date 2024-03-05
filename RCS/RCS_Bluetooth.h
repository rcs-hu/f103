/*@filename: RCS_Bluetooth.h
 *@author     ºúÐË¹ú       
 *@brief:     À¶ÑÀÒ£¿Ø
 *@date: 2023-5-7
*/

#include "rcs.h"	

#ifndef _RCS_BLUETOOTH_H_
#define _RCS_BLUETOOTH_H_

void RCS_Bluetooth_Init();
uint8_t RCS_Key_Send(void);
void RCS_Key_Judge(void);
uint8_t Get_Key0(void);
uint8_t Get_Key1(void);
uint8_t Get_Key2(void);
uint8_t Get_Key3(void);
uint8_t Get_Key4(void);
uint8_t Get_Key5(void);
uint8_t Get_Key6(void);
uint8_t Get_Key7(void);

#endif