#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

#include "uart.h"
#include "can.h"
#include "time.h"

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

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

    can_init((CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1, .sjw = 0, .smp = 0}, 0);
    // can_init((CanInit){0x00290561}, 0);

    // Uncomment after including uart above
    uart_init(F_CPU, 9600);

    CanMsg msg;

    printf("Ready\r\n");

    while (1)
    {
        if (can_rx(&msg))
        {
            printf("Message\r\n");
            can_printmsg(msg);
        }
        // time_spinFor(msecs(500));
        // printf("No message\r\n");
    }

    // PMC->PMC_PCER0 |= (1 << ID_PIOB);

    // PIOB->PIO_PER |= PIO_PB13;
    // PIOB->PIO_OER |= PIO_PB13;
    // PIOB->PIO_SODR |= PIO_PB13;

    // while (1)
    // {
    //     PIOB->PIO_SODR |= PIO_PB13;
    //     PIOB->PIO_CODR |= PIO_PB13;
    // }
}