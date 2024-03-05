/*@filename:  Filter.h
 *@author     ºúĞË¹ú       
 *@brief:     ÂË²¨Ëã·¨
 *@date: 2023-5-21
*/

#ifndef _RCS_Filter_H_
#define _RCS_Filter_H_

#include "rcs.h"

float KalmanFilter_x(float inData);
float KalmanFilter_y(float inData);
float Get_Median(float *data,int num);
float Get_Average(float data[],int num);
float Recursive_Filter(float data[],int num,int begin_index);
float Median_Ave_Filter(float data[],int num);

#endif
