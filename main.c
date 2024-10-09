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

void main(void)
{
    XMEM_init();
    ADC_init();
    joystick_init();

    Usart_init(MYUBRR);

    struct Joystick_pos_t joystick = {0};

    joystick_calibrate(&joystick);

    OLED_init();

    OLED_clear_screen();

    struct menu_t *root = init_all_menues();

    enum Menu_positions menu_pos = POS0;
    struct menu_t *curr_menu = root;

    goto_menu(curr_menu);
    OLED_goto_pos(menu_pos + 1, 127 - 9);
    OLED_print_char('#');

    /////////////
    mcp2515_init();

    mcp2515_set_mode(MODE_LOOPBACK);

    CAN_message_t *msg = malloc(sizeof(CAN_message_t));

    msg->id = 0b0000011111111110;
    msg->len = 5;
    msg->data[0] = 'H';
    msg->data[1] = 'e';
    msg->data[2] = 'l';
    msg->data[3] = 'l';
    msg->data[4] = 'o';

    CAN_send(msg);

    CAN_message_t *ret = CAN_receive();

    printf("Id: %d\n\r", ret->id);
    printf("Len: %d\n\r", ret->len);
    printf("Data: %s\n\r", ret->data);

    // while (1)
    // {
    //     SPI_clear_ss();
    //     SPI_transmit(0b10101010);
    //     SPI_set_ss();
    //     SPI_recive();
    //     //_delay_ms(1);
    //     // SPI_set_ss();
    //     //_delay_ms(1);
    // }

    // _delay_ms(2000);
    // goto_menu(root->sub_menu[2]);

    // while (1)
    // {
    //     joystick_update(&joystick);
    //     // joystick_print_pos(&joystick);
    //     _delay_ms(200);

    //     curr_menu = navigate_menu(&joystick, curr_menu, &menu_pos);

    //     // printf("Curr_menu: %d\n\r", joystick->);

    //     goto_menu(curr_menu);

    //     OLED_goto_pos(menu_pos + 1, 127 - 9);
    //     OLED_print_char('#');
    // }
}