#include <xc.h>
#include <stdbool.h>
#include "adc.h"
#include "Microchip/usb_device.h"

#define ADC_VOLTAGE_BIT_MULTIPLIER  6034  // should be 6100
#define ADC_CURRENT_BIT_MULTIPLIER  1500  // should be 1000

void adc_init(void) {
    FVRCONbits.ADFVR = 0b01;     // ADC Fixed Voltage Reference Peripheral output is 1x (1.024V)
    FVRCONbits.FVREN = 1;        // Fixed Voltage Reference is enabled
    while (!FVRCONbits.FVRRDY);  // Fixed Voltage Reference output is ready for use

    ADCON1bits.ADFM = 1;         // right justified
    ADCON1bits.ADCS = 0b111;     // dedicated RC oscillator
    ADCON1bits.ADPREF = 0b11;    // VREF+ is connected to FVR

    ADCON0bits.CHS = 0b111;      // select FVR
    ADCON0bits.ADON = 1;         // ADC is enabled
}

uint16_t adc_getRaw(uint8_t channel) {
    ADCON0bits.CHS = 0b111;       // select FVR
    ADCON0bits.GO_nDONE = 1;      // start an A/D conversion cycle
    while (ADCON0bits.GO_nDONE);  // wait for conversion to complete

    ADCON0bits.CHS = channel;     // select channel
    ADCON0bits.GO_nDONE = 1;      // start an A/D conversion cycle
    while (ADCON0bits.GO_nDONE);  // wait for conversion to complete
    uint16_t value1 = ADRES;

    ADCON0bits.GO_nDONE = 1;      // start an A/D conversion cycle
    while (ADCON0bits.GO_nDONE);  // wait for conversion to complete
    uint16_t value2 = ADRES;

    ADCON0bits.CHS = 0b111;       // select FVR

    uint16_t result = (value1 + value2 + 1) / 2;
    return result;
}

uint16_t adc_getVoltage() {
    uint32_t value = adc_getRaw(7);
    value *= ADC_VOLTAGE_BIT_MULTIPLIER;
    value /= 1000;
    return (uint16_t)value;
}

uint16_t adc_getCurrent() {
    uint32_t value = adc_getRaw(3);
    value *= ADC_CURRENT_BIT_MULTIPLIER;
    value /= 1000;
    return (uint16_t)value;
}
