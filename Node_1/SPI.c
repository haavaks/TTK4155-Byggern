#include "SPI.h"

void SPI_set_ss()
{
    PORT_SPI |= (1 << P_SS);
}

void SPI_clear_ss()
{
    PORT_SPI &= ~(1 << P_SS);
}

void SPI_master_init(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1 << DD_SS) | (1 << DD_MOSI) | (1 << DD_SCK);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPIE);

    SPI_set_ss();
}
void SPI_transmit(uint8_t cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
    {
    }
}

uint8_t SPI_recive(void)
{
    /* Start transmission */
    SPDR = 0xFF;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)))
    {
    }

    return SPDR;
}
