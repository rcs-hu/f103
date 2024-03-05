/*@filename:  RCS_BaseMove.h
 *@author     ���˹�     
 *@brief:     �����㷨
 *@date:      2023-5-29
*/
#ifndef _RCS_BASEMOVE_H_
#define _RCS_BASEMOVE_H_

#include "rcs.h"		
/***********���̹��(mm)************/
#define BASE_LENGTH     350.0f  //���̰볤
#define BASE_WIDTH			350.0f	//���
#define RAD_WHEEL				76.2f	//���Ӱ뾶
#define SLOWDOWN_RATE	  19.0f	  //������ٱ�
#define SLOWDOWN_RATE_2006  36.0f //2006������ٱ�
#define MM_S2R_MIN			0.1772276212907689f	// ������ٶ�(mm/s)ת��������ת��(r/min)�����㷽����60/(2*RAD_WHEEL*pi*cos(45��))
#define MAX_MOTOR_SPEED 8000.0f	//������ת��
#define L_ANGLE             0.5                 //���������Ƕ�
#define R_ANGLE             0.5                 //���������Ƕ�
#define BIG_ANGLE           5.0                 //��Ƕ�

void M_BaseMove_Init(void);			//���̳�ʼ��
void BaseMove_P2P(float move_speed_x,float move_speed_y,float rotate_speed);										
void BaseMove_Polar(float direction_angle,float run_speed,float rotate_speed);
void Point2Point_ER(float target_x,float target_y,float target_z,float run_speed,int *ER_flag);
void Joystick_Ctrl(void);
												
#endif
