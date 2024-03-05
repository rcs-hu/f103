/*@filename: RCS_Filter.c
 *@author     胡兴国       
 *@brief:    滤波及数学运算函数
 *@date: 2023-5-21
*/


#include "RCS_Filter.h"

/************静态全局变量*************/

/*************静态函数****************/

/**************************************/

/**
	@name: KalmanFilter
	@brief:卡尔曼滤波算法，根据应用场景调节p q r参数,两个任务不能用同一个卡尔曼函数，否则会有干扰。
	@param:p参数随便给，系统会自动修正
  @param:r参数调整滤波后的曲线与实测曲线的相近程度，r越小越接近。
	@param:q参数调滤波后的曲线平滑程度，q越小越平滑。
**/
float KalmanFilter_x(float inData)
{
  static float prevData = 0;                                 //上一个数据
//  static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;      //华科参数
//	static float p = 1.0f, q = 0.5f, r = 0.01f, kGain = 0; 
	static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;
  p = p + q;
  kGain = p / ( p + r );                                      //计算卡尔曼增益
  inData = prevData + ( kGain * ( inData - prevData ) );      //计算本次滤波估计值
  p = ( 1 - kGain ) * p;                                      //更新测量方差
  prevData = inData;
  return inData;                                             //返回估计值
}
float KalmanFilter_y(float inData)
{
  static float prevData = 0;                                 //上一个数据
//  static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;      // q 控制误差 r 控制响应速度
//	static float p = 1.0f, q = 0.5f, r = 0.01f, kGain = 0; 
	static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;
  p = p + q;
  kGain = p / ( p + r );                                      //计算卡尔曼增益
  inData = prevData + ( kGain * ( inData - prevData ) );      //计算本次滤波估计值
  p = ( 1 - kGain ) * p;                                      //更新测量方差
  prevData = inData;
  return inData;                                             //返回估计值
}


/**
	@name: Get_Median
	@brief:队列中位数滤波
	@param:float *data          		  数组
	@param:int num            			数组大小
	@return	中位数
**/
float Get_Median(float data[],int num)
{
	float buf[num];
	float temp;
	
	for(int i =0;i<num;i++)
		buf[i] = data[i];
	
	for(int i =0;i<=num-2;i++)
	{
		for(int j = 0;j<=num-2;j++)
		{
			if(buf[j] > buf[j+1])
			{
				temp = buf[j];
				buf[j] = buf[j+1];
				buf[j+1] = temp;
			}
		}
	}
	
	if(num%2 == 1)
		return buf[(num-1)/2];
	else
		return ((buf[num/2-1] + buf[num/2])/2.0f);
}


/**
	@name: Get_Average
	@brief:队列平均值滤波
	@param:float *data          		  数组
	@param:int num            			数组大小
	@return	平均值
**/
float Get_Average(float data[],int num)
{
	float sum = 0;
	for(int i=0;i<num;i++)
		sum += data[i];
	return sum/((float)num);
}

/**
	@name: Recursive_Filter
	@brief:加权递推均值滤波
	@param:float *data          		  数组
	@param:int num            			数组大小
	@param:int begin_index          最新值在数组中位置
	@return	滤波值
**/
float Recursive_Filter(float data[],int num,int begin_index)
{
	int index;
	float sum = 0;
	float sum_i = 0;
	index = begin_index;

	for(int i=num; i>0 ; i--)
	{
		sum += data[index] * i;
		sum_i += i;
		index --;
		if(index <0)
			index = num-1;
	}
	
	sum /= sum_i;
	return sum;
}


/**
	@name: Recursive_Filter
	@brief:中值滤波
	@param:float *data          		  数组
	@param:int num            			数组大小
	@param:int begin_index          最新值在数组中位置
	@return	滤波值
**/
float Median_Ave_Filter(float data[],int num)
{
	int max_num,min_num;
	float sum;
	max_num = data[0];
	min_num = data[0];
	sum = data[0];
	for(int i =1;i<num;i++)
	{
		sum += data[i];
		if(data[i] > max_num)
			max_num = data[i];
		if(data[i] < min_num)
			min_num = data[i];
	}
	sum = (sum - max_num - min_num)/(num-2);
	return sum;
}