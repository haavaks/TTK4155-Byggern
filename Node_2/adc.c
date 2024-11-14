#include "sam.h"
#include "adc.h"

volatile uint16_t adc_buffer[AVG_WINDOW_SIZE];
volatile uint8_t buffer_index = 0;
volatile uint32_t adc_sum = 0; 
volatile int ADC_isr = 0;

void ADC_Handler(void)
{
    // uint32_t adc_value = REG_ADC_LCDR;

    // adc_sum -= adc_buffer[buffer_index];
    // adc_buffer[buffer_index] = adc_value;
    // adc_sum += adc_value;
    // buffer_index = (buffer_index + 1) % AVG_WINDOW_SIZE;

    // uint16_t avg_adc_value = adc_sum / AVG_WINDOW_SIZE;
    // printf("Avg adc value: %d\n\r", avg_adc_value);

    // if (avg_adc_value < ADC_AVG_THRESHOLD){
    //     //printf("INTERRUPT\n\r");
    //     if (ADC_isr == 0)
    //     {
            
    //         ADC_isr = 1;
    //         printf("ISR: %d \r\n",ADC_isr);
    //     }
    // }

    if ((ADC->ADC_ISR & ADC_ISR_COMPE))
    {
        
        if (ADC_isr == 0)
        {
            printf("INTERRUPT\n\r");
            ADC_isr = 1;
        }
    }
    
    NVIC_ClearPendingIRQ(ID_ADC);
}

void adc_init()
{
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Enable clock for ADC
    REG_ADC_CHER |= ADC_CHER_CH13;      // Enables channel 5 for analog input
    REG_ADC_MR |= ADC_MR_FREERUN_ON;   // ADC on freerun mode
    REG_ADC_MR |= ADC_MR_PRESCAL(255);  // Not sure, but we think we set ADC clock = MCK / ((PRESCAL + 1)*2). See page 1334 in Atsam datasheet

    REG_ADC_EMR |= ADC_EMR_CMPSEL(13); // Enables comparison on channel 5
    REG_ADC_EMR |= ADC_EMR_CMPMODE_LOW;
    REG_ADC_CWR |= ADC_CWR_LOWTHRES(ADC_THRESHOLD); // Comparison value
    REG_ADC_IER |= ADC_IER_COMPE;                   // Enables comparison interrupts

    //REG_ADC_EMR |= ADC_EMR_CMPFILTER(0); // Filter; number of consecutive compare events nessecary + 1

    // for (uint8_t i = 0; i < AVG_WINDOW_SIZE; i++){
    //     adc_buffer[i] = ADC_MAX_VALUE;
    //     adc_sum = adc_sum + ADC_MAX_VALUE;
    // }

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
}

