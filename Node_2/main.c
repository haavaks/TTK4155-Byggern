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
    //PWM_stop();

    volatile CanMsg rx_msg;
    volatile CanMsg tx_msg;
    GameState state = Not_playing;

    adc_init();
    uint32_t adc;

    timer_init();
    uint32_t time;

    Encoder_init();

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
                    //PWM_start();
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

                // printf("Y-pos:  %5d", rx_msg.byte[1]);
                // printf("  Button: %5d", rx_msg.byte[3]);
                // printf("  x-pos %5d\n\r", rx_msg.byte[0]);

                // Send score back to Node1
                tx_msg.id = Playing;
                tx_msg.length = sizeof(time);
                tx_msg.dword[0] = time;
                can_tx(tx_msg);
            }

            if (ADC_game_over())
            {
                printf("GAME OVER 1\n\r");
                timer_stop();
                //PWM_stop();

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