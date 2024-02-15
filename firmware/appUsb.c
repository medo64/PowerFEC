#include <stdbool.h>
#include "Microchip/usb.h"
#include "Microchip/usb_device.h"
#include "Microchip/usb_device_cdc.h"
#include "adc.h"
#include "buffer.h"
#include "io.h"
#include "ticker.h"
#include "watchdog.h"

uint32_t voltageSum = 0;
uint32_t currentSum = 0;

void execUsb(void) {
    USBDeviceInit();
    USBDeviceAttach();

    io_power_enable();
    io_led_in_on();

    while(true) {
        watchdog_clear();

        if (ticker_hasTicked()) { io_led_in_on(); }

        USBDeviceTasks();

        if (USBGetDeviceState() < CONFIGURED_STATE) { continue; }
        if (USBIsDeviceSuspended()) { continue; }

        CDCTxService();

        // USB receive
        uint8_t readCount = getsUSBUSART(UsbReadBuffer, USB_READ_BUFFER_MAX); //until the buffer is free.
        if (readCount > 0) {
            io_led_in_off(); ticker_reset();
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
            io_led_in_off(); ticker_reset();
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

        // Process data (ignore for now)
        if (InputBufferCount > 0) {
            //for (uint8_t i = 0; i < InputBufferCount; i++) { OutputBufferAppend(InputBuffer[i]); }  // test echo
            InputBufferCount = 0;  // reset counter
        }

        // Send measurements
        if (USBUSARTIsTxTrfReady()) {
            if (OutputBufferCount == 0) {
                uint16_t instantVoltage = adc_getVoltage();
                uint16_t instantCurrent = adc_getCurrent();
                if (instantVoltage > 9999) { instantVoltage = 9999; }
                if (instantCurrent > 9999) { instantCurrent = 9999; }

                // moving average
                if (voltageSum == 0) {
                    voltageSum = instantVoltage * 60;
                } else {
                    voltageSum -= (voltageSum / 60);
                    voltageSum += instantVoltage;
                }
                uint32_t voltage = (voltageSum / 60);
                
                // moving average
                if (currentSum == 0) {
                    currentSum = instantCurrent * 60;
                } else {
                    currentSum -= (currentSum / 60);
                    currentSum += instantCurrent;
                }
                uint32_t current = (currentSum / 60);
                
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
                OutputBufferAppend(0x0D);
                OutputBufferAppend(0x0A);
            }
        }
    }
}