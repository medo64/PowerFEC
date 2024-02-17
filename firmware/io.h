#pragma once
#include <xc.h>
#include <stdbool.h>
#include "app.h"
#include "adc.h"

// Out: LED_IN
#if defined(BOARD_REV_A)
#define out_led1_on()          TRISAbits.TRISA5 = 1
#define out_led1_off()         TRISAbits.TRISA5 = 0
#else
#define out_led1_on()          TRISCbits.TRISC5 = 1
#define out_led1_off()         TRISCbits.TRISC5 = 0
#endif

// Out: LED_OUT  // not present on rev A but it doesn't hurt
#define out_led2_on()          LATCbits.LATC2 = 1
#define out_led2_off()         LATCbits.LATC2 = 0

// Out: POWER_EN#
#define out_power_enable()     LATCbits.LATC4 = 0
#define out_power_disable()    LATCbits.LATC4 = 1
#define out_power_isEnabled()  (LATCbits.LATC4 == 0)

// In: FAULT#
#if defined(BOARD_REV_A)
bool in_isFaulted(void) {
    if (!out_power_isEnabled()) { return false; }  // not a fault if it's disabled
    uint16_t voltage = adc_getVoltage();
    return (voltage <= 5);  // small enough not to cause falst positives
}
#else
#define in_isFaulted()         (PORTCbits.RA5 == 0)
#endif

void io_init(void);
