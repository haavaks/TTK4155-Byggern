#include <sam.h>
#include "solenoid.h"

void Solenoid_init()
{
    // Enabling pin PA4 for output
    PMC->PMC_PCER0 |= (1 << ID_PIOA);
    PIOA->PIO_OER |= (1 << 4);
    PIOA->PIO_PER |= (1 << 4);
    PIOA->PIO_SODR = (1 << 4);
}

void Solenoid_fire()
{
    PIOA->PIO_CODR = (1 << 4);
    //_delay_ms(1);
    for (uint32_t i = 0; i < 5 * 42000; i++)
    {
        __asm("nop");
    }
    PIOA->PIO_SODR = (1 << 4);
}