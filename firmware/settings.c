#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "settings.h"
#include "Microchip/usb_device.h"


void settings_init() {
    uint8_t* settingsPtr = (uint8_t*)&Settings;
    for (uint8_t i = 0; i < sizeof(Settings); i++) {
        *settingsPtr = _SETTINGS_PROGRAM[i];
        settingsPtr++;
    }
}

void settings_save() {
    bool hadInterruptsEnabled = (INTCONbits.GIE != 0);  // save if interrupts enabled
    INTCONbits.GIE = 0;  // disable interrupts
    PMCON1bits.WREN = 1;  // enable writes

    uint16_t address = _SETTINGS_FLASH_LOCATION;
    uint8_t* settingsPtr = (uint8_t*)&Settings;

    // erase
    PMADR = address;         // set location
    PMCON1bits.CFGS = 0;     // program space
    PMCON1bits.FREE = 1;     // erase
    PMCON2 = 0x55;           // unlock
    PMCON2 = 0xAA;           // unlock
    PMCON1bits.WR = 1;       // begin erase
    asm("NOP"); asm("NOP");  // forced

    // write
    for (uint8_t i = 1; i <= sizeof(Settings); i++) {
        unsigned latched = (i == sizeof(Settings)) ? 0 : 1;  // latch load is done for all except last
        PMADR = address;            // set location
        PMDATH = 0x3F;              // same as when erased
        PMDATL = *settingsPtr;      // load data
        PMCON1bits.CFGS = 0;        // program space
        PMCON1bits.LWLO = latched;  // load write latches
        PMCON2 = 0x55;              // unlock
        PMCON2 = 0xAA;              // unlock
        PMCON1bits.WR = 1;          // begin write
        asm("NOP"); asm("NOP");     // forced
        address++;                  // move write address
        settingsPtr++;              // move data pointer
    }

    PMCON1bits.WREN = 0;  // disable writes
    if (hadInterruptsEnabled) { INTCONbits.GIE = 1; }  // restore interrupts
}


uint8_t settings_getSmoothing() {
    return Settings.Smoothing;
}

void settings_setSmoothing(uint8_t newValue) {
    Settings.Smoothing = newValue;
}


bool settings_getStartEnabled() {
    return (Settings.Flags & 0x01) != 0;
}

void settings_setStartEnabled(bool newValue) {
    if (newValue) {
        Settings.Flags |= 0x01;
    } else {
        Settings.Flags &= ~0x01;
    }
}


uint8_t settings_getZeroCurrentAdc() {
    return Settings.ZeroCurrentAdc;
}

void settings_setZeroCurrentAdc(uint8_t newValue) {
    Settings.ZeroCurrentAdc = newValue;
}
