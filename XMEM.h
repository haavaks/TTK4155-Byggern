#ifndef XMEM_H
#define XMEM_H

#include <avr/io.h>

/**
 * @brief Enableling of addresses above 0x04FF for external SRAM
 */
void XMEM_init(void);

#endif // XMEM_H