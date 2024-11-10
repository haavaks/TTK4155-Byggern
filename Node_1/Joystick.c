#include "Joystick.h"
#include <avr/io.h>

struct Joystick_pos_t joystick_init()
{
    DDRB &= ~(1 << PB2); // Sets PB2 as input for joystick button
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);

    struct Joystick_pos_t joystick = {0};

    joystick_calibrate(&joystick);

    return joystick;
}

void joystick_update(struct Joystick_pos_t *j) // works only if offset is positive for x and y
{
    // Reading values
    int16_t x = (int8_t)(ADC_read(CH_x) - 128);
    int16_t y = (int8_t)(ADC_read(CH_y) - 128);

    // Compensating for offset
    if (x - j->x_offset < j->x_offset - 128)
    {
        x = j->x_offset - 128;
    }
    else if (((x - j->x_offset) <= 3) && ((x - j->x_offset) >= -3))
    {
        x = 0;
    }
    else
    {
        x -= j->x_offset;
    }

    if (y - j->y_offset < j->y_offset - 128)
    {
        y = j->y_offset - 128;
    }
    else if (((y - j->y_offset) <= 3) && ((y - j->y_offset) >= -3))
    {
        y = 0;
    }
    else
    {
        y -= j->y_offset;
    }

    // Updates positions
    j->pos_x = x;
    j->pos_y = y;

    // Updates new direction
    j->prev_dir = j->dir;

    if (abs(x) < 40 && abs(y) < 40)
    {
        j->dir = NEUTRAL;
    }
    else if (y > 0 && abs(y) > abs(x))
    {
        j->dir = UP;
    }
    else if (y < 0 && abs(y) > abs(x))
    {
        j->dir = DOWN;
    }
    else if (x > 0 && abs(x) > abs(y))
    {
        j->dir = RIGHT;
    }
    else if (x < 0 && abs(x) > abs(y))
    {
        j->dir = LEFT;
    }

    // Updates button pressed
    j->button = (PINB && (1 << PB2));
}

void joystick_calibrate(struct Joystick_pos_t *j)
{
    joystick_update(j);
    j->x_offset = j->pos_x;
    j->y_offset = j->pos_y;
}

void joystick_print_pos(struct Joystick_pos_t *j)
{
    printf("X: %-5d", j->pos_x);
    printf("  ");
    printf("Y: %-5d", j->pos_y);
    printf("  ");
    printf("Dir: %-5d", j->dir);
    printf("  ");
    printf("Prev_Dir: %-5d", j->prev_dir);
    printf("  ");
    printf("Button: %-5d", j->button);
    printf("\n\r");
}

int16_t slider_get_pos(enum ADC_Channels_t CH)
{
    int16_t pos = ADC_read(CH);
    if (pos < 0)
    {
        pos += 256;
    }
    return pos;
}