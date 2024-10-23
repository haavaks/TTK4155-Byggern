#include "OLED.h"
#include "fonts.h"

void OLED_write_command(uint8_t command)
{
    volatile char *oled_command = (char *)0x1000;
    oled_command[0] = command;
}

void OLED_write_data(uint8_t data)
{
    volatile char *oled_data = (char *)0x1200;
    oled_data[0] = data;
}

void OLED_init() // Taken from the datasheet LY190-128064
{
    OLED_write_command(0xa1); // segment remap
    OLED_write_command(0xae); // display off
    OLED_write_command(0xda); // common pads hardware: alternative
    OLED_write_command(0x12);
    OLED_write_command(0xc8); // common output scan direction:com63~com0
    OLED_write_command(0xa8); // multiplex ration mode:63
    OLED_write_command(0x3f);
    OLED_write_command(0xd5); // display divide ratio/osc. freq. mode
    OLED_write_command(0x80);
    OLED_write_command(0x81); // contrast control
    OLED_write_command(0x50);
    OLED_write_command(0xd9); // set pre-charge period
    OLED_write_command(0x21);
    OLED_write_command(0x20); // Set Memory Addressing Mode
    OLED_write_command(0x02); // Page
    OLED_write_command(0xdb); // VCOM deselect level mode
    OLED_write_command(0x30);
    OLED_write_command(0xad); // master configuration
    OLED_write_command(0x00);
    OLED_write_command(0xa4); // out follows RAM content
    OLED_write_command(0xa6); // set normal display
    OLED_write_command(0xaf); // display on

    OLED_clear_screen();
}

void OLED_clear_screen()
{
    OLED_write_command(0x20); // Set Memory Addressing Mode
    OLED_write_command(0x00); // horizontal
    OLED_write_command(0x21); // Set column address
    OLED_write_command(0x00); // Start 0
    OLED_write_command(0x7F); // End 127
    for (int i = 0; i < 8 * 128; i++)
    {
        OLED_write_data(0x00);
    }
    OLED_write_command(0x20); // Set Memory Addressing Mode
    OLED_write_command(0x02); // Page
}

void OLED_fill_screen()
{
    OLED_write_command(0x20); // Set Memory Addressing Mode
    OLED_write_command(0x00); // horizontal
    OLED_write_command(0x21); // Set column address
    OLED_write_command(0x00); // Start 0
    OLED_write_command(0x7F); // End 127
    for (int i = 0; i < 8 * 128; i++)
    {
        OLED_write_data(0xff);
    }
    OLED_write_command(0x20); // Set Memory Addressing Mode
    OLED_write_command(0x02); // Page
}

void OLED_goto_col(uint8_t col)
{
    OLED_write_command(0x00 + (col % 16));
    OLED_write_command(0x10 + (col / 16));
}

void OLED_goto_page(uint8_t page)
{
    OLED_write_command(0xB0 + page);
}

void OLED_goto_pos(uint8_t page, uint8_t col)
{
    OLED_write_command(0xB0 + page);       // Sets page
    OLED_write_command(0x00 + (col % 16)); // Sets column lower nibble
    OLED_write_command(0x10 + (col / 16)); // Sets column higher nibble
}

void OLED_clear_page(uint8_t page)
{
    OLED_goto_page(page);
    for (int i = 0; i < 128; i++)
    {
        OLED_write_data(0x00);
    }
}

void OLED_print_char(char c)
{
    for (int i = 0; i < 8; i++)
    {
        OLED_write_data(pgm_read_byte(&(font8[c - 32][i])));
    }
}

void OLED_print_str(char str[])
{
    for (int i = 0; i < strlen(str); i++)
    {
        OLED_print_char(str[i]);
    }
}
void OLED_print_inv_str(char str[])
{

    for (int i = 0; i < 128; i++)
    {
        OLED_write_data(0xff);
    }
    for (int i = 0; i < strlen(str); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            uint8_t data = pgm_read_byte(&(font8[str[i] - 32][j]));
            data = ~data;
            OLED_write_data(data);
        }
    }
}

void OLED_set_brightness(uint8_t lvl)
{
    OLED_write_command(0x81);
    OLED_write_command(lvl);
}
void OLED_home();
