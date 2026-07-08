#ifndef __BSP_AD9833_H__
#define __BSP_AD9833_H__

#include <stdint.h>

#define TRI_WAVE    0
#define SIN_WAVE    1
#define SQU_WAVE    2

void AD9833_Init(void);
void AD9833_Write(uint16_t TxData);
void AD9833_WaveSeting(float Freq, uint16_t Freq_SFR, uint16_t WaveMode, uint16_t Phase);

#endif
