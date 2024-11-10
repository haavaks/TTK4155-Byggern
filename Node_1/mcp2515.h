#ifndef MCP_H
#define MCP_H

#include <inttypes.h>
#include <util/delay.h>

#include "mcp2515_config.h"
#include "SPI.h"

uint8_t mcp2515_init();
uint8_t mcp2515_read(uint8_t addr);
void mcp2515_reset();
void mcp2515_write(uint8_t data, uint8_t addr);
void mcp2515_rts(uint8_t reg);
void mcp2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);
uint8_t mcp2515_read_status(void);
void mcp2515_set_mode(uint8_t mode);

#endif // MCP_H