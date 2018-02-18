/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2017 - 2018, Stefan Rothe
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ssd1306.h"
#include <Wire.h>

#define CONTROL 0x00
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

#define CMD_MEMORY_MODE           0x20
#define CMD_SET_COLUMN_ADDRESS    0x21
#define CMD_SET_PAGE_ADDRESS      0x22
#define CMD_DEACTIVATE_SCROLL     0x2E
#define CMD_SET_CONTRAST          0x81
#define CMD_CHARGE_PUMP           0x8D
#define CMD_SET_START_LINE        0x40
#define CMD_SEGRE_MAP             0xA0
#define CMD_TEST_OFF              0xA4
#define CMD_TEST_ON               0xA5
#define CMD_INVERT_OFF            0xA6
#define CMD_INVERT_ON             0xA7
#define CMD_SET_MULTIPLEX         0xA8
#define CMD_SET_DISPLAY_SLEEP     0xAE
#define CMD_SET_DISPLAY_ON        0xAF
#define CMD_SET_DISPLAY_OFFSET    0xD3
#define CMD_SET_DISPLAY_CLOCK_DIV 0xD5
#define CMD_SET_PRECHARGE         0xD9
#define CMD_SET_COM_PINS          0xDA
#define CMD_SET_VCOM_DETECT       0xDB
#define CMD_COM_SCAN_DEC          0xC8

SSD1306::SSD1306(uint8_t i2cAddress) :
    MonoGfx(DISPLAY_WIDTH, DISPLAY_HEIGHT),
    _i2cAddress(i2cAddress) {
}

void SSD1306::setContrast(uint8_t contrast) {
    sendCommand(0x81);
    sendCommand(contrast);
}

void SSD1306::setDisplayOffset(uint8_t offset) {
    sendCommand(0xD3);
    sendCommand(offset & B00111111);
}

void SSD1306::setInvert(bool enable) {
    sendCommand(enable ? CMD_INVERT_ON : CMD_INVERT_OFF);
}

void SSD1306::test(boolean enable) {
    sendCommand(enable ? CMD_TEST_ON : CMD_TEST_OFF);
}

void SSD1306::turnOn() {
    sendCommand(CMD_SET_DISPLAY_ON);
}

void SSD1306::turnOff() {
    sendCommand(CMD_SET_DISPLAY_SLEEP);    
}

bool SSD1306::doInitialize() {
    Serial.println("1306: init 1");
    Serial.println(_i2cAddress);
    Wire.beginTransmission(_i2cAddress);
    Serial.println(Wire.endTransmission());

    Serial.println("1306: init 2");
    sendCommand(CMD_SET_DISPLAY_SLEEP);

    sendCommand(CMD_SET_DISPLAY_CLOCK_DIV);
    sendCommand(0x80);

    sendCommand(CMD_SET_MULTIPLEX);
    sendCommand(DISPLAY_HEIGHT - 1);

    sendCommand(CMD_SET_DISPLAY_OFFSET);
    sendCommand(0x0);

    sendCommand(CMD_SET_START_LINE | 0x0);

    sendCommand(CMD_CHARGE_PUMP);
    bool _externalVcc = false;
    sendCommand(_externalVcc ? 0x10 : 0x14);

    sendCommand(CMD_MEMORY_MODE);
    sendCommand(0x00);

    sendCommand(CMD_SEGRE_MAP | 0x1);

    sendCommand(CMD_COM_SCAN_DEC);

    sendCommand(CMD_SET_COM_PINS);
    sendCommand(0x02);
    
    sendCommand(CMD_SET_CONTRAST);
    sendCommand(0x8F);

    sendCommand(CMD_SET_PRECHARGE);
    sendCommand(_externalVcc ? 0x22 : 0xF1);
    
    sendCommand(CMD_SET_VCOM_DETECT);
    sendCommand(0x40);
  
    sendCommand(CMD_TEST_OFF);
    
    sendCommand(CMD_INVERT_OFF);

    sendCommand(CMD_DEACTIVATE_SCROLL);

    sendCommand(CMD_SET_DISPLAY_ON);
    return true;
}

void SSD1306::doUpdate(uint8_t* buffer, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) {
    sendCommand(CMD_SET_COLUMN_ADDRESS);
    sendCommand(0);
    sendCommand(DISPLAY_WIDTH - 1);
    sendCommand(CMD_SET_PAGE_ADDRESS);
    sendCommand(0);

    uint16_t bytes = (DISPLAY_WIDTH * DISPLAY_HEIGHT / 8);
    uint16_t pos = 0;
    while (pos < bytes) {
        Wire.beginTransmission(_i2cAddress);
        Wire.write(0x40);
        for (uint16_t i = 0; i < 8; ++i) {
            Wire.write(buffer[pos]);
            ++pos;
        }

        Wire.endTransmission();
    }
}

void SSD1306::sendCommand(uint8_t command) const {
    Wire.beginTransmission(_i2cAddress);
    Wire.write(0x00);
    Wire.write(command);
    Wire.endTransmission();
}

