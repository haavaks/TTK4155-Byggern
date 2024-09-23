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

#define MYUBRR F_CPU / 16 / BAUD_RATE - 1

void main(void)
{
    XMEM_init();
    ADC_init();
    joystick_init();

    Usart_init(MYUBRR);

    struct Joystick_pos_t joystick = {0};
    enum Menu_positions start_pos = POS0;
    enum Menu_positions *menu_pos = &start_pos;

    joystick_calibrate(&joystick);

    OLED_init();

    OLED_clear_screen();

    struct menu_t *root = init_all_menues();
    goto_menu(root);
    _delay_ms(2000);
    goto_menu(root->sub_menu[2]);

    // while (1)
    // {

    //     // printf(*menu_pos);
    //     joystick_update(&joystick);
    //     // update_menu_pos(&joystick, root, menu_pos);
    //     joystick_print_pos(&joystick);

    //     _delay_ms(300);
    // }
}