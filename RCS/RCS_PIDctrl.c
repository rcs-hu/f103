/*@filename: RCS_PIDctrl.c
 *@author     ���˹�       
 *@brief:     PID�㷨
 *@date: 2023-4-28
*/

#include "RCS_PIDctrl.h"

/*-------------�������PID------------*/
PID_Struct speed_pid;							//�ٶȻ�PID
PID_Struct base_speed_pid[4];			//�����ٶȻ�
PID_Struct angle_pid;							//ƫ����ֱ��������ת��PID
PID_Struct coord_pid[2];					//���귴��PID
PID_Struct distance_x_pid;				//����x����PID
PID_Struct distance_y_pid;				//����y����PID
PID_Struct anglespeed_pid;				//ƫ���ǽ��ٶȻ�PID
PID_Struct angle_w_pid;						//ƫ����ת������ٶ�PID
/*---------------�ϲ�PID-------------*/
PID_Struct g_speed_pid;						//ȡ��ץ���ٶȻ�PID
PID_Struct fiction_speed_pid[2]; 	//Ħ�����ٶ�PID
PID_Struct ball_entry_speed_pid;	//������ٶ�PID
/*---------------��̨PID-------------*/
PID_Struct holder_z_visual_pid;		//��̨ƫ���Ӿ�PIDs
PID_Struct holder_y_visual_pid;		//��̨�����Ӿ�PID
PID_Struct holder_yaw_pid;				//��̨ƫ���Ƕ�PID
PID_Struct holder_zAngleSpeed_pid;//��̨ƫ�����ٶ�PID
PID_Struct holder_yAngleSpeed_pid;//��̨�������ٶ�PID
PID_Struct holder_pitch_pid;			//��̨�����Ƕ�PID
PID_Struct holder_zSpeed_pid;			//��̨ƫ���ٶȻ�PID
PID_Struct holder_ySpeed_pid;			//��̨�����ٶȻ�PID

/**
	@name: PID_Init
	@brief: PID��ʼ��
**/
void PID_Init(void)
{
		//����Ƕ��ٶȻ�PID
		speed_pid.P = 8.3;
		speed_pid.I = 0.25;
	  speed_pid.D = 0.38;
		speed_pid.Last_Error = 0;			
		speed_pid.Limit_Output = LIMIT_SPEED;  	
		speed_pid.Limit_Integral = LIMIT_ERR; 
		speed_pid.Integral = 0; 			
		
		//���̵���ٶȻ�PID
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
		fiction_speed_pid[1] = speed_pid;  //Ħ��������
		//holder_zSpeed_pid = speed_pid;
		//holder_ySpeed_pid = speed_pid;
		//g_speed_pid = speed_pid;
		ball_entry_speed_pid = speed_pid;
		//ƫ����ֱ��������ת��PID
		angle_pid.P = 50;
	  angle_pid.I = 0.03;
	  angle_pid.D = 1.5;
		angle_pid.Last_Error = 0;
		angle_pid.Limit_Output = LIMIT_SPEED;
		angle_pid.Limit_Integral = LIMIT_ERR;
		angle_pid.Integral = 0; 		
		
		//ƫ����������ٶ�PID
		angle_w_pid.P = 8;
	  angle_w_pid.I = 0.047;
	  angle_w_pid.D = 0;
		angle_w_pid.Last_Error = 0;			
		angle_w_pid.Limit_Output = LIMIT_SPEED;  	
		angle_w_pid.Limit_Integral = LIMIT_ERR; 
		angle_w_pid.Integral = 0; 		//û��ʹ��
		
		//�Ƕȿ��� �ٶȻ�PID
		anglespeed_pid.P = 50;
		anglespeed_pid.I = 0.03;
		anglespeed_pid.D = 1.5;
		anglespeed_pid.Last_Error = 0;			
		anglespeed_pid.Limit_Output = LIMIT_ROTATE_SPEED;  	
		anglespeed_pid.Limit_Integral = LIMIT_ERR; 
		anglespeed_pid.Integral = 0; 		
		
		//���껷PID
		coord_pid[0].P = 30;
		coord_pid[0].I = 0.02;
		coord_pid[0].D = 2;
		coord_pid[0].Last_Error = 0;			
		coord_pid[0].Limit_Output = 500.0f;  //LIMIT_SPEED;  	
		coord_pid[0].Limit_Integral = LIMIT_ERR; 
		coord_pid[0].Integral = 0; 		
		coord_pid[1] = coord_pid[0]; //PID��Ե��ƶ�
		
		//�������PID
		distance_x_pid.P = 8;
		distance_x_pid.I = 0.02;
		distance_x_pid.D = 0.3;
		distance_x_pid.Last_Error = 0;			
		distance_x_pid.Limit_Output = 4000;  	
		distance_x_pid.Limit_Integral = LIMIT_ERR; 
		distance_x_pid.Integral = 0; 		
		//�������PID
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
		
		//��̨PID
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
	@brief: ͨ��PID����
	@param:float target 				Ŀ��ֵ
	@param:float current 				��ǰֵ
	@param:PID_Struct *_pid   	PID
	@return:float								����ֵ
**/
float PID_Normal_Ctrl(float target,float current,PID_Struct *_pid)
{
		float err;
		float	different_err;
    float output;         
    
    err =target - current;														//���
    _pid->Integral += err;														//�������
    different_err = err - _pid->Last_Error;						//΢�����
    
    if(_pid->Integral >= _pid->Limit_Integral)        //�����޷�
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID����
    
    if(output >= _pid->Limit_Output)          			//����޷�
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//��¼�ϴ����
		return output;
}

/**
	@name: PID_ANGLE1_Ctrl
	@brief: �Ƕȿ���PID����
	@param:float target 				Ŀ��ֵ
	@param:float current 				��ǰֵ
	@param:PID_Struct *_pid   	PID
	@return:float								����ֵ
**/
float PID_ANGLE_Ctrl(float target,float current,PID_Struct *_pid)
{
		float err;
		float	different_err;
    float output;         
    
    err =target - current;														//���
    _pid->Integral += err;														//�������
    different_err = err - _pid->Last_Error;						//΢�����
    
    if(_pid->Integral >= _pid->Limit_Integral)        //�����޷�
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID����
    
    if(output >= _pid->Limit_Output)          			//����޷�
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//��¼�ϴ����
		return output;
}


/**
	@name: PID_Motor_Ctrl
	@brief: ���PID���ƺ���
	@param:float target 				Ŀ��ֵ
	@param:float current 				��ǰֵ
	@param:PID_Struct *_pid   	PID
	@return:int32_t							�������ֵ
**/
int32_t PID_Motor_Ctrl(float target,float current,PID_Struct *_pid)
{
		int32_t err;						
		int32_t	different_err;	
    int32_t output;         
    
    err =target - current;														//���
    _pid->Integral += err;														//�������
    different_err = err - _pid->Last_Error;						//΢�����
    
    if(_pid->Integral >= _pid->Limit_Integral)        //�����޷�
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * different_err;			//PID����
    
    if(output >= _pid->Limit_Output)          			//����޷�
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//��¼�ϴ����
		return output;
}


/**
	@name: PID_Yaw_Ctrl
	@brief: PIDƫ���Ƿ�������
	@param:float target_angle		Ŀ��ƫ����
	@param:float current_angle 	��ǰƫ����
	@param:float current_angleSpeed	��ǰ���ٶ�
	@param:PID_Struct *_pid   	PID
	@return:										���ٶ�ֵ
**/
float PID_Yaw_Ctrl(float target_angle,float current_angle,float current_angleSpeed,PID_Struct *_pid)
{
		float err;
    float output;         
    
    err =target_angle - current_angle;								//���
    _pid->Integral += err;														//�������
    
    if(_pid->Integral >= _pid->Limit_Integral)        //�����޷�
        _pid->Integral = _pid->Limit_Integral;
    if(_pid->Integral <= -_pid->Limit_Integral)
        _pid->Integral =-_pid->Limit_Integral;
    
    output = _pid->P * err + _pid->I * _pid->Integral + _pid->D * current_angleSpeed;			//PID����
    
    if(output >= _pid->Limit_Output)          			//����޷�
        output = _pid->Limit_Output;
    if(output <= - _pid->Limit_Output)
        output = - _pid->Limit_Output;
    
		_pid->Last_Error = err;													//��¼�ϴ����
		return output;
}
