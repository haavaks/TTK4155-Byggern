/**
 * @file Usart.h
 *
 * @brief File containing functions for Usart communications
 *
 */

#ifndef USART_H
#define USART_H

#include <avr/io.h>

/**
 * @brief Usart Baud rate (Symbols / second)
 */
#define BAUD_RATE 9600

/**
 * Initialize Usart communication given the chip clock frequency. Should be called once.
 */
void Usart_init(unsigned int f_cpu);

/**
 * Transmit one byte of data over Usart while transmit buffer is empty, busy-wait if buffer is not empty
 */
void Usart_transmit(unsigned char data);

/**
 * Recive one byte of data over Usart while there is data in the receive-buffer, busy-wait if buffer is empty
 */
unsigned char Usart_receive(void);

#endif /* USART_H */