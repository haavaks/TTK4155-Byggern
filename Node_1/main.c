#define F_CPU 4915200

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Usart.h"
#include "XMEM.h"
#include "ADC.h"
#include "Joystick.h"
#include "OLED.h"
#include "menu.h"
#include "mcp2515.h"
#include "SPI.h"
#include "CAN.h"

#define MYUBRR F_CPU / 16 / BAUD_RATE - 1

typedef enum
{
    Playing = 0,
    Game_over = 1,
    Menu = 2
} GameState;

void main(void)
{
    XMEM_init();
    ADC_init();
    Usart_init(MYUBRR);
    OLED_init();
    CAN_init();

    struct Joystick_pos_t joystick = joystick_init();
    GameState state = Menu;

    // MENU SETUP
    struct menu_t *root = init_all_menues();
    enum Menu_positions menu_pos = POS0;
    struct menu_t *curr_menu = root;
    print_menu(curr_menu, menu_pos);

    while (1)
    {
        switch (state)
        {
        case (Menu):
            joystick_update(&joystick);
            curr_menu = navigate_menu(&joystick, curr_menu, &menu_pos);
            print_menu(curr_menu, menu_pos);

            if (get_start_game_flag())
            {
                state = Playing;
                CAN_message_t start_game = {
                    .id = Start_game_id};
                CAN_send(&start_game);
                reset_start_game_flag();
                OLED_fill_screen();
                printf("Start game\r\n");
            }

            _delay_ms(10);
            break;

        case (Playing):
            joystick_update(&joystick);
            CAN_send_joystick(&joystick);

            joystick_print_pos(&joystick);

            if (CAN_receive()->id == Game_over_id)
            {
                state = Menu;
                printf("GAME OVER\r\n");
                printf("Time: %x\r\n", CAN_receive()->data[0]);
            }

            _delay_ms(10);
            break;
        }
    }

    return;
}
