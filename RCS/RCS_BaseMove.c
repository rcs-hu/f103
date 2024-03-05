/*@filename:  RCS_BaseMove.c
 *@author     胡兴国       
 *@brief:     底盘算法
 *@date:      2023-5-29
*/
#include "RCS_BaseMove.h"

/*******************
X型长方形麦轮底盘，默认CAN1通讯
电机编号、相对坐标系及电机正转时轮子的有效方向如下
        ↑Y
        |
 1↗----+---↘2
   |    |   |
   |    +---+----→X
   |        |长
 4↖--------↙3
       宽

绝对坐标系定义如下
       ↑Y
       |
       |
       |
     Z +---------→ X      
********************/

/********全局变量********/

static float wheel_dir[4];		   //电机正转时轮子运动的正方向
static float wheel_distance;		 //轮子和底盘中心得距离
static float wheel_w_dir[4];		 //底盘自转时自转线速度在各轮子上的方向
static int angle_flag = 0;
static int distance_flag = 0;
/********静态函数********/
static void Wheel_Init(void);


/************************/


/**
	@name: M_BaseMove_Init
	@brief: 麦轮底盘初始化
**/
void M_BaseMove_Init(void)
{
		Motor_Init();		//电机初始化
		Wheel_Init();		//轮子参数初始化
}

/**
	@name: BaseMove_P2P
	@brief: 麦轮底盘基础算法(直角坐标式)
	@param:float move_speed_x             横向平动速度(r/min)
	@param:float move_speed_y							纵向平动速度(r/min)
	@param:float rotate_speed             自转模式(rad/s逆时针为正,手动控制时开启)
**/
void BaseMove_P2P(float move_speed_x,float move_speed_y,float rotate_speed)
{
    float target_speed[4] = {0,0,0,0};			//目标转速
		float max_speed= 0;
		int32_t out[4]={0,0,0,0};

    for(int i=0;i<4;i++)
		{
				target_speed[i] += move_speed_x * cos(-pi/2.0f - wheel_dir[i]) + move_speed_y * cos( 0 - wheel_dir[i]); //平动速度在轮毂正方向投影
				target_speed[i] += rotate_speed * cos(wheel_w_dir[i] - wheel_dir[i]);									//转动线速度在轮毂正方向投影
				target_speed[i] *= MM_S2R_MIN;				//将轮毂正方向线速度转化为轮子转速
				target_speed[i] *= SLOWDOWN_RATE;		  //将轮子转速转化为编码器转速以便于PID调速
				if(abs(target_speed[i]) > max_speed)
					max_speed = abs(target_speed[i]);
		}
		
		if(max_speed > MAX_MOTOR_SPEED)
		{
			for(int i=0;i<4;i++)
				target_speed[i] = target_speed[i] * MAX_MOTOR_SPEED / max_speed;
		}
		
		for(int i= 0;i<4;i++)
			out[i] = PID_Motor_Ctrl(target_speed[i],Get_Motor_Speed(i+1),&base_speed_pid[i]); //速度环

		Motor_Send(out[0], out[1], out[2], out[3]);
}

/**
	@name: BaseMove_Polar
	@brief: 麦轮底盘基础算法(极坐标式)
	@param: float direction_angle	行车方向
	@param: float run_speed				行车速度
	@param: float rotate_speed		自转速度
**/
void BaseMove_Polar(float direction_angle,float run_speed,float rotate_speed)
{
    float target_speed[4] = {0};			//目标转速
		float max_speed = 0;							//最大速度限制
		int32_t out[4];

    for(int i=0;i<4;i++)
		{
				target_speed[i] += run_speed * cos(direction_angle * DEG2RAD - wheel_dir[i]); 			//平动速度在轮毂正方向投影
				target_speed[i] += rotate_speed * cos(wheel_w_dir[i] - wheel_dir[i]);								//转动线速度在轮毂正方向投影
				if(abs(target_speed[i]) > max_speed)
					max_speed = abs(target_speed[i]);
		}
		if(max_speed > MAX_MOTOR_SPEED)
		{
			for(int i=0;i<4;i++)
				target_speed[i] = target_speed[i] * MAX_MOTOR_SPEED / max_speed;
		}
		for(int i= 0;i<4;i++)
			out[i] = PID_Motor_Ctrl(target_speed[i],Get_Motor_Speed(i+1),&base_speed_pid[i]); //速度环

		Motor_Send(out[0], out[1], out[2], out[3]);
}

/**
	@name: Point2Point_ER
	@brief: PID直线跑点，用于厦门大学2023年ROBOCON大象机器人。快准稳，缺点是不能跑曲线
	@param: float target_x		   目标X坐标
	@param: float target_y		   目标Y坐标
	@param: float target_z		   目标Z坐标 
	@param: float run_speed		   运动速度
  @eg:Point2Point_ER(1000,1000,30,1000,&ER_step);
      if(ER_step)
			{
				ER_step = 0;
				step ++;
			}
**/

void Point2Point_ER(float target_x,float target_y,float target_z,float run_speed,int *ER_flag)
{
	float x,y,z;
	float direction,speed,rotate_speed;
	float distance;
	
	//获取当前坐标
//	x = Get_GPS_X();  //X坐标
//	y = Get_GPS_Y();  //Y坐标
//	z = Get_GPS_Z();  //Z坐标（角度）
	
	//调整当前运动方向
	direction = -z -(atan2((target_x - x),(target_y - y))) * RAD2DEG;
	
	//判断离终点距离
	distance = sqrt((x - target_x)*(x - target_x) + (y - target_y)*(y - target_y));
	
	//判断是否到达终点
	if(distance <10.5)
	{
		distance_flag = 1;
		speed = 0;
	}
	else
	{
		distance_flag = 0;
	}
	
	if(distance_flag == 0)
	{
		//快速运动范围
		if(distance > 500)
		{
			speed = run_speed;
		}
		//减速
		else if((distance > 100)&&(distance <= 500))
		{
			speed = (run_speed - 200.0f) * (distance - 100.0f) / 400.0f + 200.0f;
		}
		//准备停车
		else
		{
			speed = 200;
		}
	}
	
//	if(((target_z-Get_GPS_Z())>=-R_ANGLE)&&((target_z-Get_GPS_Z())<=L_ANGLE))
//	{
//		angle_flag = 1;
//		rotate_speed = 0;
//	}
//	else
//	{
//		angle_flag = 0;
//	}
	
//	//旋转
//	if((angle_flag == 0)&&(distance <4000))
//	{
//		if(((target_z-Get_GPS_Z())>=-BIG_ANGLE)&&((target_z-Get_GPS_Z())<=BIG_ANGLE))
//		{
//			if((target_z-Get_GPS_Z())>L_ANGLE)
//			{
//				rotate_speed = 100;
//			}
//			if((target_z-Get_GPS_Z())<-R_ANGLE)
//			{
//				rotate_speed = -100;
//			}
//		}
//		else
//		{
//			if((target_z-Get_GPS_Z())>L_ANGLE)
//			{
//				rotate_speed = 500;
//			}
//			if((target_z-Get_GPS_Z())<-R_ANGLE)
//			{
//				rotate_speed = -500;
//			}
//	  }			
//  }
	
	BaseMove_Polar(direction,speed,rotate_speed);
  
	//判断是否到达
	if((distance_flag == 1)&&(angle_flag == 1))
	{
		distance_flag = 0;
		angle_flag = 0;
		*ER_flag =  1;
	}
	else
	{
		*ER_flag =  0;
	}
	return;
}

/**
	@name: Joystick_Ctrl
	@brief: 遥控
**/
void Joystick_Ctrl(void)
{
	float Speed1=0,Speed2=0,Speed3=0,Speed4=0,speed1,speed2,speed3,speed4,output1,output2,output3,output4,NUM=1000;
//	if(direction_key[0])
//	{
//		Speed1+=NUM;Speed2-=NUM;Speed3-=NUM;Speed4+=NUM;
//	}
//	if(direction_key[1])
//	{
//		Speed1-=NUM;Speed2+=NUM;Speed3+=NUM;Speed4-=NUM;
//	}
//	if(direction_key[2])
//	{
//		Speed1-=NUM;Speed2-=NUM;Speed3+=NUM;Speed4+=NUM;
//	}
//	if(direction_key[3])
//	{
//		Speed1+=NUM;Speed2+=NUM;Speed3-=NUM;Speed4-=NUM;
//	}
//	
//  if(key_flag[0])
//	{
//		Speed1-=NUM/2;Speed2-=NUM/2;Speed3-=NUM/2;Speed4-=NUM/2;
//	}
//	if(key_flag[1])
//	{
//		Speed1+=NUM/2;Speed2+=NUM/2;Speed3+=NUM/2;Speed4+=NUM/2;
//	}
	
	speed1=Get_Motor_Speed(1);
	output1=PID_Motor_Ctrl(Speed1,speed1,&base_speed_pid[0]);
	speed2=Get_Motor_Speed(2);
	output2=PID_Motor_Ctrl(Speed2,speed2,&base_speed_pid[1]);
	speed3=Get_Motor_Speed(3);
	output3=PID_Motor_Ctrl(Speed3,speed3,&base_speed_pid[2]);
	speed4=Get_Motor_Speed(4);
	output4=PID_Motor_Ctrl(Speed4,speed4,&base_speed_pid[3]);
	Motor_Send(output1,output2,output3,output4);	
}


/**
	@name: Wheel_Init
	@brief: 轮子参数初始化
**/
static void Wheel_Init(void)
{
	  //设置轮毂正方向（见最上方图例）
		wheel_dir[0] = -pi/4.0f;			
		wheel_dir[1] = 3 * wheel_dir[0];
		wheel_dir[2] = -wheel_dir[1];
		wheel_dir[3] = -wheel_dir[0];
		//设置自转时自转线速度在各轮子上的方向
		wheel_w_dir[0] = atan2( BASE_WIDTH, BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[1] = atan2(-BASE_WIDTH, BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[2] = atan2(-BASE_WIDTH,-BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[3] = atan2( BASE_WIDTH,-BASE_LENGTH) + pi/2.0f;
		//计算轮子与底盘中心距离
		wheel_distance = sqrt(BASE_LENGTH * BASE_LENGTH + BASE_WIDTH * BASE_WIDTH) / 2.0f;
}
