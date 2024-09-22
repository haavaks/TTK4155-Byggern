#include "Joystick.h"

void joystick_get_pos(struct Joystick_pos_t *j)
{
    int16_t x = (int8_t)(ADC_read(CH_x) - 128);
    int16_t y = (int8_t)(ADC_read(CH_y) - 128);
    if (x < j->x_offset - 128)
    {
        x = j->x_offset - 128;
    }
    else if (((x - j->x_offset) < 3) && ((x - j->x_offset) > -3))
    {
        x = 0;
    }
    else
    {
        x -= j->x_offset;
    }

    if (y < j->y_offset - 128)
    {
        y = j->y_offset - 128;
    }
    else if (((y - j->y_offset) < 3) && ((y - j->y_offset) > -3))
    {
        y = 0;
    }
    else
    {
        y -= j->y_offset;
    }

    j->pos_x = x;
    j->pos_y = y;
}

void Joystick_calibrate(struct Joystick_pos_t *j)
{
    joystick_get_pos(j);
    j->x_offset = j->pos_x;
    j->y_offset = j->pos_y;
}

void joystick_print_pos(struct Joystick_pos_t *j)
{
    printf("X: %-5d", j->pos_x);
    printf("  ");
    printf("Y: %-5d", j->pos_y);
    printf("\n\r");
}