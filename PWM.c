#include "PWM.h"

volatile struct Joystick_pos_t joystick = {0};

void PWM_Handler(void)
{
    if (PWM->PWM_ISR1 & PWM_ISR1_CHID1)
    {
        PWM_set_duty_servo(joystick.pos_x);
        PWM_set_duty_motor(joystick.pos_y);
    }
}

void PWM_init()
{
    // SERVO PWM

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

    // polarity
    REG_PWM_CMR1 |= PWM_CMR_CPOL;

    // Set period to 20 ms
    REG_PWM_CPRD1 |= 52500;

    // Set duty cycle ~1.5 ms
    REG_PWM_CDTY1 |= 4000;

    // Set comparison value
    PWM->PWM_CMP[0].PWM_CMPV = PWM_CMPV_CV(100);
    PWM->PWM_CMP[0].PWM_CMPM = PWM_CMPM_CTR(0) | PWM_CMPM_CEN;

    // enable interupt on event
    REG_PWM_IER1 |= PWM_IER1_CHID1;
    REG_PWM_IER2 |= PWM_IER2_CMPM1;

    // Set pin13 as pwm output
    PIOB->PIO_PDR |= PIO_PB13;
    PIOB->PIO_ABSR |= PIO_PB13;

    // enable PWM channel
    REG_PWM_ENA |= PWM_ENA_CHID1;

    //___________________________________________________MOTOR PWM

    // disable pwm chanel 0
    REG_PWM_DIS |= PWM_DIS_CHID0;

    REG_PWM_CMR0 |= PWM_CMR_CPRE_MCK_DIV_32;

    // polarity
    REG_PWM_CMR0 |= PWM_CMR_CPOL;

    // Set period to 20 ms
    REG_PWM_CPRD0 |= 52500;

    // Set duty cycle ~1.5 ms
    REG_PWM_CDTY0 |= 4000;

    // Set pin13 as pwm output
    PIOB->PIO_PDR |= PIO_PB12;
    PIOB->PIO_ABSR |= PIO_PB12;

    // enable PWM channel
    REG_PWM_ENA |= PWM_ENA_CHID0;

    NVIC_EnableIRQ(PWM_IRQn);
}

uint32_t map(int8_t x)
{

    return (uint32_t)((x + 128) * (5500 - 2400) / (256) + 2400); // Range sat to +-128, but our joystick gives +-88
}

void PWM_set_duty_servo(int8_t value)
{
    REG_PWM_CDTYUPD1 = map(value);
}

// TODO: Fix this function! Maybe map values from 0 to 1
void PWM_set_duty_motor(int8_t value)
{
    REG_PWM_CDTYUPD0 = 5 * map(value);
}

void update_joystick_pos_from_CAN(CanMsg msg)
{
    if (msg.id == Joystick_id) // Joystick pos
    {
        joystick.pos_x = msg.byte[0];
        joystick.pos_y = msg.byte[1];
    }
}