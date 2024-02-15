#include <xc.h>
#include <stdint.h>
#include "app.h"
#include "appUsb.h"
#include "adc.h"
#include "io.h"
#include "settings.h"
#include "ticker.h"
#include "watchdog.h"

void init(void) {
    interruptsDisable();

    // Oscillator
    OSCCONbits.IRCF = 0b1111;  // 16 MHz or 48 MHz HF
    OSCCONbits.SPLLMULT = 1;   // 3x PLL is enabled
    OSCCONbits.SPLLEN = 1;     // PLL is enabled
    ACTCONbits.ACTSRC = 1;     // The HFINTOSC oscillator is tuned using Fll-speed USB events
    ACTCONbits.ACTEN = 1;      // ACT is enabled, updates to OSCTUNE are exclusive to the ACT
}

void main(void) {
    init();
    io_init();
    adc_init();
    ticker_init();
    settings_init();
    watchdog_init();

    for (uint8_t i = 0; i < 3; i++) {
        io_led_in_on(); ticker_waitTicks(3);
        io_led_in_off(); ticker_waitTicks(3);
    }

    watchdog_clear();
    execUsb();
}
