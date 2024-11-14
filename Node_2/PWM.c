#include "PWM.h"

volatile struct Joystick_pos_t joystick = {0};
volatile PI_controller controller = {1, 0.01, 0.5, 0, 0};

void PWM_Handler(void)
{
    if (PWM->PWM_ISR1 & PWM_ISR1_CHID1)
    {
        PWM_set_duty_servo(joystick.pos_x);

        // int32_t encoder = (REG_TC2_CV0 + 2810)*(255)/(5620)-128; //actual range 5620
        int32_t encoder = (REG_TC2_CV0 + 3000) * (255) / (6000) - 128;

        float c = PI_controller_update(&controller, (float)encoder, (float)joystick.pos_y);
        PWM_set_duty_motor(c);
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

    //___________________________________________________MOTOR PWM

    // disable pwm chanel 0
    REG_PWM_DIS |= PWM_DIS_CHID0;

    REG_PWM_CMR0 |= PWM_CMR_CPRE_MCK_DIV_32;

    // polarity
    REG_PWM_CMR0 |= PWM_CMR_CPOL;

    // Set period to 20 ms
    REG_PWM_CPRD0 |= MOTOR_PWM_PERIOD;

    // Set duty cycle ~1.5 ms
    REG_PWM_CDTY0 |= 0;

    // Set pin12 as pwm output
    PIOB->PIO_PDR |= PIO_PB12;
    PIOB->PIO_ABSR |= PIO_PB12;
    PIOB->PIO_PUDR |= PIO_PUDR_P12;

    NVIC_EnableIRQ(PWM_IRQn);

    // Motor dir

    PMC->PMC_PCDR0 |= ID_PIOC;

    PIOC->PIO_OER |= PIO_OER_P23;

    PIOC->PIO_PER |= PIO_PER_P23;
    PIOC->PIO_PUDR |= PIO_PUDR_P23;

    REG_PWM_ENA |= PWM_ENA_CHID0;
    REG_PWM_ENA |= PWM_ENA_CHID1;
}

uint32_t servo_map(int8_t x)
{
    return (uint32_t)((x + 128) * (5500 - 2400) / (256) + 2400); // Input range sat to +-128, but our joystick gives +-88
}

uint32_t motor_map(int8_t x)
{
    return (uint32_t)((x + 128) * (MOTOR_PWM_PERIOD) / (256)); // Input range sat to +-128, but our joystick gives +-88
}

void PWM_set_duty_servo(int8_t value)
{
    REG_PWM_CDTYUPD1 = servo_map(value);
}

void PWM_set_duty_motor(int8_t value)
{
    if (value >= 0)
    {
        PIOC->PIO_CODR = PIO_ODR_P23;
        REG_PWM_CDTYUPD0 = (value)*MOTOR_PWM_PERIOD / 127;
    }
    else
    {
        PIOC->PIO_SODR = PIO_ODR_P23;
        REG_PWM_CDTYUPD0 = (-value) * MOTOR_PWM_PERIOD / 128;
    }
}

void update_joystick_pos_from_CAN(CanMsg msg)
{
    if (msg.id == Joystick_id)
    {
        joystick.pos_x = msg.byte[0];
        joystick.pos_y = msg.byte[1];
        joystick.R_slider = msg.byte[2];
        joystick.button = msg.byte[3];
    }
}

void PWM_stop()
{
    PIOB->PIO_PER = PIO_PER_P12;
    PIOB->PIO_OER = PIO_OER_P12;
    PIOB->PIO_CODR = PIO_CODR_P12;

    PIOB->PIO_PER = PIO_PER_P13;
    PIOB->PIO_OER = PIO_OER_P13;
    PIOB->PIO_CODR = PIO_CODR_P13;

    // REG_PWM_DIS |= PWM_DIS_CHID0;
    // REG_PWM_DIS |= PWM_DIS_CHID1;

    // // Disable interupt on event
    // REG_PWM_IDR1 |= PWM_IDR1_CHID1;
    // REG_PWM_IDR2 |= PWM_IDR2_CMPM1;
}

void PWM_start()
{

    // REG_PWM_ENA |= PWM_ENA_CHID0;
    // REG_PWM_ENA |= PWM_ENA_CHID1;

    PIOB->PIO_PDR = PIO_PDR_P12;
    PIOB->PIO_ABSR &= ~PIO_ABSR_P12;
    PIOB->PIO_PDR = PIO_PDR_P13;
    PIOB->PIO_ABSR &= ~PIO_ABSR_P13;

    PI_controller_reset(&controller);

    // // enable interupt on event
    // REG_PWM_IER1 |= PWM_IER1_CHID1;
    // REG_PWM_IER2 |= PWM_IER2_CMPM1;
}