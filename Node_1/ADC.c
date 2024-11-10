#include "ADC.h"

volatile char *adc = (char *)0x1400;

void ADC_init(void)
{
    DDRD |= (1 << 5); // klokkesignal til ADC

    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS10);
}

int16_t ADC_read(enum ADC_Channels_t c)
{
    adc[0] = c;

    _delay_us(30);

    return (int16_t)adc[0];
}