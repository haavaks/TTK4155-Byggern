#ifndef CAN_H
#define CAN_H

#include <inttypes.h>

#include "mcp2515.h"
#include "mcp2515_config.h"

typedef struct
{
    uint16_t id;
    uint8_t len;
    uint8_t data[8];
} CAN_message_t;

void CAN_init(void);
void CAN_send(CAN_message_t *msg);
CAN_message_t *CAN_receive(void);

#endif // CAN_H