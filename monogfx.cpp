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

#include "monogfx.h"
#include "font.h"

Bitmap::Bitmap(uint8_t width, uint8_t height, const uint8_t* ptr) :
    _height(height),
    _ptr(ptr),
    _width(width) {
}

MonoGfx::MonoGfx(uint8_t width, uint8_t height) :
    _height(height),
    _width(width) {
}

void MonoGfx::drawBitmap(int16_t x, int16_t y, const Bitmap& bitmap, uint8_t mode) {
    const uint8_t byteWidth = (bitmap.width() + 7) / 8;
    uint8_t data;
    for(uint8_t yi = 0; yi < bitmap.height(); ++yi) {
        for(uint8_t xi = 0; xi < bitmap.width(); ++xi) {
            if (xi & 7) {
                data >>= 1;
            }
            else {
                data = bitmap[yi * byteWidth + xi / 8];
            }

            if (data & 0x01) {
                drawPixel(x + xi, y + yi, mode);
            }
        }
    }
}

void MonoGfx::drawCircle(uint8_t xm, uint8_t ym, uint8_t r, uint8_t mode = MODE_SET) {
    int16_t x = -r;
    int16_t y = 0;
    int16_t err = 2 - 2 * r;
    int16_t oldErr;
    do {
        drawPixel(xm - x, ym + y, mode);
        drawPixel(xm - y, ym - x, mode);
        drawPixel(xm + x, ym - y, mode);
        drawPixel(xm + y, ym + x, mode);
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

void MonoGfx::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode) {
   int16_t dx = abs(x2 - x1);
   int16_t sx = x1 < x2 ? 1 : -1;
   int16_t dy = -abs(y2 - y1);
   int16_t sy = y1 < y2 ? 1 : -1;
   int16_t err = dx + dy;
   int16_t e2;
   while (true){
        drawPixel(x1, y1, mode);
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

void MonoGfx::fill(uint8_t mode) {
    for (uint8_t x = 0; x < _width; ++x) {
        doDrawVLine(x, 0, _height, mode);
    }
}

void MonoGfx::drawPixel(uint8_t x, uint8_t y, uint8_t mode = MODE_SET) {
    if (0 <= x && x < _width && 0 <= y && y < _height) {
        doDrawPixel(x, y, mode);
    }
}

void MonoGfx::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t mode) {
    doDrawHLine(x, y, w, mode);
    doDrawHLine(x, y + h - 1, w, mode);
    doDrawVLine(x, y, h, mode);
    doDrawVLine(x + w - 1, y, h, mode);
}

void MonoGfx::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t mode) {
    for (uint8_t i = x; i < x + w; ++i) {
        doDrawVLine(i, y, h, mode);
    }
}

void MonoGfx::update() {
    doUpdate();
}

uint8_t MonoGfx::write(uint8_t x, uint8_t y, const char* text, uint8_t mode) {
    int i = 0;
    while (text[i] != '\0') {
        x = writeChar(x, y, text[i], mode);
        ++i;
    }

    return x;
}

void MonoGfx::doDrawHLine(uint8_t x, uint8_t y, uint8_t length, uint8_t mode) {
    while (length > 0) {
        doDrawPixel(x, y, mode);
        ++x;
        --length;
    }
}

void MonoGfx::doDrawVLine(uint8_t x, uint8_t y, uint8_t length, uint8_t mode) {
    while (length > 0) {
        drawPixel(x, y, mode);
        ++y;
        --length;
    }
}

uint8_t MonoGfx::writeChar(uint8_t x, uint8_t y, char ch, uint8_t mode) {
    uint8_t charWidth = readFont(ch, CHAR_BYTES - 1);
    uint8_t data;
    for (int i = 0; i < charWidth; ++i) {
        data = readFont(ch, i);
        for (uint8_t yi = 0; yi < 8; ++yi) {
            if (data & 1) {
                drawPixel(x, y + yi, mode);
            }

            data >>= 1;
        }

        ++x;
    }

    return x + 1;
}

