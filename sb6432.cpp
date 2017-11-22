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

#include "sb6432.h"

#define CMD_WRITE_DATA       0x40
#define CMD_SET_RGB_COLOR    0x42
#define CMD_END_TRANSMISSION 0x43

const uint8_t POS_MAP[4] = {1, 0, 3, 2};

SB6432::SB6432(uint8_t clockPin, uint8_t dataPin) :
    MonoGfx(64, 32),
    _buffer(new uint8_t[256]),
    _clockPin(clockPin),
    _dataPin(dataPin) {
}

void SB6432::begin() {
    pinMode(_dataPin, OUTPUT);
    digitalWrite(_dataPin, HIGH);
    pinMode(_clockPin, OUTPUT);
    digitalWrite(_clockPin, HIGH);
    // wait for device to initialize
    delay(10);
}

void SB6432::doDrawPixel(uint8_t x, uint8_t y, uint8_t mode) {
    uint8_t bit = 0x80 >> (y % 8);
    uint8_t pos = POS_MAP[y / 8] + 4 * x;
    switch (mode) {
        case MODE_SET:
            _buffer[pos] = _buffer[pos] | bit;
            break;
        case MODE_CLEAR:
            _buffer[pos] = _buffer[pos] & ~bit;
            break;
        case MODE_INVERT:
            _buffer[pos] = _buffer[pos] ^ bit;
            break;
    }
}

void SB6432::doSetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
    sendByte(CMD_SET_RGB_COLOR);
    sendByte(red >> 1);
    sendByte(green >> 1);
    sendByte(blue >> 1);
    sendByte(CMD_END_TRANSMISSION);
}

void SB6432::doUpdate(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom) {
    sendByte(0x47);
    sendByte(0x7F);
    sendByte(0xFF);
    for (uint16_t i = 0; i < 256; ++i) {
        sendByte(_buffer[i]);
    }

    sendByte(CMD_END_TRANSMISSION);
}

void SB6432::sendByte(uint8_t data) {
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

    digitalWrite(_dataPin, HIGH);
}

