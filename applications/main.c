#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "bsp_ad9220.h"
#include "bsp_ad9833.h"
#include "bsp_led_beep.h"
#include "vmd_algorithm.h"
#include "app_hmi.h"
#include "app_flash.h"
#include "drv_usart.h"

static rt_thread_t tid_adc_vmd = RT_NULL;
static rt_thread_t tid_hmi = RT_NULL;
static rt_thread_t tid_rs485 = RT_NULL;
static rt_thread_t tid_monitor = RT_NULL;

extern float gmean, gvariance, variance_threshold;
extern uint8_t g_alarm_triggered, g_threshold_updated;
extern USART_Data_Struct USART2_Data;
extern USART_Data_Struct USART3_Data;

extern void MX_GPIO_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);

static void thread_adc_vmd_entry(void *parameter);
static void thread_hmi_entry(void *parameter);
static void thread_rs485_entry(void *parameter);
static void thread_monitor_entry(void *parameter);

int main(void)
{
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();

    AD9833_Init();
    AD9833_WaveSeting(200000.0f, 0, SIN_WAVE, 0);
    AD9220_IO_Init();

    LED_GPIO_Config();
    BEEP_GPIO_Config();
    LED3_OFF;
    WARN_OFF;

    STMFLASH_Read(STM32_FLASH_SAVE_ADDR, (uint32_t*)&variance_threshold, 1);
    if (variance_threshold < 1000 || variance_threshold > 10000000)
    {
        variance_threshold = 550000.0f;
    }

    tid_adc_vmd = rt_thread_create("adc_vmd", thread_adc_vmd_entry, RT_NULL, 4096, 8, 10);
    if (tid_adc_vmd != RT_NULL) rt_thread_startup(tid_adc_vmd);

    tid_hmi = rt_thread_create("hmi", thread_hmi_entry, RT_NULL, 2048, 10, 10);
    if (tid_hmi != RT_NULL) rt_thread_startup(tid_hmi);

    tid_rs485 = rt_thread_create("rs485", thread_rs485_entry, RT_NULL, 1024, 12, 10);
    if (tid_rs485 != RT_NULL) rt_thread_startup(tid_rs485);

    tid_monitor = rt_thread_create("monitor", thread_monitor_entry, RT_NULL, 2048, 14, 10);
    if (tid_monitor != RT_NULL) rt_thread_startup(tid_monitor);

    USART_Send_String(USART2, (uint8_t*)"page page0\xff\xff\xff");

    rt_kprintf("=== Bio-pipeline Inspection System Started ===\n");
    rt_kprintf("Threshold: %.1f\n", variance_threshold);

    return 0;
}

static void thread_adc_vmd_entry(void *parameter)
{
    uint16_t ad_data;
    int sample_vol[128] = {0};
    float input_signal[128] = {0};
    float reconstructed[128] = {0};
    float mean_val, var_val;

    while (1)
    {
        rt_thread_mdelay(1000);

        for (int t = 0; t < 128; t++)
        {
            ad_data = AD9220ReadData(10000000);
            sample_vol[t] = (int32_t)(5000.0f * ((float)((int16_t)ad_data) / 4096.0f));
            input_signal[t] = (float)sample_vol[t];
        }

        initialize_vmd();
        update_modes(input_signal);
        calculate_energy();
        filter_modes_by_energy();
        output_results(reconstructed);

        mean_val = calculate_mean(reconstructed, 128);
        var_val = calculate_variance(reconstructed, mean_val, 128);

        gmean = mean_val;
        gvariance = var_val;

        warnTips(var_val, variance_threshold);
        LED1_TOGGLE;
    }
}

static void thread_hmi_entry(void *parameter)
{
    uint16_t recdata;

    while (1)
    {
        if (USART2_Data.recendflag == 1)
        {
            USART2_Data.recendflag = 0;
            if (USART2_Data.recdata[0] == 0x5A && USART2_Data.recdata[1] == 0xA5 && USART2_Data.recdata[4] == 0x0A)
            {
                recdata = (USART2_Data.recdata[2] << 8) | USART2_Data.recdata[3];
                hmi_process_command(recdata);
            }
        }
        hmi_update_display(gmean, gvariance, variance_threshold);
        rt_thread_mdelay(50);
    }
}

static void thread_rs485_entry(void *parameter)
{
    uint8_t alarm_data[] = {0x01, 0x03, 0x00, 0x01};

    while (1)
    {
        if (USART3_Data.recendflag == 1)
        {
            USART3_Data.recendflag = 0;
            USART3_Data.recdata[0] += 1;
            USART_Send_Data(USART3, USART3_Data.recdata, USART3_Data.reclen);
        }
        if (g_alarm_triggered)
        {
            USART_Send_Data(USART3, alarm_data, 4);
            g_alarm_triggered = 0;
        }
        rt_thread_mdelay(20);
    }
}

static void thread_monitor_entry(void *parameter)
{
    uint32_t save_cnt = 0;

    while (1)
    {
        LED3_TOGGLE;
        save_cnt++;
        if (save_cnt >= 60)
        {
            save_cnt = 0;
            if (g_threshold_updated)
            {
                STMFLASH_Write(STM32_FLASH_SAVE_ADDR, (uint32_t*)&variance_threshold, 1);
                g_threshold_updated = 0;
                rt_kprintf("Threshold saved to Flash: %.1f\n", variance_threshold);
            }
        }
        rt_thread_mdelay(1000);
    }
}
