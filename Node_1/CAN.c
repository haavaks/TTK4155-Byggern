#include "CAN.h"

volatile int new_msg = 0;

ISR(__vector_default) {}

ISR(INT0_vect, ISR_BLOCK)
{
    printf("Hello from ISR \n\r");
    mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0); // Reset interrupt flag
}

void CAN_init(void)
{
    mcp2515_init();

    mcp2515_set_mode(MODE_CONFIG);

    mcp2515_bit_modify(MCP_CNF3, 0xFF, 5);                       // PS2 = 5
    mcp2515_bit_modify(MCP_CNF2, 0xFF, (1 << 7) | (6 << 3) | 1); // PRSEG = 1, PS1 = 6
    mcp2515_bit_modify(MCP_CNF1, 0xFF, 3);                       // BRP = 3

    // mcp2515_bit_modify(MCP_CANINTE, (1 << 0), 0xFF); // Set RX0IE, active low interrupt on message recive in RXB0

    cli();

    // SREG |= (1 << 7);    // Global interrrupt enable
    GICR |= (1 << INT0); // External interrupt request 2 enable
    MCUCR |= (1 << ISC01);

    sei();
}

void CAN_send(CAN_message_t *msg)
{
    mcp2515_write(((msg->id >> 3) & 0xFF), MCP_TXB0SIDH);
    mcp2515_write((((msg->id) << 5) & 0xE0), MCP_TXB0SIDL);

    mcp2515_write(msg->len, MCP_TXB0DLC);

    for (int i = 0; i < msg->len; i++)
    {
        mcp2515_write(msg->data[i], MCP_TXB0D0 + i);
    }

    mcp2515_rts(0);

    _delay_ms(1);

    uint8_t ret = mcp2515_read(MCP_TXB0CTRL);
    uint8_t be = mcp2515_read(MCP_EFLG);

    printf("%x \n\r", be);

    if (ret & (1 << 3))
    {
        printf("Buffer pending\n\r");
    }
    else if (ret & (1 << 4))
    {
        printf("Buss Error\n\r");
    }
    else if (ret & (1 << 5))
    {
        printf("Message lost arbitration\n\r");
    }
    else if (ret & (1 << 6))
    {
        printf("Message aborted\n\r");
    }
    else
    {
        printf("Message success\n\r");
    }
}

CAN_message_t *CAN_receive(void)
{
    CAN_message_t *msg = malloc(sizeof(CAN_message_t));

    msg->id = (mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5);

    msg->len = mcp2515_read(MCP_RXB0DLC);

    for (int i = 0; i < msg->len; i++)
    {
        msg->data[i] = mcp2515_read(MCP_RXB0D0 + i);
    }

    return msg;
}