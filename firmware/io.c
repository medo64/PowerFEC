#include <xc.h>
#include <stdint.h>
#include "app.h"
#include "io.h"

void io_init(void) {
    // LED_IN
#if defined(BOARD_REV_A)
    LATC5 = 0;
    TRISC5 = 0;  // default state is off (inverse)
#else
    LATA5 = 0;
    TRISA5 = 0;  // default state is off (inverse)
#endif

    // POWER_EN
    LATC4 = 0;
    TRISC4 = 0;  // default state is on
}
