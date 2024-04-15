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
        out_led1_on(); out_led2_off(); ticker_waitTicks(3);
        out_led1_off(); out_led2_on(); ticker_waitTicks(3);
    }
    out_led2_off();


    // ADC offset
    if (settings_getZeroCurrentAdc() == 0xFF) {  // first time after programming set offset
        watchdog_clear();
        uint16_t value = adc_getRawCurrent();
        if (value < 0xFF) {
            settings_setZeroCurrentAdc((uint8_t)value);
            settings_save();
        }

        while (true) {
            watchdog_clear();
            out_led1_on();  out_led1_on();  ticker_waitTicks(3);
            out_led1_off(); out_led2_off(); ticker_waitTicks(3);
        }
    }
    adc_setCurrentOffset(settings_getZeroCurrentAdc());


    watchdog_clear();
    execUsb();
}
