#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "ADC.h"
#include <stdlib.h>

enum Directions
{
    NEUTRAL = 0,
    RIGHT = 1,
    LEFT = 2,
    UP = 3,
    DOWN = 4
};

struct Joystick_pos_t
{
    int8_t pos_x;
    int8_t pos_y;
    int8_t x_offset;
    int8_t y_offset;
    enum Directions dir;
    enum Directions prev_dir;
    enum Directions new_dir;
    uint8_t button;
};

void joystick_init();

void joystick_update(struct Joystick_pos_t *j);

void joystick_calibrate(struct Joystick_pos_t *j);

void joystick_print_pos(struct Joystick_pos_t *j);

int16_t slider_get_pos(enum ADC_Channels_t CH);

#endif // JOYSTICK_H