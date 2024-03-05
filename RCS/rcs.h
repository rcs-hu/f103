//@filename: rcs.h
//@date: 2023-4-25
//@author: 胡兴国
//@brief: rcs.h文件

#ifndef _RCS_H_
#define _RCS_H_

#include "stm32f10x.h"

#include <stdio.h>	
#include <stdlib.h>	
#include <string.h>	
#include <stdint.h>	
#include <math.h>


/********* RCS功能库 *********/
#include "Delay.h"
#include "RCS_OLED.h"
#include "RCS_GPIO.h"
#include "RCS_Pwm.h"
#include "RCS_Timer.h"
#include "RCS_Exti.h"
#include "RCS_Encoder.h"
#include "RCS_PIDctrl.h"
#include "RCS_Usart.h"
#include "RCS_Gyro.h"
#include "RCS_CAN.h"
#include "RCS_ADC.h"
#include "RCS_Ultrasonic.h" 
#include "RCS_I2C.h"
#include "RCS_Bluetooth.h" 
#include "RCS_Filter.h"
#include "Task.h"
#include "RCS_SPI.h"
#include "RCS_BaseMove.h"

/*
常用常数定义
 */
#define pi 							3.14159265357f//π
#define DEG2RAD         1.74532925199433e-2f            //角度变换成弧度     
#define RAD2DEG         5.72957795130823e1f		////弧度变换成角度
#define FLOAT_ZERO      1e-10f
#define G                 9.8f                           //重力加速度
#define ENCODER2ANGLE	0.0439453125		//电机参数


/***************************管脚定义*****************************/
/*
OLED显示屏
 */
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*
陀螺仪
*/
#define GYRO_USART				USART1			
#define GYRO_GPIO				GPIOA	
#define GYRO_TX					GPIO_Pin_9	
#define GYRO_RX					GPIO_Pin_10
#define GYRO_BAUD				115200			
#define GYRO_PRI                0x10

/*
蓝牙串口
*/
#define BLUETOOTH_USART			USART2  				
#define BLUETOOTH_GPIO			GPIOA      				
#define BLUETOOTH_TX		    GPIO_Pin_2			
#define BLUETOOTH_RX			GPIO_Pin_3		
#define BLUETOOTH_BAUD			9600							
#define BLUETOOTH_PRI           0x01					


/*
CAN总线硬件接口
*/
#define CAN1_GPIO     GPIOA
#define CAN1_TXD      GPIO_Pin_12
#define CAN1_RXD      GPIO_Pin_11
#define CAN1_BAUD     1000000


/*
超声波测距接口
*/
#define ULTR_GPIO     GPIOA
#define ULTR_TRIG     GPIO_Pin_9  //输出高电平引脚
#define ULTR_ECHO     GPIO_Pin_10  //输入测距引脚
#define ULTR_TIM      TIM4         //定时器    


#endif

