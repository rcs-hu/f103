/*@filename: Task.c
 *@author     ���˹�       
 *@brief:    ����ִ��
 *@date: 2023-5-21
*/

#include "Task.h"
/*--------------------ȫ�ֱ���---------------------*/
static int step = 0;   //����
static int i=0;

/**
	@name: Task1
	@brief:	����1
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
//            if(Count_Delay(600))   //����ִ��600������
//						{
//							step ++;
//						}
//						break;