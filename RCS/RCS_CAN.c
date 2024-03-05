/*@filename: RCS_CAN.c
 *@author     ���˹�       
 *@brief:     canͨѶ����Ҫ���ڴ�3508��
 *@date: 2023-5-2
*/

#include "RCS_CAN.h"

/***************ȫ�ֱ���*****************/
static float g_encoder_start_angle[8];
static uint16_t g_encoder_oringin_angle[8];
static uint16_t g_encoder_last_angle[8];
static int16_t g_encoder_cycles[8];
volatile float g_encoder_float_angle[8];
volatile int32_t g_encoder_int_angle[8];
static int16_t g_encoder_oringin_speed[8];
volatile int16_t g_encoder_speed[8];
/****************************************/

//�����ʼ��
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
	
	RCS_CAN1_Init();  //CAN1���ų�ʼ��
	delay_ms(10);
	for (int i = 0; i < 8; i++)
		g_encoder_start_angle[i] = g_encoder_float_angle[i];
}

//@name: RCS_CAN1_Init
//@brief: ����CAN1��ͨ�Ź���
//@param: _baudRate  ���ò����ʣ���������������3000000��
void RCS_CAN1_Init(void)
{	
	CAN_InitTypeDef CAN_InitStructure; //can��ʼ���ṹ��
	CAN_FilterInitTypeDef CAN_FilterInitStructure; //��������ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����		
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ�� CAN1 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CAN1_TXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CAN1_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//can��ʼ���ṹ������
	CAN_InitStructure.CAN_TTCM=DISABLE; //��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM=ENABLE; //����Զ����߹���
	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR �� SLEEP λ)
	CAN_InitStructure.CAN_NART=ENABLE; //ʹ�ñ����Զ�����
	CAN_InitStructure.CAN_RFLM=DISABLE; //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP=ENABLE; //���ȼ��ɱ��ı�ʶ������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //ģʽ����
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //��ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq-CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq; //Tbs1��Χ CAN_BS1_1tq-CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;//Tbs2��ΧCAN_BS2_1tq -CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = 4; //1M (baudrate = pclk1/(1+bs1+bs2)/psc)
	CAN_Init(CAN1, &CAN_InitStructure); // ��ʼ�� CAN1

	//��������ʼ���ṹ������
	CAN_FilterInitStructure.CAN_FilterNumber=0; //������ 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32 λ
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32 λ ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32 λ MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������ 0 ������ FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

	//�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
		
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	
	
}


//@name: Motor_Send
//@brief: ��3508�ĵ�����Ϳ�������
//@param:ID1_Current ���IDΪ1�ĵ�������
//@param:ID2_Current ���IDΪ2�ĵ�������
//@param:ID3_Current ���IDΪ3�ĵ�������
//@param:ID4_Current ���IDΪ4�ĵ�������
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
//@brief: ��3508�ĵ�����Ϳ�������
//@param:ID1_Current ���IDΪ5�ĵ�������
//@param:ID2_Current ���IDΪ6�ĵ�������
//@param:ID3_Current ���IDΪ7�ĵ�������
//@param:ID4_Current ���IDΪ8�ĵ�������
void Motor_Send_ADD(int16_t ID5_Current, int16_t ID6_Current, int16_t ID7_Current, int16_t ID8_Current)
{
	uint8_t current_data[8];

	Current_Data_Takepart(ID5_Current, &current_data[0], &current_data[1]);
	Current_Data_Takepart(ID6_Current, &current_data[2], &current_data[3]);
	Current_Data_Takepart(ID7_Current, &current_data[4], &current_data[5]);
	Current_Data_Takepart(ID8_Current, &current_data[6], &current_data[7]);        
	RCS_CANx_Send(CAN1,0x1FF, 0x08, current_data);
}


//CAN1�����ж�
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;

	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

		if ((rx_message.IDE == CAN_Id_Standard) && (rx_message.RTR == CAN_RTR_Data) && (rx_message.DLC == 0x08)) //��׼֡������֡����Ϊ8
		{
			//����Ž���
			uint8_t motor_number = (rx_message.StdId & 0x000f);

			//�ٶ����ݽ���
			g_encoder_oringin_speed[motor_number - 1] = (rx_message.Data[2] << 8) | (rx_message.Data[3]);
			if (g_encoder_oringin_speed[motor_number - 1] <= 3 && g_encoder_oringin_speed[motor_number - 1] >= -3)
				g_encoder_speed[motor_number - 1] = 0;
			else
				g_encoder_speed[motor_number - 1] = g_encoder_oringin_speed[motor_number - 1];

			//λ�����ݽ���
			g_encoder_oringin_angle[motor_number - 1] = (rx_message.Data[0] << 8) | (rx_message.Data[1]);
			//��0�����ж�
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

//��ȡCAN1��������е�Ƕ�,�Ƕ�ֵ������ʱ�Ƕ�Ϊ0
float Get_Motor_Float_Angle(uint8_t Motor_ID)
{
	return g_encoder_float_angle[Motor_ID - 1];
}

//��ȡ���ε����е�Ƕ�,�Ƕ�ֵ
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID)
{
	return g_encoder_int_angle[Motor_ID - 1];
}

//��ȡ���ת��,��λRPM
int16_t Get_Motor_Speed(uint8_t Motor_ID)
{
	return g_encoder_speed[Motor_ID - 1];
}


//@name: RCS_CANx_Send
//@brief: CAN��������
//@param:CAN_TypeDef *CANx	CAN��
//@param:uint32_t Id	���͸���ӦID��
//@param:uint8_t Length �������ݳ���
//@param:uint8_t* sendData  ����
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