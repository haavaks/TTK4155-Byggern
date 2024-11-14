#pragma once

#define ADC_MAX_VALUE 4096
#define ADC_THRESHOLD 167 //135 mv
#define ADC_AVG_THRESHOLD 500
#define AVG_WINDOW_SIZE 10

void adc_init();
bool ADC_game_over();
void reset_ADC_isr();