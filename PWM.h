#pragma once

#include <inttypes.h>

#include "sam.h"

struct Joystick_pos_t
{
    int8_t pos_x;
    int8_t pos_y;
};

extern volatile struct Joystick_pos_t joystick;

void PWM_init();
void PWM_set_duty(int8_t value);