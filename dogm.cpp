/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2017, Stefan Rothe
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

#include "dogm.h"

#define CMD_SET_PAGE_ADDRESS           B01000000 // 0x40
#define CMD_SET_ADC                    B10100000 // 0xA0
#define CMD_SET_COMMON_OUTPUT_MODE     B11000000 // 0xC0
#define CMD_SET_DISPLAY_ORIENTATION    B10100110 // 0xA6
#define CMD_SET_LCD_BIAS_RATIO         B10100010 // 0xA2
#define CMD_SET_POWER_CONTROL          B00101000 // 0x28
#define CMD_SET_BOOSTER_RATIO          B11111000 // 0xF8
#define CMD_SWITCH_OFF                 B10101110 // 0xAE
#define CMD_SWITCH_ON                  B10101111 // 0xAF
#define CMD_SET_PAGE                   B10110000 // 0xB0

uint8_t inline powerControlCmd(bool booster, bool regulator, bool follower) {
    uint8_t cmd = CMD_SET_POWER_CONTROL;
    if (booster) {
        cmd |= 4;
    }

    if (regulator) {
        cmd |= 2;
    }

    if (follower) {
        cmd |= 1;
    }

    return cmd;
}

DOGM::DOGM(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin, uint8_t clockPin, uint8_t dataPin,
           uint8_t columns, uint8_t pages) :
    MonoGfx(columns, 8 * pages),
    _buffer(new uint8_t[columns * pages]),
    _a0Pin(a0Pin),
    _clockPin(clockPin),
    _columns(columns),
    _csPin(csPin),
    _dataPin(dataPin),
    _pages(pages),
    _resetPin(resetPin) {
}

DOGM132::DOGM132(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin, uint8_t clockPin, uint8_t dataPin) :
    DOGM(csPin, resetPin, a0Pin, clockPin, dataPin, 132, 4) {
}

void DOGM::begin() {
    pinMode(_a0Pin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_csPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);
    pinMode(_resetPin, OUTPUT);
    // reset pin is active low
    digitalWrite(_resetPin, HIGH);
    // chip select pin is active low
    digitalWrite(_csPin, HIGH);
    // perform hardware reset
    digitalWrite(_resetPin, LOW);
    delayMicroseconds(10);
    digitalWrite(_resetPin, HIGH);
    delay(1);
    // initialize display (see datasheet page 6)
    beginTransferCmd();
    sendByte(CMD_SET_PAGE_ADDRESS); // set ram page address to 0
    sendByte(CMD_SET_ADC | 1); // set ADC reverse
    sendByte(CMD_SET_COMMON_OUTPUT_MODE); // set normal direction common output mode
    sendByte(CMD_SET_DISPLAY_ORIENTATION); // set normal display orientation
    sendByte(CMD_SET_LCD_BIAS_RATIO); // set LCD bias to 1/9
    sendByte(powerControlCmd(true, true, true)); // turn on booster, regulator, and follower
    sendByte(CMD_SET_BOOSTER_RATIO);
    sendByte(0); // set booster ratio to 3x/4x

    // TOOD: document
    sendByte(0x23);
    sendByte(0x81);
    sendByte(0x1F);
    sendByte(0xAC);
    sendByte(0x00);
    sendByte(CMD_SWITCH_ON);
    endTransfer();
}

void DOGM::setPowerControl(bool booster, bool regulator, bool follower) {
    beginTransferCmd();
    sendByte(powerControlCmd(booster, regulator, follower));
    endTransfer();
}

void DOGM::showBuffer() {
    // clock polarity is 1 (SPI mode 3)
    beginTransferCmd();
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_csPin, LOW);

    for (uint8_t page = 0; page < _pages; ++page) {
        sendByte(CMD_SET_PAGE | page); // select page
        digitalWrite(_a0Pin, HIGH); // set A0 to 1 to transfer data
        for (uint8_t column = 0; column < _columns; ++column) {
            sendByte(_buffer[_columns * page + column]);
        }

        digitalWrite(_a0Pin, LOW);
    }
}

void DOGM::switchOn() {
    beginTransferCmd();
    sendByte(CMD_SWITCH_ON);
    endTransfer();
}

void DOGM::switchOff() {
    beginTransferCmd();
    sendByte(CMD_SWITCH_OFF);
    endTransfer();
}

void DOGM::doDrawPixel(uint8_t x, uint8_t y, uint8_t mode) {
    uint8_t bit = y % 8;
    uint8_t pos = _columns * y / 8 + x;
    switch (mode) {
        case MODE_SET:
            _buffer[pos] = _buffer[pos] | _BV(bit);
            break;
        case MODE_CLEAR:
            _buffer[pos] = _buffer[pos] & ~_BV(bit);
            break;
        case MODE_INVERT:
            _buffer[pos] = _buffer[pos] ^ _BV(bit);
            break;
    }
}

void DOGM::beginTransferCmd() {
    // clock polarity is 1 (SPI mode 3)
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_csPin, LOW);
    digitalWrite(_a0Pin, LOW);
}

void DOGM::endTransfer() {
    digitalWrite(_csPin, HIGH);
}

void DOGM::sendByte(uint8_t data) {
    uint8_t mask = 128;
    while (mask > 0) {
        if ((data & mask) == mask) {
            digitalWrite(_dataPin, HIGH);
        }
        else {
            digitalWrite(_dataPin, LOW);
        }

        digitalWrite(_clockPin, LOW);
        delayMicroseconds(2);
        digitalWrite(_clockPin, HIGH);
        delayMicroseconds(2);
        mask >>= 1;
    }
}

