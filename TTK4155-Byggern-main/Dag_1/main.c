#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1

void USART_Init(unsigned int ubrr)
{
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char USART_Receive(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get and return received data from buffer */
    return UDR0;
}

void main(void)
{
    // DDRB = (1 << 0);

    // while (1)
    // {
    //     PORTB = 0xFF;
    //     _delay_ms(500);
    //     PORTB = 0x00;
    //     _delay_ms(500);
    // }
    fdevopen(&USART_Transmit, 0);

    USART_Init(MYUBRR);

    unsigned char data;

    // while (1)
    // {
    //     data = USART_Receive();
    //     _delay_ms(10000);
    //     USART_Transmit(data + 1);
    // }

    while (1)
    {
        printf("Hello World!\n\r");
        _delay_ms(5000);
    }
}