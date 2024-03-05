/*@filename:  RCS_BaseMove.c
 *@author     ���˹�       
 *@brief:     �����㷨
 *@date:      2023-5-29
*/
#include "RCS_BaseMove.h"

/*******************
X�ͳ��������ֵ��̣�Ĭ��CAN1ͨѶ
�����š��������ϵ�������תʱ���ӵ���Ч��������
        ��Y
        |
 1�J----+---�K2
   |    |   |
   |    +---+----��X
   |        |��
 4�I--------�L3
       ��

��������ϵ��������
       ��Y
       |
       |
       |
     Z +---------�� X      
********************/

/********ȫ�ֱ���********/

static float wheel_dir[4];		   //�����תʱ�����˶���������
static float wheel_distance;		 //���Ӻ͵������ĵþ���
static float wheel_w_dir[4];		 //������תʱ��ת���ٶ��ڸ������ϵķ���
static int angle_flag = 0;
static int distance_flag = 0;
/********��̬����********/
static void Wheel_Init(void);


/************************/


/**
	@name: M_BaseMove_Init
	@brief: ���ֵ��̳�ʼ��
**/
void M_BaseMove_Init(void)
{
		Motor_Init();		//�����ʼ��
		Wheel_Init();		//���Ӳ�����ʼ��
}

/**
	@name: BaseMove_P2P
	@brief: ���ֵ��̻����㷨(ֱ������ʽ)
	@param:float move_speed_x             ����ƽ���ٶ�(r/min)
	@param:float move_speed_y							����ƽ���ٶ�(r/min)
	@param:float rotate_speed             ��תģʽ(rad/s��ʱ��Ϊ��,�ֶ�����ʱ����)
**/
void BaseMove_P2P(float move_speed_x,float move_speed_y,float rotate_speed)
{
    float target_speed[4] = {0,0,0,0};			//Ŀ��ת��
		float max_speed= 0;
		int32_t out[4]={0,0,0,0};

    for(int i=0;i<4;i++)
		{
				target_speed[i] += move_speed_x * cos(-pi/2.0f - wheel_dir[i]) + move_speed_y * cos( 0 - wheel_dir[i]); //ƽ���ٶ������������ͶӰ
				target_speed[i] += rotate_speed * cos(wheel_w_dir[i] - wheel_dir[i]);									//ת�����ٶ������������ͶӰ
				target_speed[i] *= MM_S2R_MIN;				//��������������ٶ�ת��Ϊ����ת��
				target_speed[i] *= SLOWDOWN_RATE;		  //������ת��ת��Ϊ������ת���Ա���PID����
				if(abs(target_speed[i]) > max_speed)
					max_speed = abs(target_speed[i]);
		}
		
		if(max_speed > MAX_MOTOR_SPEED)
		{
			for(int i=0;i<4;i++)
				target_speed[i] = target_speed[i] * MAX_MOTOR_SPEED / max_speed;
		}
		
		for(int i= 0;i<4;i++)
			out[i] = PID_Motor_Ctrl(target_speed[i],Get_Motor_Speed(i+1),&base_speed_pid[i]); //�ٶȻ�

		Motor_Send(out[0], out[1], out[2], out[3]);
}

/**
	@name: BaseMove_Polar
	@brief: ���ֵ��̻����㷨(������ʽ)
	@param: float direction_angle	�г�����
	@param: float run_speed				�г��ٶ�
	@param: float rotate_speed		��ת�ٶ�
**/
void BaseMove_Polar(float direction_angle,float run_speed,float rotate_speed)
{
    float target_speed[4] = {0};			//Ŀ��ת��
		float max_speed = 0;							//����ٶ�����
		int32_t out[4];

    for(int i=0;i<4;i++)
		{
				target_speed[i] += run_speed * cos(direction_angle * DEG2RAD - wheel_dir[i]); 			//ƽ���ٶ������������ͶӰ
				target_speed[i] += rotate_speed * cos(wheel_w_dir[i] - wheel_dir[i]);								//ת�����ٶ������������ͶӰ
				if(abs(target_speed[i]) > max_speed)
					max_speed = abs(target_speed[i]);
		}
		if(max_speed > MAX_MOTOR_SPEED)
		{
			for(int i=0;i<4;i++)
				target_speed[i] = target_speed[i] * MAX_MOTOR_SPEED / max_speed;
		}
		for(int i= 0;i<4;i++)
			out[i] = PID_Motor_Ctrl(target_speed[i],Get_Motor_Speed(i+1),&base_speed_pid[i]); //�ٶȻ�

		Motor_Send(out[0], out[1], out[2], out[3]);
}

/**
	@name: Point2Point_ER
	@brief: PIDֱ���ܵ㣬�������Ŵ�ѧ2023��ROBOCON��������ˡ���׼�ȣ�ȱ���ǲ���������
	@param: float target_x		   Ŀ��X����
	@param: float target_y		   Ŀ��Y����
	@param: float target_z		   Ŀ��Z���� 
	@param: float run_speed		   �˶��ٶ�
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
	
	//��ȡ��ǰ����
//	x = Get_GPS_X();  //X����
//	y = Get_GPS_Y();  //Y����
//	z = Get_GPS_Z();  //Z���꣨�Ƕȣ�
	
	//������ǰ�˶�����
	direction = -z -(atan2((target_x - x),(target_y - y))) * RAD2DEG;
	
	//�ж����յ����
	distance = sqrt((x - target_x)*(x - target_x) + (y - target_y)*(y - target_y));
	
	//�ж��Ƿ񵽴��յ�
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
		//�����˶���Χ
		if(distance > 500)
		{
			speed = run_speed;
		}
		//����
		else if((distance > 100)&&(distance <= 500))
		{
			speed = (run_speed - 200.0f) * (distance - 100.0f) / 400.0f + 200.0f;
		}
		//׼��ͣ��
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
	
//	//��ת
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
  
	//�ж��Ƿ񵽴�
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
	@brief: ң��
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
	@brief: ���Ӳ�����ʼ��
**/
static void Wheel_Init(void)
{
	  //������������򣨼����Ϸ�ͼ����
		wheel_dir[0] = -pi/4.0f;			
		wheel_dir[1] = 3 * wheel_dir[0];
		wheel_dir[2] = -wheel_dir[1];
		wheel_dir[3] = -wheel_dir[0];
		//������תʱ��ת���ٶ��ڸ������ϵķ���
		wheel_w_dir[0] = atan2( BASE_WIDTH, BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[1] = atan2(-BASE_WIDTH, BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[2] = atan2(-BASE_WIDTH,-BASE_LENGTH) + pi/2.0f;
		wheel_w_dir[3] = atan2( BASE_WIDTH,-BASE_LENGTH) + pi/2.0f;
		//����������������ľ���
		wheel_distance = sqrt(BASE_LENGTH * BASE_LENGTH + BASE_WIDTH * BASE_WIDTH) / 2.0f;
}
