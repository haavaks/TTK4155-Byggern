#include <inttypes.h>

void OLED_write_command(uint8_t command);
void OLED_write_data(uint8_t data);
void OLED_init();
void OLED_home();
void OLED_set_col(uint8_t col_start, uint8_t col_end);
void OLED_clear_col(uint8_t col);
void OLED_pos(uint8_t row, uint8_t column);
void OLED_print(char *data);
void OLED_set_brightness(uint8_t lvl);
void OLED_fill_screen();
void OLED_clear_screen();