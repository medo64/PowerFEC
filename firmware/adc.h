#pragma once
#include <stdbool.h>

void adc_init(void);
uint16_t adc_getVoltage(void);
uint16_t adc_getCurrent(void);
