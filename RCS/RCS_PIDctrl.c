/*@filename: RCS_PIDctrl.c
 *@author     胡兴国       
 *@brief:     PID算法
 *@date: 2023-4-28
*/

#include "RCS_PIDctrl.h"

/*-------------常规底盘PID------------*/
PID_Struct speed_pid;							//速度环PID
PID_Struct base_speed_pid[4];			//底盘速度环
PID_Struct angle_pid;							//偏航角直接输出电机转速PID
PID_Struct coord_pid[2];					//坐标反馈PID
PID_Struct distance_x_pid;				//距离x反馈PID
PID_Struct distance_y_pid;				//距离y反馈PID
PID_Struct anglespeed_pid;				//偏航角角速度环PID
PID_Struct angle_w_pid;						//偏航角转输出角速度PID
/*---------------上层PID-------------*/
PID_Struct g_speed_pid;						//取球抓手速度环PID
PID_Struct fiction_speed_pid[2]; 	//摩擦轮速度PID
PID_Struct ball_entry_speed_pid;	//球进给速度PID
/*---------------云台PID-------------*/
PID_Struct holder_z_visual_pid;		//云台偏航视觉PIDs
PID_Struct holder_y_visual_pid;		//云台俯仰视觉PID
PID_Struct holder_yaw_pid;				//云台偏航角度PID
PID_Struct holder_zAngleSpeed_pid;//云台偏航角速度PID
PID_Struct holder_yAngleSpeed_pid;//云台俯仰角速度PID
PID_Struct holder_pitch_pid;			//云台俯仰角度PID
PID_Struct holder_zSpeed_pid;			//云台偏航速度环PID
PID_Struct holder_ySpeed_pid;			//云台俯仰速度环PID

/**
	@name: PID_Init
	@brief: PID初始化
**/
void PID_Init(void)
{
		//电机角度速度环PID
		speed_pid.P = 8.3;
		speed_pid.I = 0.25;
	  speed_pid.D = 0.38;
		speed_pid.Last_Error = 0;			
		speed_pid.Limit_Output = LIMIT_SPEED;  	
		speed_pid.Limit_Integral = LIMIT_ERR; 
		speed_pid.Integral = 0; 			
		
		//底盘电机速度环PID
//		for(int i = 0;i<4;i++)
//		{
//			//base_speed_pid[i] = speed_pid;
//	  base_speed_pid[i].P = 8.3;
//		base_speed_pid[i].I = 0.25;
//	  base_speed_pid[i].D = 0.38;
//		base_speed_pid[i].Last_Error = 0;			
//		base_speed_pid[i].Limit_Output = LIMIT_SPEED;  	
//		base_speed_pid[i].Limit_Integral = LIMIT_ERR; 
//		base_speed_pid[i].Integral = 0; 			
//		}
		
		base_speed_pid[0].P = 8.3;
		base_speed_pid[0].I = 0.25;
	  base_speed_pid[0].D = 0.38;
		base_speed_pid[0].Last_Error = 0;			
		base_speed_pid[0].Limit_Output = LIMIT_SPEED;  	
		base_speed_pid[0].Limit_Integral = LIMIT_ERR; 
		base_speed_pid[0].Integral = 0;
		
		base_speed_pid[1].P = 8.3;
		base_speed_pid[1].I = 0.25;
	  base_speed_pid[1].D = 0.38;
		base_speed_pid[1].Last_Error = 0;			
		base_speed_pid[1].Limit_Output = LIMIT_SPEED;  	
		base_speed_pid[1].Limit_Integral = LIMIT_ERR; 
		base_speed_pid[1].Integral = 0;
		
		base_speed_pid[2].P = 8.3;
		base_speed_pid[2].I = 0.25;
	  base_speed_pid[2].D = 0.38;
		base_speed_pid[2].Last_Error = 0;			
		base_speed_pid[2].Limit_Output = LIMIT_SPEED;  	
		base_speed_pid[2].Limit_Integral = LIMIT_ERR; 
		base_speed_pid[2].Integral = 0;
		
		base_speed_pid[3].P = 8.3;
		base_speed_pid[3].I = 0.25;
	  base_speed_pid[3].D = 0.38;
		base_speed_pid[3].Last_Error = 0;			
		base_speed_pid[3].Limit_Output = LIMIT_SPEED;  	
		base_speed_pid[3].Limit_Integral = LIMIT_ERR; 
		base_speed_pid[3].Integral = 0;
		
		fiction_speed_pid[0] = speed_pid;
		fiction_speed_pid[1] = speed_pid;  //摩擦力控制
		//holder_zSpeed_pid = speed_pid;
		//holder_ySpeed_pid = speed_pid;
		//g_speed_pid = speed_pid;
		ball_entry_speed_pid = speed_pid;
		//偏航角直接输出电机转速PID
		angle_pid.P = 50;
	  angle_pid.I = 0.03;
	  angle_pid.D = 1.5;
		angle_pid.Last_Error = 0;
		angle_pid.Limit_Output = LIMIT_SPEED;
		angle_pid.Limit_Integral = LIMIT_ERR;
		angle_pid.Integral = 0; 		
		
		//偏航角输出角速度PID
		angle_w_pid.P = 8;
	  angle_w_pid.I = 0.047;
	  angle_w_pid.D = 0;
		angle_w_pid.Last_Error = 0;			
		angle_w_pid.Limit_Output = LIMIT_SPEED;  	
		angle_w_pid.Limit_Integral = LIMIT_ERR; 
		angle_w_pid.Integral = 0; 		//没有使用
		
		//角度控制 速度环PID
		anglespeed_pid.P = 50;
		anglespeed_pid.I = 0.03;
		anglespeed_pid.D = 1.5;
		anglespeed_pid.Last_Error = 0;			
		anglespeed_pid.Limit_Output = LIMIT_ROTATE_SPEED;  	
		anglespeed_pid.Limit_Integral = LIMIT_ERR; 
		anglespeed_pid.Integral = 0; 		
		
		//坐标环PID
		coord_pid[0].P = 30;
		coord_pid[0].I = 0.02;
		coord_pid[0].D = 2;
		coord_pid[0].Last_Error = 0;			
		coord_pid[0].Limit_Output = 500.0f;  //LIMIT_SPEED;  	
		coord_pid[0].Limit_Integral = LIMIT_ERR; 
		coord_pid[0].Integral = 0; 		
		coord_pid[1] = coord_pid[0]; //PID点对点移动
		
		//距离控制PID
		distance_x_pid.P = 8;
		distance_x_pid.I = 0.02;
		distance_x_pid.D = 0.3;
		distance_x_pid.Last_Error = 0;			
		distance_x_pid.Limit_Output = 4000;  	
		distance_x_pid.Limit_Integral = LIMIT_ERR; 
		distance_x_pid.Integral = 0; 		
		//距离控制PID
		distance_y_pid = distance_x_pid;
		
		//g_speed_pid = speed_pid;
		g_speed_pid.P = 14.0;
		g_speed_pid.I = 0.25;
	  g_speed_pid.D = 0.38;
		g_speed_pid.Last_Error = 0;			
		g_speed_pid.Limit_Output = LIMIT_SPEED;  	
		g_speed_pid.Limit_Integral = LIMIT_ERR; 
		g_speed_pid.Integral = 0; 	
		
		holder_zSpeed_pid.P = 12.0;
		holder_zSpeed_pid.I = 0.25;
	  holder_zSpeed_pid.D = 0.38;
		holder_zSpeed_pid.Last_Error = 0;			
		holder_zSpeed_pid.Limit_Output = LIMIT_SPEED;  	
		holder_zSpeed_pid.Limit_Integral = LIMIT_ERR; 
		holder_zSpeed_pid.Integral = 0; 	
		
		holder_ySpeed_pid.P = 14.0;
		holder_ySpeed_pid.I = 0.25;
	  holder_ySpeed_pid.D = 0.38;
		holder_ySpeed_pid.Last_Error = 0;			
		holder_ySpeed_pid.Limit_Output = LIMIT_SPEED;  	
		holder_ySpeed_pid.Limit_Integral = LIMIT_ERR; 
		holder_ySpeed_pid.Integral = 0; 	
		
		//云台PID
		holder_y_visual_pid.P = 20.0f;
		holder_y_visual_pid.I = 0.02f;
		holder_y_visual_pid.D = 1.0f;
		holder_y_visual_pid.Last_Error = 0;			
		holder_y_visual_pid.Limit_Output = LIMIT_SPEED;  	
		holder_y_visual_pid.Limit_Integral = LIMIT_ERR; 
		holder_y_visual_pid.Integral = 0;
		
		holder_z_visual_pid.P = 20.0f;
		holder_z_visual_pid.I = 0.02f;
		holder_z_visual_pid.D = 1.0f;
		holder_z_visual_pid.Last_Error = 0;			
		holder_z_visual_pid.Limit_Output = LIMIT_SPEED;  	
		holder_z_visual_pid.Limit_Integral = LIMIT_ERR; 
		holder_z_visual_pid.Integral = 0;

		holder_yaw_pid.P = 20.0f;
		holder_yaw_pid.I = 0.02f;
		holder_yaw_pid.D = 1.0f;
		holder_yaw_pid.Last_Error = 0;			
		holder_yaw_pid.Limit_Output = LIMIT_SPEED;  	
		holder_yaw_pid.Limit_Integral = LIMIT_ERR; 
		holder_yaw_pid.Integral = 0;

		holder_zAngleSpeed_pid.P = 20.0f;
		holder_zAngleSpeed_pid.I = 0.02f;
		holder_zAngleSpeed_pid.D = 1.0f;
		holder_zAngleSpeed_pid.Last_Error = 0;			
		holder_zAngleSpeed_pid.Limit_Output = LIMIT_SPEED;  	
		holder_zAngleSpeed_pid.Limit_Integral = LIMIT_ERR; 
		holder_zAngleSpeed_pid.Integral = 0;
		
		holder_yAngleSpeed_pid.P = 20.0f;
		holder_yAngleSpeed_pid.I = 0.02f;
		holder_yAngleSpeed_pid.D = 1.0f;
		holder_yAngleSpeed_pid.Last_Error = 0;			
		holder_yAngleSpeed_pid.Limit_Output = LIMIT_SPEED;  	
		holder_yAngleSpeed_pid.Limit_Integral = LIMIT_ERR; 
		holder_yAngleSpeed_pid.Integral = 0;
		
		holder_pitch_pid.P = 20.0f;
		holder_pitch_pid.I = 0.02f;
		holder_pitch_pid.D = 1.0f;
		holder_pitch_pid.Last_Error = 0;			
		holder_pitch_pid.Limit_Output = LIMIT_SPEED;  	
		holder_pitch_pid.Limit_Integral = LIMIT_ERR; 
		holder_pitch_pid.Integral = 0;
		
}

/**
	@name: PID_Normal_Ctrl
	@brief: 通用PID函数
	@param:float target 				目标值
	@param:float current 				当前值
	@param:PID_Struct *_pid   	PID
	@return:float								浮点值
**/
float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid)
{
		float err;
		float	different_err;
    float output;         
    
    err =target - current;														//误差
    _pid->Integral += err;														//积分误差
    different_err = err - _pid->Last_Error;						//微分误差
    
    if(_pid->Integral >= _pid->Limit_Integral)        //积分限幅
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID整合
    
    if(output >= _pid->Limit_Output)          			//输出限幅
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//记录上次误差
		return output;
}

/**
	@name: PID_ANGLE1_Ctrl
	@brief: 角度控制PID函数
	@param:float target 				目标值
	@param:float current 				当前值
	@param:PID_Struct *_pid   	PID
	@return:float								浮点值
**/
float PID_ANGLE_Ctrl(float target,float current,PID_Struct *_pid)
{
		float err;
		float	different_err;
    float output;         
    
    err =target - current;														//误差
    _pid->Integral += err;														//积分误差
    different_err = err - _pid->Last_Error;						//微分误差
    
    if(_pid->Integral >= _pid->Limit_Integral)        //积分限幅
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID整合
    
    if(output >= _pid->Limit_Output)          			//输出限幅
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//记录上次误差
		return output;
}


/**
	@name: PID_Motor_Ctrl
	@brief: 电机PID控制函数
	@param:float target 				目标值
	@param:float current 				当前值
	@param:PID_Struct *_pid   	PID
	@return:int32_t							电机电流值
**/
int32_t PID_Motor_Ctrl(float target,float current,PID_Struct *_pid)
{
		int32_t err;						
		int32_t	different_err;	
    int32_t output;         
    
    err =target - current;														//误差
    _pid->Integral += err;														//积分误差
    different_err = err - _pid->Last_Error;						//微分误差
    
    if(_pid->Integral >= _pid->Limit_Integral)        //积分限幅
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID整合
    
    if(output >= _pid->Limit_Output)          			//输出限幅
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//记录上次误差
		return output;
}


/**
	@name: PID_Yaw_Ctrl
	@brief: PID偏航角反馈控制
	@param:float target_angle		目标偏航角
	@param:float current_angle 	当前偏航角
	@param:float current_angleSpeed	当前角速度
	@param:PID_Struct *_pid   	PID
	@return:										角速度值
**/
float PID_Yaw_Ctrl(float target_angle,float current_angle,float current_angleSpeed,PID_Struct *_pid)
{
		float err;
    float output;         
    
    err =target_angle - current_angle;								//误差
    _pid->Integral += err;														//积分误差
    
    if(_pid->Integral >= _pid->Limit_Integral)        //积分限幅
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * current_angleSpeed;			//PID整合
    
    if(output >= _pid->Limit_Output)          			//输出限幅
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//记录上次误差
		return output;
}
