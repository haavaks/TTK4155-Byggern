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
    POS7 = 7,
};

typedef void (*menu_function)(void);

struct menu_t
{
    struct menu_t *sub_menu[8];
    char *title;
    // funksjon når button pressed
    menu_function func;
};

struct menu_t *menu_init(struct menu_t *parent, struct menu_t *sub_menu[], char *title, menu_function func);
struct menu_t *init_all_menues();
void goto_menu(struct menu_t *menu);
void update_menu_pos(struct Joystick_pos_t *j, struct menu_t *menu, enum Menu_positions *menu_pos);