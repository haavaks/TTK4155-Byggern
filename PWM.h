#pragma once

#include <inttypes.h>
#include "can.h"
#include "sam.h"

struct Joystick_pos_t
{
    int8_t pos_x;
    int8_t pos_y;
};

volatile struct Joystick_pos_t joystick;

void PWM_init();
void PWM_set_duty_servo(int8_t value);
void PWM_set_duty_motor(int8_t value);
void update_joystick_pos_from_CAN(CanMsg msg);