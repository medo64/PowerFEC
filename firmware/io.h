#pragma once
#include <xc.h>
#include "app.h"

#if defined(BOARD_REV_A)
    #define io_led_in_on()       TRISA5 = 1
    #define io_led_in_off()      TRISA5 = 0
#else
    #define io_led_in_on()       TRISC5 = 1
    #define io_led_in_off()      TRISC5 = 0
#endif

#define io_power_enable()        LATC4 = 0
#define io_power_disable()       LATC4 = 1

void io_init(void);
