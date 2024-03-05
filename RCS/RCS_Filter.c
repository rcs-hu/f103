/*@filename: RCS_Filter.c
 *@author     ���˹�       
 *@brief:    �˲�����ѧ���㺯��
 *@date: 2023-5-21
*/


#include "RCS_Filter.h"

/************��̬ȫ�ֱ���*************/

/*************��̬����****************/

/**************************************/

/**
	@name: KalmanFilter
	@brief:�������˲��㷨������Ӧ�ó�������p q r����,������������ͬһ��������������������и��š�
	@param:p����������ϵͳ���Զ�����
  @param:r���������˲����������ʵ�����ߵ�����̶ȣ�rԽСԽ�ӽ���
	@param:q�������˲��������ƽ���̶ȣ�qԽСԽƽ����
**/
float KalmanFilter_x(float inData)
{
  static float prevData = 0;                                 //��һ������
//  static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;      //���Ʋ���
//	static float p = 1.0f, q = 0.5f, r = 0.01f, kGain = 0; 
	static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;
  p = p + q;
  kGain = p / ( p + r );                                      //���㿨��������
  inData = prevData + ( kGain * ( inData - prevData ) );      //���㱾���˲�����ֵ
  p = ( 1 - kGain ) * p;                                      //���²�������
  prevData = inData;
  return inData;                                             //���ع���ֵ
}
float KalmanFilter_y(float inData)
{
  static float prevData = 0;                                 //��һ������
//  static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;      // q ������� r ������Ӧ�ٶ�
//	static float p = 1.0f, q = 0.5f, r = 0.01f, kGain = 0; 
	static float p = 1.0f, q = 0.001f, r = 0.05f, kGain = 0;
  p = p + q;
  kGain = p / ( p + r );                                      //���㿨��������
  inData = prevData + ( kGain * ( inData - prevData ) );      //���㱾���˲�����ֵ
  p = ( 1 - kGain ) * p;                                      //���²�������
  prevData = inData;
  return inData;                                             //���ع���ֵ
}


/**
	@name: Get_Median
	@brief:������λ���˲�
	@param:float *data          		  ����
	@param:int num            			�����С
	@return	��λ��
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
	@brief:����ƽ��ֵ�˲�
	@param:float *data          		  ����
	@param:int num            			�����С
	@return	ƽ��ֵ
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
	@brief:��Ȩ���ƾ�ֵ�˲�
	@param:float *data          		  ����
	@param:int num            			�����С
	@param:int begin_index          ����ֵ��������λ��
	@return	�˲�ֵ
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
	@brief:��ֵ�˲�
	@param:float *data          		  ����
	@param:int num            			�����С
	@param:int begin_index          ����ֵ��������λ��
	@return	�˲�ֵ
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