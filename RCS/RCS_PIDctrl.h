/*@filename: RCS_PIDctrl.h
 *@author     胡兴国      
 *@brief:     PID算法
 *@date: 2023-4-28
*/
#ifndef _RCS_PIDCTRL_H_
#define _RCS_PIDCTRL_H_

#include "rcs.h"		

#define LIMIT_ERR							10000				//积分误差限幅
#define	LIMIT_SPEED						20000				//输出电流限幅
#define LIMIT_FEEDBACK_SPEED 	1000				//坐标反馈的输出速度限幅
#define LIMIT_ROTATE_SPEED    200				//偏航角反馈的输出速度限幅

//定义PID结构体
typedef struct {
float P;
float I;
float D;
float Last_Error;			//上次误差
float Limit_Output;  	//输出上限
float Limit_Integral; //积分项上限
float Integral; 			//积分项,存储积分误差	
}PID_Struct;

void PID_Init(void);		//PID初始化

int32_t PID_Motor_Ctrl(float target,float current,PID_Struct *_pid);
float PID_ANGLE_Ctrl(float target,float current,PID_Struct *_pid);
float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid);
float PID_Yaw_Ctrl(float target_angle,float current_angle,float current_angleSpeed,PID_Struct *_pid);


/*所有PID结构体都在PIDctrl中定义，通过申明外部变量的方式共享*/
extern PID_Struct speed_pid;
extern PID_Struct base_speed_pid[4];
extern PID_Struct coord_pid[2];
extern PID_Struct angle_pid;
extern PID_Struct anglespeed_pid;
extern PID_Struct	angle_w_pid;
extern PID_Struct distance_x_pid;
extern PID_Struct distance_y_pid;
extern PID_Struct fiction_speed_pid[2];
extern PID_Struct g_speed_pid;						//取球抓手速度环PID
extern PID_Struct holder_z_visual_pid;
extern PID_Struct holder_y_visual_pid;
extern PID_Struct holder_yaw_pid;
extern PID_Struct holder_zAngleSpeed_pid;//云台偏航角速度PID
extern PID_Struct holder_yAngleSpeed_pid;//云台俯仰角速度PID
extern PID_Struct holder_pitch_pid;			//云台俯仰角度PID
extern PID_Struct holder_zSpeed_pid;			//云台偏航速度环PID
extern PID_Struct holder_ySpeed_pid;			//云台俯仰速度环PID
extern PID_Struct ball_entry_speed_pid;
#endif
