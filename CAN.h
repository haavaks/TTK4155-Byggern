#ifndef CAN_H
#define CAN_H

#define F_OSC 16000000

#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "mcp2515.h"
#include "mcp2515_config.h"
#include "Joystick.h"

typedef struct
{
    uint16_t id;
    uint8_t len;
    uint8_t data[8];
} CAN_message_t;

void CAN_init(void);
void CAN_send(CAN_message_t *msg);
void CAN_send_joystick(struct Joystick_pos_t *j);
CAN_message_t *CAN_receive(void);

#endif // CAN_H