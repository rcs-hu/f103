/*@filename: RCS_PIDctrl.h
 *@author     ���˹�      
 *@brief:     PID�㷨
 *@date: 2023-4-28
*/
#ifndef _RCS_PIDCTRL_H_
#define _RCS_PIDCTRL_H_

#include "rcs.h"		

#define LIMIT_ERR							10000				//��������޷�
#define	LIMIT_SPEED						20000				//��������޷�
#define LIMIT_FEEDBACK_SPEED 	1000				//���귴��������ٶ��޷�
#define LIMIT_ROTATE_SPEED    200				//ƫ���Ƿ���������ٶ��޷�

//����PID�ṹ��
typedef struct {
float P;
float I;
float D;
float Last_Error;			//�ϴ����
float Limit_Output;  	//�������
float Limit_Integral; //����������
float Integral; 			//������,�洢�������	
}PID_Struct;

void PID_Init(void);		//PID��ʼ��

int32_t PID_Motor_Ctrl(float target,float current,PID_Struct *_pid);
float PID_ANGLE_Ctrl(float target,float current,PID_Struct *_pid);
float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid);
float PID_Yaw_Ctrl(float target_angle,float current_angle,float current_angleSpeed,PID_Struct *_pid);


/*����PID�ṹ�嶼��PIDctrl�ж��壬ͨ�������ⲿ�����ķ�ʽ����*/
extern PID_Struct speed_pid;
extern PID_Struct base_speed_pid[4];
extern PID_Struct coord_pid[2];
extern PID_Struct angle_pid;
extern PID_Struct anglespeed_pid;
extern PID_Struct	angle_w_pid;
extern PID_Struct distance_x_pid;
extern PID_Struct distance_y_pid;
extern PID_Struct fiction_speed_pid[2];
extern PID_Struct g_speed_pid;						//ȡ��ץ���ٶȻ�PID
extern PID_Struct holder_z_visual_pid;
extern PID_Struct holder_y_visual_pid;
extern PID_Struct holder_yaw_pid;
extern PID_Struct holder_zAngleSpeed_pid;//��̨ƫ�����ٶ�PID
extern PID_Struct holder_yAngleSpeed_pid;//��̨�������ٶ�PID
extern PID_Struct holder_pitch_pid;			//��̨�����Ƕ�PID
extern PID_Struct holder_zSpeed_pid;			//��̨ƫ���ٶȻ�PID
extern PID_Struct holder_ySpeed_pid;			//��̨�����ٶȻ�PID
extern PID_Struct ball_entry_speed_pid;
#endif
