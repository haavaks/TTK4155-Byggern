#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OLED.h"
#include "Joystick.h"

enum Menu_positions
{
    POS0 = 0,
    POS1 = 1,
    POS2 = 2,
    POS3 = 3,
    POS4 = 4,
    POS5 = 5,
    POS6 = 6,
    // POS7 = 7,
};

typedef void (*menu_function)(void);

struct menu_t
{
    char *title;

    struct menu_t *sub_menu[7];
    struct menu_t *parent;

    // funksjon når button pressed
    menu_function func;
};

struct menu_t *menu_init(struct menu_t *parent, char *title, menu_function func);
struct menu_t *init_all_menues();
void goto_menu(struct menu_t *menu);
void update_menu_pos(struct Joystick_pos_t *j, struct menu_t *menu, enum Menu_positions *menu_pos);

struct menu_t *navigate_menu(struct Joystick_pos_t *j, struct menu_t *curr_menu, enum Menu_positions *menu_pos);

#endif // MENU_H