#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include <inttypes.h>

enum ADC_Channels_t
{
    CH_y = 0b10000000,
    CH_x = 0b10000001,
    CH_ls = 0b10000010,
    CH_rs = 0b10000011
};

void ADC_init(void);

int16_t ADC_read(enum ADC_Channels_t c);

#endif // ADC_H