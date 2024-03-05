/*@filename: Task.c
 *@author     胡兴国       
 *@brief:    任务执行
 *@date: 2023-5-21
*/

#include "Task.h"
/*--------------------全局变量---------------------*/
static int step = 0;   //步骤
static int i=0;

/**
	@name: Task1
	@brief:	任务1
**/
void Task1(void)
{

		switch(step)
	{
		case 0: i = 0;
			      if(Count_Delay(21))
						{
							step ++;
						}
						break;	
					
		case 1: i = 1;
			      step ++;
            break;

    case 2: i = 2;
						break;	
						
		default:i = 3;
	}						
	
}
//            if(Count_Delay(600))   //反复执行600次任务
//						{
//							step ++;
//						}
//						break;