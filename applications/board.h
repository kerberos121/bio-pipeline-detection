#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "stm32f4xx_hal.h"

void SystemClock_Config(void);

#define LED1_PIN                 GPIO_PIN_6
#define LED1_GPIO_PORT           GPIOF
#define LED1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()

#define LED2_PIN                 GPIO_PIN_7
#define LED2_GPIO_PORT           GPIOF
#define LED2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()

#define LED3_PIN                 GPIO_PIN_8
#define LED3_GPIO_PORT           GPIOF
#define LED3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE()

#define LED1_ON                  HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET)
#define LED1_OFF                 HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET)
#define LED1_TOGGLE              HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN)

#define LED2_ON                  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET)
#define LED2_OFF                 HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET)
#define LED2_TOGGLE              HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN)

#define LED3_ON                  HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET)
#define LED3_OFF                 HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET)
#define LED3_TOGGLE              HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN)

#define BEEP_PIN                 GPIO_PIN_2
#define BEEP_PORT                GPIOE
#define BEEP_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()

#define WARN_ON                  HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET)
#define WARN_OFF                 HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET)

#define AD9220_DATA_PORT         GPIOD
#define AD9220_DATA_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define AD9220_CLK_PIN           GPIO_PIN_7
#define AD9220_CLK_PORT          GPIOE
#define AD9220_CLK_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()

#define AD9220_CLK_HIGH()        HAL_GPIO_WritePin(AD9220_CLK_PORT, AD9220_CLK_PIN, GPIO_PIN_SET)
#define AD9220_CLK_LOW()         HAL_GPIO_WritePin(AD9220_CLK_PORT, AD9220_CLK_PIN, GPIO_PIN_RESET)

#define AD9833_FSYNC_PIN         GPIO_PIN_12
#define AD9833_SCK_PIN           GPIO_PIN_11
#define AD9833_DATA_PIN          GPIO_PIN_10
#define AD9833_PORT              GPIOG
#define AD9833_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()

#define AD9833_FSYNC_HIGH()      HAL_GPIO_WritePin(AD9833_PORT, AD9833_FSYNC_PIN, GPIO_PIN_SET)
#define AD9833_FSYNC_LOW()       HAL_GPIO_WritePin(AD9833_PORT, AD9833_FSYNC_PIN, GPIO_PIN_RESET)
#define AD9833_SCK_HIGH()        HAL_GPIO_WritePin(AD9833_PORT, AD9833_SCK_PIN, GPIO_PIN_SET)
#define AD9833_SCK_LOW()         HAL_GPIO_WritePin(AD9833_PORT, AD9833_SCK_PIN, GPIO_PIN_RESET)
#define AD9833_DATA_HIGH()       HAL_GPIO_WritePin(AD9833_PORT, AD9833_DATA_PIN, GPIO_PIN_SET)
#define AD9833_DATA_LOW()        HAL_GPIO_WritePin(AD9833_PORT, AD9833_DATA_PIN, GPIO_PIN_RESET)

#define STM32_FLASH_SAVE_ADDR    0x080E0000

extern float gmean;
extern float gvariance;
extern float variance_threshold;
extern uint8_t g_alarm_triggered;
extern uint8_t g_threshold_updated;

#include "drv_usart.h"

#endif /* __BOARD_H__ */
