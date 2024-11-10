#include "menu.h"
#include "CAN.h"

volatile bool start_game_flag = 0;

struct menu_t *menu_init(struct menu_t *parent, char *title, menu_function func)
{
    struct menu_t *menu = (struct menu_t *)malloc(sizeof(struct menu_t));

    menu->parent = parent;

    for (int i = 0; i < 7; i++)
    {
        menu->sub_menu[i] = NULL;
    }

    menu->title = title;

    menu->func = func;

    return menu;
}

bool get_start_game_flag()
{
    return start_game_flag;
}

void reset_start_game_flag()
{
    start_game_flag = 0;
}

void start_game_f(void)
{
    start_game_flag = 1;
}

struct menu_t *init_all_menues()
{
    struct menu_t *root = menu_init(NULL, "Main Menu", NULL);

    struct menu_t *start_game = menu_init(root, "Start game", &start_game_f);
    struct menu_t *settings = menu_init(root, "Settings", NULL);

    root->sub_menu[0] = start_game;
    root->sub_menu[1] = settings;

    struct menu_t *setting_0 = menu_init(settings, "Setting 1", NULL);
    struct menu_t *setting_1 = menu_init(settings, "Setting 2", NULL);
    struct menu_t *setting_2 = menu_init(settings, "Setting 3", NULL);

    settings->sub_menu[0] = setting_0;
    settings->sub_menu[1] = setting_1;
    settings->sub_menu[2] = setting_2;

    return root;
}

void print_menu(struct menu_t *menu, enum Menu_positions pos)
{
    OLED_clear_screen();

    OLED_goto_pos(0, 20);

    OLED_print_str(menu->title);

    for (int i = 0; i < 7; i++)
    {
        OLED_goto_pos(i + 1, 0);

        if (menu->sub_menu[i] != NULL)
        {
            OLED_print_str(menu->sub_menu[i]->title);
        }
    }
    OLED_goto_pos(pos + 1, 127 - 9);
    OLED_print_char('#');
}

struct menu_t *navigate_menu(struct Joystick_pos_t *j, struct menu_t *curr_menu, enum Menu_positions *menu_pos)
{
    if (j->dir == j->prev_dir)
    {

        return curr_menu;
    }

    switch (j->dir)
    {
    case UP:
        if (*(menu_pos) > POS0)
        {
            *(menu_pos) -= 1;
        }

        break;

    case DOWN:
        if (*(menu_pos) < POS6 && curr_menu->sub_menu[*(menu_pos) + 1] != NULL)
        {
            *(menu_pos) += 1;
        }
        break;

    case RIGHT:
        if (curr_menu->sub_menu[*(menu_pos)] != NULL)
        {
            if (curr_menu->sub_menu[*(menu_pos)]->func != NULL)
            {
                (*curr_menu->sub_menu[*(menu_pos)]->func)();
            }
            else
            {
                curr_menu = curr_menu->sub_menu[*(menu_pos)];
                *(menu_pos) = POS0;
            }
        }

        break;

    case LEFT:
        if (curr_menu->parent != NULL)
        {
            curr_menu = curr_menu->parent;
            *(menu_pos) = POS0;
        }

        break;

    default:
        break;
    }

    return curr_menu;
}