#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "sam.h"

#include "uart.h"
#include "can.h"
#include "time.h"
#include "PWM.h"
#include "adc.h"
#include "timer.h"
#include "encoder.h"
#include "controller.h"
#include "solenoid.h"

#define F_CPU 84000000
/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"

typedef enum
{
    Playing = 0,
    Not_playing = 1
} GameState;

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw = 0, .smp = 0}, 0);

    uart_init(F_CPU, 9600);

    PWM_init();
    PWM_start();

    volatile CanMsg rx_msg;
    volatile CanMsg tx_msg;
    GameState state = Not_playing;

    adc_init();
    uint32_t adc;

    timer_init();
    uint32_t time;

    Encoder_init();
    int32_t encoder = 0;

    Solenoid_init();

    while (1)
    {
        switch (state)
        {
        case Not_playing:
            if (can_rx(&rx_msg))
            {
                if (rx_msg.id == Start_game_id)
                {
                    printf("STARTING GAME\r\n");

                    state = Playing;

                    // PWM_start();
                    reset_ADC_isr();
                    timer_start();
                }
            }
            break;

        case Playing:

            if (can_rx(&rx_msg))
            {
                update_joystick_pos_from_CAN(rx_msg);
                if (rx_msg.byte[3] == 0)
                {
                    Solenoid_fire();
                    printf("FIRE\n\r");
                }

                printf("Slider pos:  %5d", rx_msg.byte[2]);
                printf("Button: %5d", rx_msg.byte[3]);
                printf("x-pos %5d\n\r", rx_msg.byte[0]);

                // sende tilbake klokke til node1
                tx_msg.id = Playing;
                tx_msg.length = sizeof(time);
                tx_msg.dword[0] = time;
                can_tx(tx_msg);
            }

            // encoder = REG_TC2_CV0;
            // printf("Encoder: %d\n\r", encoder);

            if (ADC_game_over())
            {
                timer_stop();
                // PWM_stop();

                state = Not_playing;
                time = timer_value() / 10500000;
                printf("GAME OVER\n\r");
                printf("Time: %d\n\r", time);
                tx_msg.id = Game_over_id;
                tx_msg.length = sizeof(time);
                tx_msg.dword[0] = time;
                can_tx(tx_msg);
            }
            break;
        }
    }
}