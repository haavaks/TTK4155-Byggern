#include "timer.h"

void timer_init()
{
    PMC->PMC_PCER0 |= (1 << ID_TC0); // PMC enable TC0

    // TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK2;

    REG_TC0_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK2; // MCK/8
}

void timer_start()
{
    TC0->TC_CHANNEL[0].TC_CV = 0; // reset counter
    REG_TC0_CCR0 &= ~TC_CCR_CLKDIS;
    REG_TC0_CCR0 |= TC_CCR_CLKEN; // Enable TC0
    REG_TC0_CCR0 |= TC_CCR_SWTRG;
}

void timer_stop()
{
    REG_TC0_CCR0 &= ~TC_CCR_CLKDIS;
}

uint32_t timer_value()
{
    return REG_TC0_CV0;
}