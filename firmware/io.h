#pragma once
#include <xc.h>
#include <stdbool.h>
#include "app.h"

// Out: LED_IN
#if defined(BOARD_REV_A)
    #define out_led_on()       TRISAbits.TRISA5 = 1
    #define out_led_off()      TRISAbits.TRISA5 = 0
#else
    #define out_led_on()       TRISCbits.TRISC5 = 1
    #define out_led_off()      TRISCbits.TRISC5 = 0
#endif

// Out: POWER_EN#
#define out_power_enable()     LATCbits.LATC4 = 0
#define out_power_disable()    LATCbits.LATC4 = 1
#define out_power_isEnabled()  (LATCbits.LATC4 == 0)

// In: FAULT#
#if !defined(BOARD_REV_A)
    #define in_isFaulted()     (PORTCbits.RA5 == 0)
#else
    bool in_isFaulted(void);
#endif

void io_init(void);
