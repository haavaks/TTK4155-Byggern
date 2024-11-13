#include <sam.h>
#include "solenoid.h"

void Solenoid_init()
{
    // Enabling pin PC23 for output
    PMC->PMC_PCER0 |= (1 << ID_PIOC);
    PIOC->PIO_OER |= (1 << 23);
    PIOC->PIO_PER |= (1 << 23);
    PIOC->PIO_SODR = (1 << 23);
}

void Solenoid_fire()
{
    PIOC->PIO_CODR = (1 << 23);
    //_delay_ms(1);
    for (uint32_t i = 0; i < 5 * 42000; i++)
    {
        __asm("nop");
    }
    PIOC->PIO_SODR = (1 << 23);
}