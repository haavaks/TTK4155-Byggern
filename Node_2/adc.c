#include "sam.h"
#include "adc.h"

volatile int ADC_isr = 0;

void ADC_Handler(void)
{
    if ((ADC->ADC_ISR & ADC_ISR_COMPE))
    {
        //printf("Below threshold\n\r");
        if (ADC_isr == 0)
        {
            ADC_isr = 1;
        }
    }
}

void adc_init()
{
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Enable clock for ADC
    REG_ADC_CHER |= ADC_CHER_CH5;      // Enables channel 5 for analog input
    REG_ADC_MR |= ADC_MR_FREERUN_ON;   // ADC on freerun mode
    REG_ADC_MR |= ADC_MR_PRESCAL(15);  // Not sure, but we think we set ADC clock = MCK / 32. See page 1334 in Atsam datasheet

    REG_ADC_EMR |= ADC_EMR_CMPSEL(5); // Enables comparison on channel 5
    REG_ADC_EMR |= ADC_EMR_CMPMODE_LOW;
    REG_ADC_CWR |= ADC_CWR_LOWTHRES(ADC_THRESHOLD); // Comparison value
    REG_ADC_IER |= ADC_IER_COMPE;                   // Enables comparison interrupts

    NVIC_EnableIRQ(ADC_IRQn);

    REG_ADC_CR |= ADC_CR_START;
}

bool ADC_game_over()
{
    return ADC_isr;
}


void reset_ADC_isr()
{
    ADC_isr = 0;
    //printf("%d\n\r", ADC_isr);
}