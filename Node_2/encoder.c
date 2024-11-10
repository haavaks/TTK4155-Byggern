#include "encoder.h"

void Encoder_init()
{
    PIOC->PIO_ABSR |= PIO_ABSR_P25 | PIO_ABSR_P26; // Set peripheral for PC25 and PC26
    PIOC->PIO_PDR |= PIO_PDR_P25 | PIO_PDR_P26;    // Disable PIO control, giving control to TC2
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32));        // Enable clock for TC6

    REG_TC2_BMR |= TC_BMR_QDEN; // Enable the QDEC mode
    REG_TC2_BMR |= TC_BMR_POSEN | TC_BMR_EDGPHA;
    REG_TC2_BMR &= ~TC_BMR_QDTRANS;

    REG_TC2_CMR0 |= TC_CMR_ETRGEDG_RISING;
    REG_TC2_CMR0 |= TC_CMR_ABETRG;
    REG_TC2_CMR0 |= TC_CMR_TCCLKS_XC0;

    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}