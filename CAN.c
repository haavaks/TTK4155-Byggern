#include "CAN.h"

void CAN_init(void)
{
    mcp2515_init();
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