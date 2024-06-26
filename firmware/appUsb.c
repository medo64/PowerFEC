#include <stdbool.h>
#include "Microchip/usb.h"
#include "Microchip/usb_device.h"
#include "Microchip/usb_device_cdc.h"
#include "adc.h"
#include "buffer.h"
#include "io.h"
#include "ticker.h"
#include "watchdog.h"
#include "settings.h"

uint24_t voltageSum = 0xFFFFFF;
uint24_t currentSum = 0xFFFFFF;

void processInput(uint8_t b, uint8_t *smoothingPower, bool* shouldEnable);

void execUsb(void) {
    USBDeviceInit();
    USBDeviceAttach();

    out_power_enable();
    out_led1_on();
    out_led2_on();

    uint8_t smoothingPower = settings_getSmoothing();
    bool shouldEnable = settings_getStartEnabled();

    while(true) {
        watchdog_clear();
        bool hasTicked = ticker_hasTicked();
        if (out_power_isEnabled()) {
            out_led1_on();
            out_led2_on();
        } else {
            out_led1_off();
            out_led2_off();
        }

        USBDeviceTasks();

        if (USBGetDeviceState() < CONFIGURED_STATE) { continue; }
        if (USBIsDeviceSuspended()) { continue; }

        CDCTxService();

        // USB receive
        uint8_t readCount = getsUSBUSART(UsbReadBuffer, USB_READ_BUFFER_MAX); //until the buffer is free.
        if (readCount > 0) {
            for (uint8_t i = 0; i < readCount; i++) {  // copy to buffer
                uint8_t value = UsbReadBuffer[i];
                if (InputBufferCorrupted && ((value == 0x0A) || (value == 0x0D))) {
                    InputBufferCount = 0;  // clear the whole buffer
                    InputBufferCorrupted = false;
                } else if (InputBufferCount < INPUT_BUFFER_MAX) {
                    InputBuffer[InputBufferCount] = value;
                    InputBufferCount++;
                } else {
                    InputBufferCorrupted = true;  // no more buffer; darn it
                }
            }
        }

        // USB send
        if ((OutputBufferCount > 0) && USBUSARTIsTxTrfReady()) {  // send output if TX is ready
            uint8_t writeCount = 0;
            for (uint8_t i = 0; i < USB_WRITE_BUFFER_MAX; i++) {  // copy to output buffer
                if (i < OutputBufferCount) {
                    UsbWriteBuffer[i] = OutputBuffer[i];
                    writeCount++;
                } else {
                    break;
                }
            }
            if (writeCount > 0) {
                putUSBUSART(&UsbWriteBuffer[0], writeCount);  // send data
                OutputBufferCount -= writeCount;  // reduce buffer for the length used
                buffer_copy(&OutputBuffer[0], &OutputBuffer[writeCount], OutputBufferCount);  // move buffer content to beginning
            }
        }

        // Process data
        if (InputBufferCount > 0) {
            for (uint8_t i = 0; i < InputBufferCount; i++) {
                //OutputBufferAppend(InputBuffer[i]);  // test echo
                uint8_t b = InputBuffer[i];
                processInput(b, &smoothingPower, &shouldEnable);
            }
            InputBufferCount = 0;  // reset counter
        }

        // Set enabled state
        if (shouldEnable) { out_power_enable(); } else { out_power_disable(); }

        // Check for fault
        bool isFaulted = in_isFaulted();
        bool isEnabled = out_power_isEnabled();
        if (hasTicked) {
            if (isFaulted && isEnabled) {
                out_power_disable();
            } else {
                out_power_enable();
            }
        }

        // Send measurements
        if (USBUSARTIsTxTrfReady()) {
            // measure 
            uint24_t instantVoltage = adc_getVoltage();
            uint24_t instantCurrent = adc_getCurrent();
            if (instantVoltage > 9999) { instantVoltage = 9999; }
            if (instantCurrent > 9999) { instantCurrent = 9999; }

            // moving average for voltage
            if (voltageSum == 0xFFFFFF) {
                voltageSum = (instantVoltage << smoothingPower);
            } else {
                voltageSum -= (voltageSum >> smoothingPower);
                voltageSum += instantVoltage;
            }

            // moving average for current
            if (currentSum == 0xFFFFFF) {
                currentSum = (instantCurrent << smoothingPower);
            } else {
                currentSum -= (currentSum >> smoothingPower);
                currentSum += instantCurrent;
            }

            // send when existing data was already sent
            if (OutputBufferCount == 0) {
                uint16_t voltage = (uint16_t)(voltageSum >> smoothingPower);
                uint16_t current = (uint16_t)(currentSum >> smoothingPower);

                uint8_t voltage4 = (voltage % 10); voltage /= 10;
                uint8_t voltage3 = (voltage % 10); voltage /= 10;
                uint8_t voltage2 = (voltage % 10); voltage /= 10;
                uint8_t voltage1 = (uint8_t)voltage;

                uint8_t current4 = (current % 10); current /= 10;
                uint8_t current3 = (current % 10); current /= 10;
                uint8_t current2 = (current % 10); current /= 10;
                uint8_t current1 = (uint8_t)current;

                OutputBufferAppend('0' + voltage1);
                OutputBufferAppend('.');
                OutputBufferAppend('0' + voltage2);
                OutputBufferAppend('0' + voltage3);
                OutputBufferAppend('0' + voltage4);
                OutputBufferAppend(' ');
                OutputBufferAppend('0' + current1);
                OutputBufferAppend('.');
                OutputBufferAppend('0' + current2);
                OutputBufferAppend('0' + current3);
                OutputBufferAppend('0' + current4);
                if (!isEnabled) {
                    OutputBufferAppend(' ');
                    OutputBufferAppend('O');
                    OutputBufferAppend('F');
                    OutputBufferAppend('F');
                } else if (isFaulted) {
                    OutputBufferAppend(' ');
                    OutputBufferAppend('F');
                    OutputBufferAppend('A');
                    OutputBufferAppend('U');
                    OutputBufferAppend('L');
                    OutputBufferAppend('T');
                }
                //if (hasTicked) {  / debug
                    //OutputBufferAppend(' ');
                    //OutputBufferAppend('T');
                //}
                OutputBufferAppend(0x0D);
                OutputBufferAppend(0x0A);
            }
        }
    }
}

void processInput(uint8_t b, uint8_t *smoothingPower, bool *shouldEnable) {
    bool wasSuccessful = false;
    switch (b) {
        case '0':  // set smoothing
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            *smoothingPower = b - 0x30;
            settings_setSmoothing(*smoothingPower);
            wasSuccessful = true;
            break;
        case 'd':
            *shouldEnable = false;
            wasSuccessful = true;
            break;
        case 'D':
            *shouldEnable = false;
            settings_setStartEnabled(false);
            wasSuccessful = true;
            break;
        case 'e':
            *shouldEnable = true;
            wasSuccessful = true;
            break;
        case 'E':
            *shouldEnable = true;
            settings_setStartEnabled(true);
            wasSuccessful = true;
            break;
        case 'S':  // save
            settings_save();
            wasSuccessful = true;
            break;
    }
    if (wasSuccessful) {
        OutputBufferAppend('~')
        OutputBufferAppend(b)
        OutputBufferAppend(0x0D);
        OutputBufferAppend(0x0A);
    }
}