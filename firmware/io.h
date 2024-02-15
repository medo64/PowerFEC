#pragma once
#include <xc.h>

#define io_led_in_on()           TRISA5 = 1
#define io_led_in_off()          TRISA5 = 0

#define io_power_enable()        LATC4 = 0
#define io_power_disable()       LATC4 = 1

void io_init(void);