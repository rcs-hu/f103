/*@filename: RCS_Ultrasonic.h
 *@author     ºúÐË¹ú       
 *@brief:     ³¬Éù²¨²â¾à
 *@date: 2023-5-6
*/

#include "rcs.h"	

#ifndef _RCS_ULTRASONIC_H_
#define _RCS_ULTRASONIC_H_

void RCS_Ultrasonic_Init();
void Send_Ultrasonic(void);
float Get_Ultr_Distance(void);
	
#endif
