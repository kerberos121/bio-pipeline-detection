#ifndef __BSP_AD9220_H__
#define __BSP_AD9220_H__

#include <stdint.h>

void AD9220_IO_Init(void);
uint16_t AD9220ReadData(long Freq);

#endif
