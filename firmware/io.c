#include <xc.h>
#include <stdint.h>
#include "app.h"
#include "io.h"

void io_init(void) {
    // LED_IN
    LATA5 = 0;
    TRISA5 = 0;  // default state is off (inverse))

    // POWER_EN
    LATC4 = 0;
    TRISC4 = 0;  // default state is on
}
