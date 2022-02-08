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

#include "ssd1331.h"
#include <SPI.h>

#define DISPLAY_WIDTH 96
#define DISPLAY_HEIGHT 64
#define DISPLAY_PAGES 1

#define CMD_SET_COLUMN_ADDRESS        0x15
#define CMD_SET_ROW_ADDRESS           0x75
#define CMD_SET_CONTRAST_A            0x81
#define CMD_SET_CONTRAST_B            0x82
#define CMD_SET_CONTRAST_C            0x83
#define CMD_MASTER_CURRENT_CONTROL    0x87
#define CMD_PRECHARGE_SPEED_A         0x8A
#define CMD_PRECHARGE_SPEED_B         0x8B
#define CMD_PRECHARGE_SPEED_C         0x8C
#define CMD_SET_DISPLAY_DIM           0xAC
#define CMD_SET_DISPLAY_SLEEP         0xAE
#define CMD_SET_DISPLAY_ON            0xAF
#define CMD_REMAP_COLOR_DEPTH_SETTING 0xA0
#define CMD_SET_DISPLAY_START_LINE    0xA1
#define CMD_SET_DISPLAY_OFFSET        0xA2
#define CMD_SET_DISPLAY_MODE_NORMAL   0xA4
#define CMD_SET_DISPLAY_MODE_ALL_ON   0xA5
#define CMD_SET_DISPLAY_MODE_ALL_OFF  0xA6
#define CMD_SET_DISPLAY_MODE_INVERSE  0xA7
#define CMD_SET_MULTIPLEX_RATIO       0xA8
#define CMD_SET_MASTER_CONFIGURATION  0xAD
#define CMD_POWER_SAVE_MODE           0xB0
#define CMD_PHASE12_PERIOD_ADJUSTMENT 0xB1
#define CMD_DISPLAY_CLOCK_DIVIDER     0xB3
#define CMD_SET_PRECHARGE_LEVEL       0xBB
#define CMD_SET_VCOMH                 0xBE

#define CMD_DRAW_LINE                 0x21
#define CMD_DRAW_RECT                 0x22
#define CMD_FILL                      0x26


static uint16_t color16bit(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t c = red >> 3;
    c <<= 6;
    c |= green >> 2;
    c <<= 5;
    c |= blue >> 3;
    return c;
}

static uint16_t color8bit(uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t c = red >> 5;
    c <<= 3;
    c |= green >> 5;
    c <<= 2;
    c |= blue >> 6;
    return c;
}

SSD1331::SSD1331(uint8_t csPin, uint8_t resetPin, uint8_t dcPin) :
    MonoGfx(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_PAGES),
    _csPin(csPin),
    _dcPin(dcPin),
    _resetPin(resetPin) {
    setForegroundColor(255, 255, 255);
    setBackgroundColor(0, 0, 0);
}

bool SSD1331::doInitialize() {
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3);
    SPI.setClockDivider(SPI_CLOCK_DIV4);

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

    startTransfer(true);
    // turn display off
    SPI.transfer(CMD_SET_DISPLAY_SLEEP);
    // 256 color, RGB
    SPI.transfer(CMD_REMAP_COLOR_DEPTH_SETTING);
    SPI.transfer(B00110010);
    // set display start line to 0
    SPI.transfer(CMD_SET_DISPLAY_START_LINE);
    SPI.transfer(0);
    // set display offset to 0
    SPI.transfer(CMD_SET_DISPLAY_OFFSET);
    SPI.transfer(0);
    // set normal display mode
    SPI.transfer(CMD_SET_DISPLAY_MODE_NORMAL);
    // set multiplex ratio
    SPI.transfer(CMD_SET_MULTIPLEX_RATIO);
    SPI.transfer(0x3F);
    // dry master configuration
    SPI.transfer(CMD_SET_MASTER_CONFIGURATION);
    SPI.transfer(0x8E);
    // disable power save mode
    SPI.transfer(CMD_POWER_SAVE_MODE);
    SPI.transfer(0x0B);
    // phase 1 and 2 period adjustment
    SPI.transfer(CMD_PHASE12_PERIOD_ADJUSTMENT);
    SPI.transfer(0x31);
    // display clock divider / oscillator frequency
    SPI.transfer(CMD_DISPLAY_CLOCK_DIVIDER);
    SPI.transfer(0xF0);

    SPI.transfer(CMD_PRECHARGE_SPEED_A);
    SPI.transfer(0x64);

    SPI.transfer(CMD_PRECHARGE_SPEED_B);
    SPI.transfer(0x78);

    SPI.transfer(CMD_PRECHARGE_SPEED_C);
    SPI.transfer(0x64);

    SPI.transfer(CMD_SET_PRECHARGE_LEVEL);
    SPI.transfer(0x3A);

    SPI.transfer(CMD_SET_VCOMH);
    SPI.transfer(0x3E);

    SPI.transfer(CMD_MASTER_CURRENT_CONTROL);
    SPI.transfer(0x06);

    SPI.transfer(CMD_SET_CONTRAST_A);
    SPI.transfer(0x91);

    SPI.transfer(CMD_SET_CONTRAST_B);
    SPI.transfer(0x50);

    SPI.transfer(CMD_SET_CONTRAST_C);
    SPI.transfer(0x7D);

    SPI.transfer(CMD_SET_COLUMN_ADDRESS);
    SPI.transfer(0);
    SPI.transfer(DISPLAY_WIDTH - 1);

    SPI.transfer(CMD_SET_ROW_ADDRESS);
    SPI.transfer(0);
    SPI.transfer(DISPLAY_HEIGHT - 1);

    SPI.transfer(CMD_SET_DISPLAY_ON);
    endTransfer();
    return true;
}

void SSD1331::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
    _backgroundColor = color8bit(red, green, blue);
}

void SSD1331::setForegroundColor(uint8_t red, uint8_t green, uint8_t blue) {
    _foregroundColor = color8bit(red, green, blue);
}

void SSD1331::setSleepMode(bool enable) {
    digitalWrite(_dcPin, LOW);
    digitalWrite(_csPin, LOW);
    SPI.transfer(enable ? CMD_SET_DISPLAY_SLEEP : CMD_SET_DISPLAY_ON);
    digitalWrite(_csPin, HIGH);
}

void SSD1331::doUpdate(uint8_t* buffer, uint8_t page) {
    startTransfer(true);
    SPI.transfer(CMD_SET_COLUMN_ADDRESS);
    SPI.transfer(0);
    SPI.transfer(width() - 1);
    SPI.transfer(CMD_SET_ROW_ADDRESS);
    SPI.transfer(0);
    SPI.transfer(height() - 1);
    startTransfer(false);
    for (uint8_t y = 0; y < height(); ++y) {
        uint8_t bit = 1 << (y % 8);
        uint16_t pos = DISPLAY_WIDTH * (y / 8);
        for (uint8_t x = 0; x < width(); ++x) {
            if (buffer[pos + x] & bit) {
                SPI.transfer(_foregroundColor);
            }
            else {
                SPI.transfer(_backgroundColor);
            }
        }
    }

    endTransfer();
}

void SSD1331::endTransfer() const {
    digitalWrite(_csPin, HIGH);
}

void SSD1331::startTransfer(bool command) const {
    if (command) {
        digitalWrite(_dcPin, LOW);
    }
    else {
        digitalWrite(_dcPin, HIGH);
    }

    digitalWrite(_csPin, LOW);
}
