#include "drv_usart.h"
#include "board.h"

USART_Data_Struct USART2_Data = {0};
USART_Data_Struct USART3_Data = {0};

void USART_Send_Byte(USART_TypeDef* USARTx, uint8_t byte)
{

    while ((USARTx->SR & USART_SR_TC) == 0);
    USARTx->DR = byte;
}

void USART_Send_Data(USART_TypeDef* USARTx, uint8_t* data, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        USART_Send_Byte(USARTx, data[i]);
    }
    while ((USARTx->SR & USART_SR_TC) == 0);
}

void USART_Send_String(USART_TypeDef* USARTx, uint8_t* str)
{
    while (*str != '\0') {
        USART_Send_Byte(USARTx, *str);
        str++;
    }
    while ((USARTx->SR & USART_SR_TC) == 0);
}
