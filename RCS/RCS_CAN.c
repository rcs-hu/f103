/*@filename: RCS_CAN.c
 *@author     胡兴国       
 *@brief:     can通讯（主要用于大疆3508）
 *@date: 2023-5-2
*/

#include "RCS_CAN.h"

/***************全局变量*****************/
static float g_encoder_start_angle[8];
static uint16_t g_encoder_oringin_angle[8];
static uint16_t g_encoder_last_angle[8];
static int16_t g_encoder_cycles[8];
volatile float g_encoder_float_angle[8];
volatile int32_t g_encoder_int_angle[8];
static int16_t g_encoder_oringin_speed[8];
volatile int16_t g_encoder_speed[8];
/****************************************/

//电机初始化
void Motor_Init(void)
{
		for (int i = 0; i < 8; i++)
	{
              
		g_encoder_start_angle[i] = 0;
		g_encoder_oringin_angle[i] = 0;
		g_encoder_cycles[i] = 0;
		g_encoder_float_angle[i] = 0;
		g_encoder_int_angle[i] = 0;
		g_encoder_oringin_speed[i] = 0;
		g_encoder_speed[i] = 0;
	}
	
	RCS_CAN1_Init();  //CAN1引脚初始化
	delay_ms(10);
	for (int i = 0; i < 8; i++)
		g_encoder_start_angle[i] = g_encoder_float_angle[i];
}

//@name: RCS_CAN1_Init
//@brief: 配置CAN1的通信功能
//@param: _baudRate  设置波特率（波特率需能整除3000000）
void RCS_CAN1_Init(void)
{	
	CAN_InitTypeDef CAN_InitStructure; //can初始化结构体
	CAN_FilterInitTypeDef CAN_FilterInitStructure; //过滤器初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量		
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能 CAN1 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CAN1_TXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CAN1_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//can初始化结构体配置
	CAN_InitStructure.CAN_TTCM=DISABLE; //非时间触发通信模式
	CAN_InitStructure.CAN_ABOM=ENABLE; //软件自动离线管理
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR 的 SLEEP 位)
	CAN_InitStructure.CAN_NART=ENABLE; //使用报文自动传送
	CAN_InitStructure.CAN_RFLM=DISABLE; //报文不锁定,新的覆盖旧的
	CAN_InitStructure.CAN_TXFP=ENABLE; //优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //模式设置
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //重同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq-CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq; //Tbs1范围 CAN_BS1_1tq-CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;//Tbs2范围CAN_BS2_1tq -CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = 4; //1M (baudrate = pclk1/(1+bs1+bs2)/psc)
	CAN_Init(CAN1, &CAN_InitStructure); // 初始化 CAN1

	//过滤器初始化结构体配置
	CAN_FilterInitStructure.CAN_FilterNumber=0; //过滤器 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32 位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32 位 ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32 位 MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器 0 关联到 FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
		
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	
	
}


//@name: Motor_Send
//@brief: 给3508的电调发送控制数据
//@param:ID1_Current 电调ID为1的电流数据
//@param:ID2_Current 电调ID为2的电流数据
//@param:ID3_Current 电调ID为3的电流数据
//@param:ID4_Current 电调ID为4的电流数据
void Motor_Send(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current)
{
	uint8_t current_data[8];

	Current_Data_Takepart(ID1_Current, &current_data[0], &current_data[1]);
	Current_Data_Takepart(ID2_Current, &current_data[2], &current_data[3]);
	Current_Data_Takepart(ID3_Current, &current_data[4], &current_data[5]);
	Current_Data_Takepart(ID4_Current, &current_data[6], &current_data[7]);        
	RCS_CANx_Send(CAN1,0x200, 0x08, current_data);
}

//@name: Motor_Send_2006
//@brief: 给3508的电调发送控制数据
//@param:ID1_Current 电调ID为5的电流数据
//@param:ID2_Current 电调ID为6的电流数据
//@param:ID3_Current 电调ID为7的电流数据
//@param:ID4_Current 电调ID为8的电流数据
void Motor_Send_ADD(int16_t ID5_Current, int16_t ID6_Current, int16_t ID7_Current, int16_t ID8_Current)
{
	uint8_t current_data[8];

	Current_Data_Takepart(ID5_Current, &current_data[0], &current_data[1]);
	Current_Data_Takepart(ID6_Current, &current_data[2], &current_data[3]);
	Current_Data_Takepart(ID7_Current, &current_data[4], &current_data[5]);
	Current_Data_Takepart(ID8_Current, &current_data[6], &current_data[7]);        
	RCS_CANx_Send(CAN1,0x1FF, 0x08, current_data);
}


//CAN1接收中断
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;

	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

		if ((rx_message.IDE == CAN_Id_Standard) && (rx_message.RTR == CAN_RTR_Data) && (rx_message.DLC == 0x08)) //标准帧、数据帧长度为8
		{
			//电机号解析
			uint8_t motor_number = (rx_message.StdId & 0x000f);

			//速度数据解析
			g_encoder_oringin_speed[motor_number - 1] = (rx_message.Data[2] << 8) | (rx_message.Data[3]);
			if (g_encoder_oringin_speed[motor_number - 1] <= 3 && g_encoder_oringin_speed[motor_number - 1] >= -3)
				g_encoder_speed[motor_number - 1] = 0;
			else
				g_encoder_speed[motor_number - 1] = g_encoder_oringin_speed[motor_number - 1];

			//位置数据解析
			g_encoder_oringin_angle[motor_number - 1] = (rx_message.Data[0] << 8) | (rx_message.Data[1]);
			//跨0度线判定
			if (abs(g_encoder_oringin_angle[motor_number - 1] - g_encoder_last_angle[motor_number - 1]) > 6000)
			{
				if (g_encoder_last_angle[motor_number - 1] > 4000)g_encoder_cycles[motor_number - 1]++;
				else g_encoder_cycles[motor_number - 1]--;
			}
			g_encoder_last_angle[motor_number - 1] = g_encoder_oringin_angle[motor_number - 1];
			g_encoder_float_angle[motor_number - 1] = 360.0 * g_encoder_cycles[motor_number - 1] + g_encoder_oringin_angle[motor_number - 1] * ENCODER2ANGLE - g_encoder_start_angle[motor_number - 1];
			g_encoder_int_angle[motor_number - 1] = (int32_t)g_encoder_float_angle[motor_number - 1];
		}
		CAN_FIFORelease(CAN1, CAN_FIFO0);
	}
}

//获取CAN1浮点电机机械角度,角度值，启动时角度为0
float Get_Motor_Float_Angle(uint8_t Motor_ID)
{
	return g_encoder_float_angle[Motor_ID - 1];
}

//获取整形电机机械角度,角度值
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID)
{
	return g_encoder_int_angle[Motor_ID - 1];
}

//获取电机转速,单位RPM
int16_t Get_Motor_Speed(uint8_t Motor_ID)
{
	return g_encoder_speed[Motor_ID - 1];
}


//@name: RCS_CANx_Send
//@brief: CAN发送数据
//@param:CAN_TypeDef *CANx	CAN组
//@param:uint32_t Id	发送给对应ID号
//@param:uint8_t Length 发送数据长度
//@param:uint8_t* sendData  数据
void RCS_CANx_Send(CAN_TypeDef *CANx,uint32_t Id, uint8_t Length, uint8_t* sendData)
{
		int i;
    CanTxMsg TxMessage;
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.StdId = Id;
    TxMessage.ExtId = Id;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.DLC = Length;

    for(i = 0;i < Length; i++)
    {
        TxMessage.Data[i] = sendData[i];
    }
    CAN_Transmit(CANx, &TxMessage);
}