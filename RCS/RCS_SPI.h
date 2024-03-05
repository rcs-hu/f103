/*@filename: RCS_SPI.h
 *@author     บ๚ะหน๚       
 *@brief:     SPI
 *@date: 2023-5-29
*/
#ifndef _RCS_SPI_H_
#define _RCS_SPI_H_

#include "rcs.h"

void SPI1_Init(void);
void SPI_Start(void);
void SPI_Stop(void);
uint8_t SPI_SwapByte(uint8_t ByteSend);

#endif
