#include "PWM.h"

volatile struct Joystick_pos_t joystick = {0};

void PWM_Handler(void)
{
    if (PWM->PWM_ISR1 & PWM_ISR1_CHID1)
    {
        // printf("Hello from ISR\n\r");
        PWM_set_duty(joystick.pos_x);
    }
}

void PWM_init()
{
    // enable PWM periferal clock
    PMC->PMC_PCER1 |= PMC_PCDR1_PID36;

    // disable pwm chanel 1
    REG_PWM_DIS |= PWM_DIS_CHID1;

    PWM_WPCR_WPCMD(1);
    // PWM WPSR

    // CLKB set by PREB
    PWM_CLK_DIVB(1);
    // CLKB = MCK/32
    PWM_CLK_PREB(0b0101);

    REG_PWM_CMR1 |= PWM_CMR_CPRE_MCK_DIV_32;

    // left aligned by default

    // polarity
    REG_PWM_CMR1 |= PWM_CMR_CPOL;

    // Set period to 20 ms
    REG_PWM_CPRD1 |= 52500;

    // Set duty cycle ~1.5 ms
    REG_PWM_CDTY1 |= 4000;

    // Set comparison value
    // REG_PWM_CMPV1 = PWM_CMPV_CV(100);
    PWM->PWM_CMP[0].PWM_CMPV = PWM_CMPV_CV(100);

    // REG_PWM_CMPM1 = PWM_CMPM_CTR(0) | PWM_CMPM_CEN;
    PWM->PWM_CMP[0].PWM_CMPM = PWM_CMPM_CTR(0) | PWM_CMPM_CEN;

    // enable pulse on event line 1
    // REG_PWM_ELMR |= PWM_ELMR_CSEL1;

    // enable interupt on event
    REG_PWM_IER1 |= PWM_IER1_CHID1;
    REG_PWM_IER2 |= PWM_IER2_CMPM1;

    // REG_PWM_IER1 |= PWM_IER1_CHID1;
    // REG_PWM_CMPV0 = PWM_CMPV_CV(100);
    // REG_PWM_IER2 |= PWM_IER2_CMPM0;

    // Set pin13 as pwm output
    PIOB->PIO_PDR |= PIO_PB13;
    PIOB->PIO_ABSR |= PIO_PB13;

    // enable PWM channel
    REG_PWM_ENA |= PWM_ENA_CHID1;

    NVIC_EnableIRQ(PWM_IRQn);

    // REG_PWM_CDTYUPD1 = 1000;
}

uint32_t map(int8_t x)
{

    return (uint32_t)((x + 128) * (5500 - 2400) / (256) + 2400); // Range sat to +-128, but our joystick gives +-88
}

void PWM_set_duty(int8_t value)
{
    REG_PWM_CDTYUPD1 = map(value);
}