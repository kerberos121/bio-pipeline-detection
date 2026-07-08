#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
    uint8_t recdata[64];
    uint8_t senddata[64];
    uint8_t reclen;
    uint8_t sendlen;
    uint8_t recendflag;
} USART_Data_Struct;

extern USART_Data_Struct USART2_Data;
extern USART_Data_Struct USART3_Data;

void USART_Send_Byte(USART_TypeDef* USARTx, uint8_t byte);
void USART_Send_Data(USART_TypeDef* USARTx, uint8_t* data, uint8_t len);
void USART_Send_String(USART_TypeDef* USARTx, uint8_t* str);

#endif
