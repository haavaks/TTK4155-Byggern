#pragma once

#include <inttypes.h>
#include "can.h"
#include "controller.h"
#include "sam.h"

#define MOTOR_PWM_PERIOD 30000

struct Joystick_pos_t
{
    int8_t pos_x;
    int8_t pos_y;
    int8_t R_slider;
    int8_t button;
};

// volatile struct Joystick_pos_t joystick;

// PWM_start must be called to start signals
void PWM_init();
void PWM_set_duty_servo(int8_t value);
void PWM_set_duty_motor(int8_t value);
void update_joystick_pos_from_CAN(CanMsg msg);
void PWM_stop();
void PWM_start();