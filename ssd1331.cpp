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

#include "ssd1331.h"
#include <SPI.h>

#define DISPLAY_WIDTH 96
#define DISPLAY_HEIGHT 64

#define SSD1331_CMD_DRAWLINE        0x21
#define SSD1331_CMD_DRAWRECT        0x22
#define SSD1331_CMD_FILL            0x26
#define SSD1331_CMD_SETCOLUMN       0x15
#define SSD1331_CMD_SETROW          0x75
#define SSD1331_CMD_CONTRASTA       0x81
#define SSD1331_CMD_CONTRASTB       0x82
#define SSD1331_CMD_CONTRASTC       0x83
#define SSD1331_CMD_MASTERCURRENT   0x87
#define SSD1331_CMD_SETREMAP        0xA0
#define SSD1331_CMD_STARTLINE       0xA1
#define SSD1331_CMD_DISPLAYOFFSET   0xA2
#define SSD1331_CMD_NORMALDISPLAY   0xA4
#define SSD1331_CMD_DISPLAYALLON    0xA5
#define SSD1331_CMD_DISPLAYALLOFF   0xA6
#define SSD1331_CMD_INVERTDISPLAY   0xA7
#define SSD1331_CMD_SETMULTIPLEX    0xA8
#define SSD1331_CMD_SETMASTER       0xAD
#define SSD1331_CMD_DISPLAYOFF      0xAE
#define SSD1331_CMD_DISPLAYON       0xAF
#define SSD1331_CMD_POWERMODE       0xB0
#define SSD1331_CMD_PRECHARGE       0xB1
#define SSD1331_CMD_CLOCKDIV        0xB3
#define SSD1331_CMD_PRECHARGEA      0x8A
#define SSD1331_CMD_PRECHARGEB      0x8B
#define SSD1331_CMD_PRECHARGEC      0x8C
#define SSD1331_CMD_PRECHARGELEVEL  0xBB
#define SSD1331_CMD_VCOMH           0xBE

static uint16_t color(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t c = red >> 3;
    c <<= 6;
    c |= green >> 2;
    c <<= 5;
    c |= blue >> 3;
    return c;
}

SSD1331::SSD1331(uint8_t csPin, uint8_t resetPin, uint8_t dcPin) :
    SSD1331(csPin, resetPin, dcPin, 0, 0) {
}

SSD1331::SSD1331(uint8_t csPin, uint8_t resetPin, uint8_t dcPin, uint8_t clockPin, uint8_t dataPin) :
    MonoGfx(DISPLAY_WIDTH, DISPLAY_HEIGHT),
    _buffer(new uint8_t[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8]),
    _clockPin(clockPin),
    _csPin(csPin),
    _dataPin(dataPin),
    _dcPin(dcPin),
    _hwSpi(clockPin == dataPin),
    _resetPin(resetPin) {
    setForegroundColor(255, 255, 255);
    setBackgroundColor(0, 0, 0);
}

void SSD1331::begin() {
    if (_hwSpi) {
        SPI.begin();
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE3);
        SPI.setClockDivider(SPI_CLOCK_DIV4);
    }
    else {
        pinMode(_clockPin, OUTPUT);
        digitalWrite(_clockPin, HIGH);
        pinMode(_dataPin, OUTPUT);
        digitalWrite(_dataPin, LOW);
        _clockPinMask = digitalPinToBitMask(_clockPin);
        _clockPinReg = portOutputRegister(digitalPinToPort(_clockPin));
        _dataPinMask = digitalPinToBitMask(_dataPin);
        _dataPinReg = portOutputRegister(digitalPinToPort(_dataPin));
    }

    pinMode(_dcPin, OUTPUT);
    digitalWrite(_dcPin, LOW);
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, LOW);

    pinMode(_resetPin, OUTPUT);
    digitalWrite(_resetPin, HIGH);
    delay(500);
    digitalWrite(_resetPin, LOW);
    delay(500);
    digitalWrite(_resetPin, HIGH);
    delay(500);

    sendCommand(SSD1331_CMD_DISPLAYOFF);
    sendCommand(SSD1331_CMD_SETREMAP,0x72);             // RGB Color
    sendCommand(SSD1331_CMD_STARTLINE, 0x0);
    sendCommand(SSD1331_CMD_DISPLAYOFFSET, 0x0);
    sendCommand(SSD1331_CMD_NORMALDISPLAY);
    sendCommand(SSD1331_CMD_SETMULTIPLEX, 0x3F);
    sendCommand(SSD1331_CMD_SETMASTER, 0x8E);
    sendCommand(SSD1331_CMD_POWERMODE, 0x0B);
    sendCommand(SSD1331_CMD_PRECHARGE, 0x31);
    sendCommand(SSD1331_CMD_CLOCKDIV,0xF0);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    sendCommand(SSD1331_CMD_PRECHARGEA, 0x64);
    sendCommand(SSD1331_CMD_PRECHARGEB, 0x78);
    sendCommand(SSD1331_CMD_PRECHARGEC, 0x64);
    sendCommand(SSD1331_CMD_PRECHARGELEVEL, 0x3A);
    sendCommand(SSD1331_CMD_VCOMH, 0x3E);
    sendCommand(SSD1331_CMD_MASTERCURRENT, 0x06);
    sendCommand(SSD1331_CMD_CONTRASTA, 0x91);
    sendCommand(SSD1331_CMD_CONTRASTB, 0x50);
    sendCommand(SSD1331_CMD_CONTRASTC, 0x7D);
    sendCommand(SSD1331_CMD_SETCOLUMN, 0, DISPLAY_WIDTH - 1);
    sendCommand(SSD1331_CMD_SETROW, 0, DISPLAY_HEIGHT - 1);
    sendCommand(SSD1331_CMD_DISPLAYON);
}

void SSD1331::doDrawPixel(uint8_t x, uint8_t y, uint8_t mode) {
    uint8_t bit = _BV(y % 8);
    uint16_t pos = DISPLAY_WIDTH * (y / 8) + x;
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

void SSD1331::doSetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
    _backgroundColor = color(red, green, blue);
}

void SSD1331::doSetForegroundColor(uint8_t red, uint8_t green, uint8_t blue) {
    _foregroundColor = color(red, green, blue);
}

void SSD1331::doUpdate() {
    startTransfer(false);
    for (uint8_t y = 0; y < DISPLAY_HEIGHT; ++y) {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; ++x) {
            uint8_t bit = _BV(y % 8);
            uint16_t pos = DISPLAY_WIDTH * (y / 8) + x;
            if (_buffer[pos] & bit) {
                sendByte(_foregroundColor >> 8);
                sendByte(_foregroundColor);
            }
            else {
                sendByte(_backgroundColor >> 8);
                sendByte(_backgroundColor);
            }
        }
    }

    endTransfer();
}

void SSD1331::endTransfer() const {
    digitalWrite(_csPin, HIGH);
}

void SSD1331::sendByte(uint8_t data) const {
    if (_hwSpi) {
        SPI.transfer(data);
    }
    else {
        uint8_t mask = 128;
        while (mask > 0) {
            *_clockPinReg &= ~_clockPinMask;
            if ((data & mask) == mask) {
                *_dataPinReg |= _dataPinMask;
            }
            else {
                *_dataPinReg &= ~_dataPinMask;
            }

            *_clockPinReg |= _clockPinMask;
            mask >>= 1;
        }
    }
}

void SSD1331::sendCommand(uint8_t command) const {
    startTransfer(true);
    sendByte(command);
    endTransfer();
}

void SSD1331::sendCommand(uint8_t command, uint8_t param1) const {
    startTransfer(true);
    sendByte(command);
    sendByte(param1);
    endTransfer();
}

void SSD1331::sendCommand(uint8_t command, uint8_t param1, uint8_t param2) const {
    startTransfer(true);
    sendByte(command);
    sendByte(param1);
    sendByte(param2);
    endTransfer();
}

void SSD1331::startTransfer(bool command) const {
    if (!_hwSpi) {
        // clock polarity is 1 (SPI mode 3)
        *_clockPinReg |= _clockPinMask;
    }

    if (command) {
        digitalWrite(_dcPin, LOW);
    }
    else {
        digitalWrite(_dcPin, HIGH);
    }

    digitalWrite(_csPin, LOW);
}

