#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <inttypes.h>

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define DD_SS DDB4
#define P_SS PB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK DDB7

void SPI_set_ss();
void SPI_clear_ss();

void SPI_master_init(void);
void SPI_transmit(uint8_t cData);
uint8_t SPI_recive(void);

#endif // SPI_H