#include "menu.h"

struct menu_t *menu_init(struct menu_t *parent, char *title, menu_function func)
{
    struct menu_t *menu = (struct menu_t *)malloc(sizeof(struct menu_t));

    menu->parent = parent;

    for (int i = 0; i < 7; i++)
    {
        menu->sub_menu[i] = NULL;
    }

    // instant memory leak !!
    // menu->title = (char *)malloc(128 * sizeof(char));
    menu->title = title;

    menu->func = func;

    return menu;
}

void start_game_f(void)
{
    printf("Playing game!\n\r");
}

struct menu_t *init_all_menues()
{
    // struct menu_t *nulls[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    // struct menu_t *root = menu_init(NULL, nulls, "Root", NULL);
    // struct menu_t *start_game = menu_init(root, nulls, "Start game", NULL);
    // struct menu_t *settings = menu_init(root, nulls, "Settings", NULL);
    // struct menu_t *scoreboard = menu_init(root, nulls, "Scoreboard", NULL);
    // struct menu_t *setting_1 = menu_init(settings, nulls, "Setting 1", NULL);
    // struct menu_t *setting_2 = menu_init(settings, nulls, "Setting 2", NULL);
    // struct menu_t *setting_3 = menu_init(settings, nulls, "Setting 3", NULL);
    // root->sub_menu[1] = start_game;
    // root->sub_menu[2] = settings;
    // root->sub_menu[3] = scoreboard;
    // settings->sub_menu[1] = setting_1;
    // settings->sub_menu[2] = setting_2;
    // settings->sub_menu[3] = setting_3;

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

void goto_menu(struct menu_t *menu)
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