#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "ADC.h"

enum Directions
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

struct Joystick_pos_t
{
    int8_t pos_x;
    int8_t pos_y;
    int8_t x_offset;
    int8_t y_offset;
    int8_t dir;
};

void joystick_get_pos(struct Joystick_pos_t *j);

void Joystick_calibrate(struct Joystick_pos_t *j);

void joystick_print_pos(struct Joystick_pos_t *j);

#endif // JOYSTICK_H