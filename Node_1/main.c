#define F_CPU 4915200

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Usart.h"
#include "XMEM.h"
#include "ADC.h"
#include "Joystick.h"
#include "OLED.h"

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

void SRAM_test_one_addr(void)
{
    volatile char *ext_ram = (char *)0x1800;

    ext_ram[0] = 0b01010101;
}

void main(void)
{
    XMEM_init();
    ADC_init();

    Usart_init(MYUBRR);

    // struct Joystick_pos_t joystick = {0};

    // Joystick_calibrate(&joystick);

    // while (1)
    // {
    //     joystick_get_pos(&joystick);

    //     joystick_print_pos(&joystick);

    //     _delay_ms(200);
    // }

    OLED_init();
    while (1)
    {
        // OLED_write_command(0xA5);
        OLED_fill_screen();
        _delay_ms(200);
        // OLED_write_command(0xA4);
        OLED_clear_screen();
        _delay_ms(200);
    }
}