/*@filename: main.c
 *@author     ���˹�       
 *@brief:     main����
 *@date: 2023-4-28
*/

#include "rcs.h"   

int main(void)
{
	OLED_Init();	
	while (1)
	{
		OLED_ShowHexNum(1, 7, 0x01, 4);
		delay_ms(5);
	}
}
