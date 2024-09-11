#define F_CPU 4915200

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include "Usart.h"
#include <stdint.h>
#include <stdlib.h>

#define MYUBRR F_CPU / 16 / BAUD_RATE - 1

void test_latch(void)
{
    DDRE = 0b10; // Setter ALE (pin 1 på port E) som utgang. 0=inngang, 1=utgang

    PORTE = 0b10;     // Setter ALE høy. Forteller vippa at nå kommer det en adresse som skal lagres.
    PORTA = 0b000001; // = 0x00. Sender en adresse ut til vippa.

    _delay_ms(2000); // 2 sek delay for å gjøre det enklere å måle på kretsen.

    PORTE = 0b00; // Setter ALE lav. Nå lagres adresseverdien i vippa.

    _delay_ms(2000);

    PORTA = 0b01010101; // Sender ut ny adresse. Ingenting skjer på baksiden av vippa.

    _delay_ms(2000);

    PORTE = 0b10; // Gammel adresse fjernes fra vippa og den nye sendes gjennom.

    _delay_ms(2000);
}

void SRAM_test(void)
{
    volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\r\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value)
        {
            // printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value)
        {
            // printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}

void SRAM_test_one_addr(void)
{
    volatile char *ext_ram = (char *)0x1800;

    ext_ram[0] = 0b01010101;
}

void main(void)
{
    // DDRC |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    // DDRA |= 0xFF;
    // DDRE |= (1 << 1);
    // DDRD |= (1 << 6) | (1 << 7);

    MCUCR = (1 << SRE);
    // SFIOR &= ~(1 << XMM0);
    // SFIOR &= ~(1 << XMM1);
    SFIOR = (1 << XMM2);

    // PORTC = 0b0100;
    fdevopen(&Usart_transmit, 0);
    Usart_init(MYUBRR);

    // PORTC |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    // PORTD |= (0 << 6) | (0 << 7);
    for (int i = 0; i < 10; i++)
        SRAM_test();

    volatile char *ext_adc = (char *)0x1400;

    ext_adc[0] = 0xFF;
}