#pragma once
#include <stdbool.h>

void adc_init(void);
uint16_t adc_getVoltage(void);
uint16_t adc_getRawCurrent(void);
uint16_t adc_getCurrent(void);
void adc_setCurrentOffset(uint8_t offset);
