#include <inttypes.h>

void OLED_write_command(uint8_t command);
void OLED_write_data(uint8_t data);
void OLED_init();
void OLED_home();
void OLED_goto_col(uint8_t col);
void OLED_goto_page(uint8_t page);
void OLED_clear_page(uint8_t page);
void OLED_goto_pos(uint8_t page, uint8_t col);
void OLED_print_str(char str[]);
void OLED_print_inv_str(char str[]);
void OLED_set_brightness(uint8_t lvl);
void OLED_fill_screen();
void OLED_clear_screen();
void OLED_print_char(char c);