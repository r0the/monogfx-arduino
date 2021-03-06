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

#include "monogfx.h"
#include "all_fonts.h"

MonoGfx::MonoGfx(uint16_t width, uint16_t height, uint8_t pages) :
    _buffer(new uint8_t[width * height / 8]),
    _font(&HELVETICA_8),
    _fontScale(1),
    _height(height),
    _mode(MODE_SET),
    _nextPage(0),
    _pages(pages),
    _ready(false),
    _width(width)
{
    clear();
}

MonoGfx::~MonoGfx() {
    delete[] _buffer;
}

void MonoGfx::begin() {
    _ready = doInitialize();
}

void MonoGfx::clear() {
    for (uint16_t i = 0; i < _width * _height / 8; ++i) {
        _buffer[i] = 0;
    }
}

void MonoGfx::debugBegin() {
    Serial.begin(9600);
    while (!Serial) {
        delay(1);
    }
}

void MonoGfx::debugShow() {
    Serial.println();
    for (uint16_t y = 0; y < _height; ++y) {
        for (uint8_t x = 0; x < _width; ++x) {
            uint16_t pos = _width * (y / 8) + x;
            uint8_t bit = 1 << (y % 8);
            Serial.print(_buffer[pos] & bit ? "0" : " ");
        }

        Serial.println();
    }
}

void MonoGfx::drawBitmapPgm(int16_t x, int16_t y, const uint8_t* pgmBitmap, uint16_t width, uint16_t height) {
    const uint8_t byteWidth = (width + 7) / 8;
    uint8_t data;
    for(uint16_t yi = 0; yi < height; ++yi) {
        for(uint16_t xi = 0; xi < width; ++xi) {
            if (xi & 7) {
                data >>= 1;
            }
            else {
                data = pgm_read_byte(pgmBitmap + (yi * byteWidth + xi / 8));
            }

            if (data & 0x01) {
                drawPixel(x + xi, y + yi);
            }
        }
    }
}


int16_t MonoGfx::drawCharacter(int16_t x, int16_t y, char ch) {
    uint8_t glyphWidth = _font->glyphWidth(ch);
    uint8_t fontHeight = _font->fontHeight();
    uint8_t bufferPos = 0;
    for (uint8_t cx = 0; cx < glyphWidth; ++cx) {
        uint8_t data = _font->glyphData(ch, bufferPos);
        ++bufferPos;
        uint8_t bit = 1;
        for (uint8_t cy = 0; cy < fontHeight; ++cy) {
            if (data & bit) {
                drawPixel(x + cx, y + cy);
            }

            if (bit == 128) {
                data = _font->glyphData(ch, bufferPos);
                ++bufferPos;
                bit = 1;
            }
            else {
                bit <<= 1;
            }
        }
    }

    return x + glyphWidth + _font->byteHeight();
}

void MonoGfx::drawCircle(int16_t centerX, int16_t centerY, uint16_t radius) {
    int16_t x = -radius;
    int16_t y = 0;
    int16_t err = 2 - 2 * radius;
    int16_t oldErr;
    do {
        drawPixel(centerX - x, centerY + y);
        drawPixel(centerX - y, centerY - x);
        drawPixel(centerX + x, centerY - y);
        drawPixel(centerX + y, centerY + x);
        oldErr = err;
        if (oldErr <= y) {
            ++y;
            err += 2 * y + 1;
        }

        if (oldErr > x || err > y) {
            ++x;
            err += 2 * x + 1;
        }
    } while (x < 0);
}

void MonoGfx::drawHorizontalLine(int16_t x1, int16_t y, int16_t x2) {
    if (x1 > x2) {
        return;
    }

    while (x1 <= x2) {
        drawPixel(x1, y);
        ++x1;
    }
}

void MonoGfx::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
   int16_t dx = abs(x2 - x1);
   int16_t sx = x1 < x2 ? 1 : -1;
   int16_t dy = -abs(y2 - y1);
   int16_t sy = y1 < y2 ? 1 : -1;
   int16_t err = dx + dy;
   int16_t e2;
   while (true){
        drawPixel(x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void MonoGfx::drawPixel(int16_t x, int16_t y) {
    if (x < 0 || _width <= x || y < 0 || _height <= y) {
        return;
    }

    uint16_t pos = _width * (y / 8) + x;
    uint8_t bit = 1 << (y % 8);
    switch (_mode) {
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

void MonoGfx::drawRectangle(int16_t left, int16_t top, uint16_t width, uint16_t height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    const int16_t right = left + width - 1;
    const int16_t bottom = top + height - 1;
    drawHorizontalLine(left, top, right);
    drawHorizontalLine(left, bottom, right);
    drawVerticalLine(left, top, bottom);
    drawVerticalLine(right, top, bottom);
}

int16_t MonoGfx::drawText(int16_t x, int16_t y, const char* text) {
    uint8_t i = 0;
    switch (_textAlign) {
        case ALIGN_RIGHT:
            x = x - textWidth(text);
            break;
        case ALIGN_CENTER:
            x = x - textWidth(text) / 2;
            break;
    }

    while (text[i] != '\0') {
        x = drawCharacter(x, y, text[i]);
        ++i;
    }

    return x;
}

void MonoGfx::drawVerticalLine(int16_t x, int16_t y1, int16_t y2) {
    if (y1 > y2) {
        return;
    }

    while (y1 <= y2) {
        drawPixel(x, y1);
        ++y1;
    }
}

void MonoGfx::fill() {
    for (int16_t i = 0; i < _width * _height / 8; ++i) {
        _buffer[i] = 0xFF;
    }
}

void MonoGfx::fillRectangle(int16_t left, int16_t top, uint16_t width, uint16_t height) {
    if (width <= 0 || height <= 0) {
        return;
    }

    const int16_t right = left + width - 1;
    const int16_t bottom = top + height - 1;
    while (top <= bottom) {
        drawHorizontalLine(left, top, right);
        ++top;
    }
}

void MonoGfx::setFont(Font* font) {
    _font = font;
}

void MonoGfx::setFontScale(uint8_t fontScale) {
    if (fontScale == 0) {
        return;
    }

    _fontScale = fontScale;
}

void MonoGfx::setMode(uint8_t mode) {
    if (mode > MODE_INVERT) {
        return;
    }

    _mode = mode;
}

void MonoGfx::setTextAlign(uint8_t textAlign) {
    if (textAlign > ALIGN_CENTER) {
        return;
    }

    _textAlign = textAlign;
}

uint16_t MonoGfx::textWidth(const char* text) const {
    uint8_t result = 0;
    uint8_t i = 0;
    while (text[i] != '\0') {
        result += (_font->glyphWidth(text[i]) + 1) * _fontScale;
        ++i;
    }

    return result - 1;
}

void MonoGfx::update() {
    for (uint8_t page = 0; page < _pages; ++page) {
        doUpdate(_buffer, page);
    }
}

void MonoGfx::updatePage() {
    doUpdate(_buffer, _nextPage);
    _nextPage = (_nextPage + 1) % _pages;
}

