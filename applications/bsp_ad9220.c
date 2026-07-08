#include "bsp_ad9220.h"
#include "board.h"

void AD9220_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    AD9220_DATA_CLK_ENABLE();
    AD9220_CLK_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                          GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
                          GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AD9220_CLK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD9220_CLK_PORT, &GPIO_InitStruct);

    AD9220_CLK_LOW();
}

uint16_t AD9220ReadData(long Freq)
{
    uint16_t data;
    (void)Freq;

    AD9220_CLK_LOW();
    data = (uint16_t)(GPIOD->IDR & 0x1FFF);
    data = ((data << 1) & 0x1FFF) | (data >> 12);
    AD9220_CLK_HIGH();

    return (data & 0x0FFF);
}
