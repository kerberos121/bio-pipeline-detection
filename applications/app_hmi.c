#include "app_hmi.h"
#include "board.h"
#include "bsp_led_beep.h"
#include "app_flash.h"
#include <stdio.h>
#include <string.h>

static HMI_STRUCT hmiData = {0};
static float min_variance = 9999999.0f;
static uint8_t alarm_sent_flag = 0;

uint8_t g_alarm_triggered = 0;
uint8_t g_threshold_updated = 0;
float gmean = 0.0f;
float gvariance = 0.0f;
float variance_threshold = 550000.0f;

extern USART_Data_Struct USART2_Data;
extern USART_Data_Struct USART3_Data;

void hmi_process_command(uint16_t cmd)
{
    uint8_t str[40];
    uint8_t alarm_data[] = {0x01, 0x03, 0x00, 0x01};
    static HMI_STRUCT hmiData = {0};
    static float min_variance = 9999999.0f;
    static uint8_t alarm_sent_flag = 0;

    (void)alarm_data;
    (void)alarm_sent_flag;
    (void)min_variance;

    switch (cmd) {
        case 0x1001:
            USART_Send_String(USART2, (uint8_t*)"page page2\xff\xff\xff");
            break;
        case 0x1002:
            USART_Send_String(USART2, (uint8_t*)"page page4\xff\xff\xff");
            WARN_OFF;
            LED3_OFF;
            break;
        case 0x2001:
            USART_Send_String(USART2, (uint8_t*)"page page2\xff\xff\xff");
            break;
        case 0x2002:
            USART_Send_String(USART2, (uint8_t*)"page page4\xff\xff\xff");
            WARN_OFF;
            LED3_OFF;
            break;
        case 0x3001:
            USART_Send_String(USART2, (uint8_t*)"page page1\xff\xff\xff");
            break;
        case 0x3002:
            USART_Send_String(USART2, (uint8_t*)"page page5\xff\xff\xff");
            sprintf((char*)str, "page5.t7.txt=\"%10.1f\"\xff\xff\xff", variance_threshold);
            USART_Send_String(USART2, str);
            break;
        case 0x3003:
            USART_Send_String(USART2, (uint8_t*)"page page1\xff\xff\xff");
            break;
        case 0x3004:
            USART_Send_String(USART2, (uint8_t*)"page page1\xff\xff\xff");
            WARN_OFF;
            LED3_OFF;
            break;
        default:
            break;
    }
}

void hmi_update_display(float mean, float variance, float threshold)
{
    uint8_t str[40];
    static HMI_STRUCT hmiData = {0};

    (void)hmiData;

    if (hmiData.sta >= 3) {
        sprintf((char*)str, "page5.t2.txt=\"%7.1f\"\xff\xff\xff", mean);
        USART_Send_String(USART2, str);

        sprintf((char*)str, "page5.t3.txt=\"%10.1f\"\xff\xff\xff", variance);
        USART_Send_String(USART2, str);

        sprintf((char*)str, "page5.t7.txt=\"%10.1f\"\xff\xff\xff", threshold);
        USART_Send_String(USART2, str);
    }
}

void warnTips(float variance, float threshold)
{
    static HMI_STRUCT hmiData = {0};

    (void)hmiData;

    if (variance > threshold) {
        WARN_ON;
        LED3_ON;
    } else {
        WARN_OFF;
        LED3_OFF;
    }
}
