#include <xc.h>
#include <stdint.h>
#include "app.h"
#include "io.h"

void io_init(void) {
    //LED: In
    LATA5 = 0;
    TRISA5 = 0;  // default state is off
}
