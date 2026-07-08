#ifndef __APP_HMI_H__
#define __APP_HMI_H__

#include <stdint.h>

typedef struct {
    uint8_t materialflag;
    uint8_t slopflag;
    uint8_t sta;
    uint8_t warncnt;
    uint8_t warnsta;
    uint16_t timecnt;
} HMI_STRUCT;

void hmi_process_command(uint16_t cmd);
void hmi_update_display(float mean, float variance, float threshold);
void warnTips(float variance, float threshold);

#endif
