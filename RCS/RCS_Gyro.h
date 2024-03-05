/*@filename: RCS_Gyro.h
 *@author     ºúÐË¹ú       
 *@brief:     ÍÓÂÝÒÇ
 *@date: 2023-4-28
*/
#ifndef _RCS_GYRO_H_
#define _RCS_GYRO_H_

#include "rcs.h"	

void Gyro_Init(void);
float Get_Gyro_Z(void);
float Get_Gyro_X(void);
float Get_Gyro_Y(void);
float Get_Gyro_Z_AngleSpeed(void);
#endif
