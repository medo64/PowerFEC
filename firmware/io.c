#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "io.h"

#if defined(BOARD_REV_A)
    #include "adc.h"
#include "Microchip/usb_device.h"
#endif

void io_init(void) {
    // Out: LED1 (left)
#if defined(BOARD_REV_A)
    LATCbits.LATC5 = 0;
    TRISCbits.TRISC5 = 0;  // default state is off (inverse)
#else
    LATAbits.LATA5 = 0;
    TRISAbits.TRISA5 = 0;  // default state is off (inverse)
#endif

    // Out: LED2 (right)
    LATCbits.LATC2 = 1;    // default state is off
    TRISCbits.TRISC2 = 0;
    ANSELCbits.ANSC2 = 0;

    // Out: POWER_EN#
    LATCbits.LATC4 = 0;
    TRISCbits.TRISC4 = 0;  // default state is on

    // In: FAULT#
#if !defined(BOARD_REV_A)
    WPUAbits.WPUA5 = 1;    // weak pull-up enabled
    LATAbits.LATA5 = 0;
    TRISAbits.TRISA5 = 1;  // input
#endif
}
