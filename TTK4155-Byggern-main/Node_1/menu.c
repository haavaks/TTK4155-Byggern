#include "menu.h"

struct menu_t *menu_init(struct menu_t *parent, struct menu_t *sub_menu[], char *title, menu_function func)
{
    struct menu_t *menu = (struct menu_t *)malloc(sizeof(struct menu_t));

    menu->sub_menu[0] = parent;

    for (int i = 0; i < 7; i++)
    {
        menu->sub_menu[i + 1] = sub_menu[i];
    }

    menu->title = (char *)malloc(128 * sizeof(char));
    menu->title = title;

    menu->func = func;

    return menu;
}

struct menu_t *init_all_menues()
{
    struct menu_t *nulls[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    struct menu_t *root = menu_init(NULL, nulls, "Root", NULL);
    struct menu_t *start_game = menu_init(root, nulls, "Start game", NULL);
    struct menu_t *settings = menu_init(root, nulls, "Settings", NULL);
    struct menu_t *scoreboard = menu_init(root, nulls, "Scoreboard", NULL);
    struct menu_t *setting_1 = menu_init(settings, nulls, "Setting 1", NULL);
    struct menu_t *setting_2 = menu_init(settings, nulls, "Setting 2", NULL);
    struct menu_t *setting_3 = menu_init(settings, nulls, "Setting 3", NULL);
    root->sub_menu[1] = start_game;
    root->sub_menu[2] = settings;
    root->sub_menu[3] = scoreboard;
    settings->sub_menu[1] = setting_1;
    settings->sub_menu[2] = setting_2;
    settings->sub_menu[3] = setting_3;

    return root;
}

void goto_menu(struct menu_t *menu)
{
    OLED_clear_screen();
    for (int i = 0; i < 8; i++)
    {
        OLED_goto_pos(i, 0);

        if (menu->sub_menu[i] != NULL)
        {
            OLED_print_str(menu->sub_menu[i]->title);
        }
    }
}

void update_menu_pos(struct Joystick_pos_t *j, struct menu_t *menu, enum Menu_positions *menu_pos)
{
    int num_submenues = 0;
    while (menu->sub_menu[num_submenues] != NULL) // Counts all submenues present
    {
        num_submenues += 1;
    }

    if (j->dir == UP && (int)menu_pos != 0 && j->prev_dir == NEUTRAL)
    {
        menu_pos -= 1;
        goto_menu(menu);
        OLED_clear_page((uint8_t)menu_pos);
        OLED_print_inv_str(menu->sub_menu[(int)menu_pos]->title);
    }
    else if (j->dir == DOWN && (int)menu_pos != num_submenues && j->prev_dir == NEUTRAL)
    {
        menu_pos += 1;
        goto_menu(menu);
        OLED_clear_page((uint8_t)menu_pos);
        OLED_print_inv_str(menu->sub_menu[(int)menu_pos]->title);
    }
}