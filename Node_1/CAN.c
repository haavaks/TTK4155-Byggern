#include "CAN.h"

volatile CAN_message_t rx_msg = {0};

ISR(__vector_default) {}

ISR(INT0_vect)
{
    rx_msg.id = (mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5);

    rx_msg.len = mcp2515_read(MCP_RXB0DLC);

    for (int i = 0; i < rx_msg.len; i++)
    {
        rx_msg.data[i] = mcp2515_read(MCP_RXB0D0 + i);
    }

    mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0); // Reset interrupt flag
}

void CAN_init(void)
{
    mcp2515_init();

    mcp2515_set_mode(MODE_CONFIG);

    mcp2515_bit_modify(MCP_CNF3, 0xFF, 5);                       // PS2 = 5
    mcp2515_bit_modify(MCP_CNF2, 0xFF, (1 << 7) | (6 << 3) | 1); // PRSEG = 1, PS1 = 6
    mcp2515_bit_modify(MCP_CNF1, 0xFF, 3);                       // BRP = 3

    mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IF, 1); // Set RX0IE, active low interrupt on message recive in RXB0

    // Turn off mask/filter on RXB0
    mcp2515_bit_modify(MCP_RXB0CTRL, (0b11 << 5), (0b11 << 5));

    cli();

    // SREG |= (1 << 7);    // Global interrrupt enable
    MCUCR |= (1 << ISC01);
    GICR |= (1 << INT0); // External interrupt request 2 enable

    sei();
    mcp2515_set_mode(MODE_NORMAL);
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
}

void CAN_send_joystick(struct Joystick_pos_t *j)
{
    CAN_message_t *msg = malloc(sizeof(CAN_message_t));

    msg->id = 1;
    msg->len = 4;

    msg->data[0] = j->pos_x;
    msg->data[1] = j->pos_y;
    msg->data[2] = 100;
    msg->data[3] = j->button;

    CAN_send(msg);

    free(msg);
}

CAN_message_t *CAN_receive(void)
{
    // CAN_message_t *msg = malloc(sizeof(CAN_message_t));

    // msg->id = (mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5);

    // msg->len = mcp2515_read(MCP_RXB0DLC);

    // for (int i = 0; i < msg->len; i++)
    // {
    //     msg->data[i] = mcp2515_read(MCP_RXB0D0 + i);
    // }

    return &rx_msg;
}