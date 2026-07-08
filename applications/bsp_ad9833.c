#include "bsp_ad9833.h"
#include "board.h"

#define TRI_WAVE    0
#define SIN_WAVE    1
#define SQU_WAVE    2

static void AD9833_Delay(void)
{
    for (int i = 0; i < 10; i++);
}

void AD9833_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    AD9833_CLK_ENABLE();

    GPIO_InitStruct.Pin = AD9833_FSYNC_PIN | AD9833_SCK_PIN | AD9833_DATA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AD9833_PORT, &GPIO_InitStruct);

    AD9833_FSYNC_HIGH();
    AD9833_SCK_HIGH();
    AD9833_DATA_HIGH();
}

void AD9833_Write(uint16_t TxData)
{
    AD9833_SCK_HIGH();
    AD9833_Delay();
    AD9833_FSYNC_HIGH();
    AD9833_Delay();
    AD9833_FSYNC_LOW();
    AD9833_Delay();

    for (int i = 0; i < 16; i++) {
        if (TxData & 0x8000)
            AD9833_DATA_HIGH();
        else
            AD9833_DATA_LOW();

        AD9833_Delay();
        AD9833_SCK_LOW();
        AD9833_Delay();
        AD9833_SCK_HIGH();

        TxData <<= 1;
    }

    AD9833_FSYNC_HIGH();
}

void AD9833_WaveSeting(float Freq, uint16_t Freq_SFR, uint16_t WaveMode, uint16_t Phase)
{
    int frequence_LSB, frequence_MSB;
    float frequence_mid;
    long int frequence_hex;

    frequence_mid = 268435456.0f / 25.0f;
    frequence_hex = (long int)((Freq / 1000000.0f) * frequence_mid);

    frequence_LSB = frequence_hex & 0x3FFF;
    frequence_MSB = (frequence_hex >> 14) & 0x3FFF;

    AD9833_Write(0x0100);
    AD9833_Write(0x2100);

    if (Freq_SFR == 0) {
        AD9833_Write(frequence_LSB | 0x4000);
        AD9833_Write(frequence_MSB | 0x4000);
        AD9833_Write(Phase | 0xC000);
        AD9833_Write(0x2000);
    } else {
        AD9833_Write(frequence_LSB | 0x8000);
        AD9833_Write(frequence_MSB | 0x8000);
        AD9833_Write(Phase | 0xC000);
        AD9833_Write(0x2800);
    }

    if (WaveMode == TRI_WAVE)
        AD9833_Write(0x2002);
    else if (WaveMode == SQU_WAVE)
        AD9833_Write(0x2028);
    else
        AD9833_Write(0x2000);
}
