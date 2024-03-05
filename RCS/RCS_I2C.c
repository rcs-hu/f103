/*@filename: RCS_I2C.c
 *@author     ���˹�       
 *@brief:     I2CͨѶЭ�飬��MPU6050Ϊ��
 *@date: 2023-5-7
*/

#include "RCS_I2C.h"       

//��ʱ��������ֹ������
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}

/**
	@name: MPU6050_WriteReg
	@brief: I2Cд����
**/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);  //�ӻ���ַ
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

/**
	@name: MPU6050_ReadReg
	@brief: I2C������
**/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	//I2C��������
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);  //�ӻ���ַ
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
}

/**
	@name: MPU6050_Init
	@brief: I2C2��ʼ��
**/
void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //����I2C2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;    //���ÿ�©���ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��ʼ��I2C
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;  //I2Cʱ���ٶ�
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  //�͵�ƽ��ߵ�ƽ��ʱ�����2��1
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;  //��Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //��Ӧ7λ��ַ
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;  //stm32�����ĵ�ַ
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

/**
	@name: MPU6050_GetID
	@brief: ����MPU6050�ĵ�ַ��68��
**/
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
	@name: MPU6050_GetData
	@brief: ������ֵ��Acc�Ǽ��ٶ�ֵ��Gyro�ǽ��ٶ�ֵ
**/
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}


////������ʹ��ʾ��
//uint8_t ID;
//int16_t AX, AY, AZ, GX, GY, GZ;

//int main(void)
//{
//	OLED_Init();
//	MPU6050_Init();
//	
//	OLED_ShowString(1, 1, "ID:");
//	ID = MPU6050_GetID();
//	OLED_ShowHexNum(1, 4, ID, 2);
//	
//	while (1)
//	{
//		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
//		OLED_ShowSignedNum(2, 1, AX, 5);
//		OLED_ShowSignedNum(3, 1, AY, 5);
//		OLED_ShowSignedNum(4, 1, AZ, 5);
//		OLED_ShowSignedNum(2, 8, GX, 5);
//		OLED_ShowSignedNum(3, 8, GY, 5);
//		OLED_ShowSignedNum(4, 8, GZ, 5);
//	}
//}