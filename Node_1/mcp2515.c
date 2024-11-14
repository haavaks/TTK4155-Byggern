#include "mcp2515.h"

uint8_t mcp2515_init()
{
    uint8_t value;
    SPI_master_init(); // Initialize SPI
    mcp2515_reset();   // Send reset - command

    _delay_ms(1);

    // Self - test
    value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG)
    {
        printf(" MCP2515 is NOT in configuration mode after reset !\n\r ");
        return 1;
    }

    return 0;
}

uint8_t mcp2515_read(uint8_t addr)
{
    uint8_t result;

    SPI_clear_ss();
    SPI_transmit(MCP_READ); // Send read instruction
    SPI_transmit(addr);     // Send address
    result = SPI_recive();  // Read result
    SPI_set_ss();

    return result;
}

void mcp2515_reset()
{
    SPI_clear_ss();
    SPI_transmit(MCP_RESET);
    SPI_set_ss();
}

void mcp2515_write(uint8_t data, uint8_t addr)
{
    SPI_clear_ss();
    SPI_transmit(MCP_WRITE);
    SPI_transmit(addr);
    SPI_transmit(data);
    SPI_set_ss();
}

void mcp2515_rts(uint8_t reg)
{
    SPI_clear_ss();
    switch (reg)
    {
    case 0:
        SPI_transmit(MCP_RTS_TX0);
        break;
    case 1:
        SPI_transmit(MCP_RTS_TX1);
        break;
    case 2:
        SPI_transmit(MCP_RTS_TX2);
        break;
    default:
        break;
    }
    SPI_set_ss();
}

/**
 * @param addr Adress of register to be changed
 * @param mask Bits to be changed are set high
 * @param data New register values
 */
void mcp2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data)
{
    SPI_clear_ss();
    SPI_transmit(MCP_BITMOD);
    SPI_transmit(addr);
    SPI_transmit(mask);
    SPI_transmit(data);
    SPI_set_ss();
}

uint8_t mcp2515_read_status(void)
{
    uint8_t value;

    SPI_clear_ss();
    SPI_transmit(MCP_READ_STATUS);
    value = SPI_recive();
    SPI_set_ss();

    return value;
}

void mcp2515_set_mode(uint8_t mode)
{
    mcp2515_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}