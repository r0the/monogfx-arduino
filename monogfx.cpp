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

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
    #define pgm_read_ptr(addr) pgm_read_ptr_far(addr)
#else
    #define pgm_read_ptr(addr) pgm_read_ptr_near(addr)
#endif


PROGMEM const uint8_t DefaultFontBitmaps[] PROGMEM = {
    B00000000, B00000000, B00000000, B00000000, B00000000, 3, // SPACE
    B01011111, B00000000, B00000000, B00000000, B00000000, 1, // !
    B00000011, B00000000, B00000011, B00000000, B00000000, 3, // "
    B00010100, B01111111, B00010100, B01111111, B00010100, 5, // #
    B00100100, B00101010, B01101011, B00101010, B00010010, 5, // $
    B00100011, B00010011, B00001000, B01100100, B01100010, 5, // %
    B00000000, B00000000, B00000000, B00000000, B00000000, 0, // & TODO
    B00000011, B00000000, B00000000, B00000000, B00000000, 1, // '

    B00011100, B00100010, B01000001, B00000000, B00000000, 3, // (
    B01000001, B00100010, B00011100, B00000000, B00000000, 3, // )
    B00101010, B00011100, B00111110, B00011100, B00101010, 5, // * IMPROVE
    B00001000, B00001000, B00111110, B00001000, B00001000, 5, // +
    B10000000, B01000000, B00000000, B00000000, B00000000, 2, // ,
    B00001000, B00001000, B00001000, B00001000, B00001000, 5, // -
    B01000000, B01000000, B00000000, B00000000, B00000000, 1, // .
    B00100000, B00010000, B00001000, B00000100, B00000010, 5, // /

    B00111110, B01010001, B01001001, B01000101, B00111110, 5, // 0
    B01000100, B01000010, B01111111, B01000000, B01000000, 5, // 1
    B01100010, B01010001, B01001001, B01001001, B01000110, 5, // 2
    B00100010, B01000001, B01001001, B01001001, B00110110, 5, // 3
    B00011000, B00010100, B00010010, B01111111, B00010000, 5, // 4
    B00100111, B01000101, B01000101, B01000101, B00111001, 5, // 5
    B00111100, B01001010, B01001001, B01001001, B00110000, 5, // 6
    B00000001, B01110001, B00001001, B00000101, B00000011, 5, // 7

    B00110110, B01001001, B01001001, B01001001, B00110110, 5, // 8
    B00000110, B01001001, B01001001, B00101001, B00011110, 5, // 9
    B01001000, B00000000, B00000000, B00000000, B00000000, 1, // :
    B10000000, B01001000, B00000000, B00000000, B00000000, 2, // ;
    B00001000, B00010100, B00100010, B01000001, B00000000, 4, // <
    B00010100, B00010100, B00010100, B00010100, B00010100, 5, // =
    B01000001, B00100010, B00010100, B00001000, B00000000, 4, // >
    B00000010, B00000001, B01010001, B00001001, B00000110, 5, // ?

    B00111110, B01000001, B01011101, B01010101, B01011110, 5, // @
    B01111110, B00001001, B00001001, B00001001, B01111110, 5, // A
    B01111111, B01001001, B01001001, B01001001, B00110110, 5, // B
    B00111110, B01000001, B01000001, B01000001, B00100010, 5, // C
    B01111111, B01000001, B01000001, B00100010, B00011100, 5, // D
    B01111111, B01001001, B01001001, B01001001, B01000001, 5, // E
    B01111111, B00001001, B00001001, B00001001, B00000001, 5, // F
    B00111110, B01000001, B01001001, B01001001, B00111010, 5, // G

    B01111111, B00001000, B00001000, B00001000, B01111111, 5, // H
    B01000001, B01111111, B01000001, B00000000, B00000000, 3, // I
    B00110000, B01000000, B01000001, B00111111, B00000001, 5, // J
    B01111111, B00001000, B00010100, B00100010, B01000001, 5, // K
    B01111111, B01000000, B01000000, B01000000, B01000000, 5, // L
    B01111111, B00000010, B00001100, B00000010, B01111111, 5, // M
    B01111111, B00000100, B00001000, B00010000, B01111111, 5, // N
    B00111110, B01000001, B01000001, B01000001, B00111110, 5, // O

    B01111111, B00001001, B00001001, B00001001, B00000110, 5, // P
    B00111110, B01000001, B01100001, B01000001, B10111110, 5, // Q
    B01111111, B00001001, B00011001, B00101001, B01000110, 5, // R
    B00100110, B01001001, B01001001, B01001001, B00110010, 5, // S
    B00000001, B00000001, B01111111, B00000001, B00000001, 5, // T
    B00111111, B01000000, B01000000, B01000000, B00111111, 5, // U
    B00011111, B00100000, B01000000, B00100000, B00011111, 5, // V
    B00111111, B01000000, B00111000, B01000000, B00111111, 5, // W

    B01100011, B00010100, B00001000, B00010100, B01100011, 5, // X
    B00000111, B00001000, B01110000, B00001000, B00000111, 5, // Y
    B01100001, B01010001, B01001001, B01000101, B01000011, 5, // Z
    B01111111, B01000001, B01000001, B00000000, B00000000, 3, // [
    B00000010, B00000100, B00001000, B00010000, B00100000, 5, // backslash
    B01000001, B01000001, B01111111, B00000000, B00000000, 3, // ]
    B00000100, B00000010, B00000001, B00000010, B00000100, 5, // ^
    B01000000, B01000000, B01000000, B01000000, B01000000, 5, // _
    B00000001, B00000010, B00000000, B00000000, B00000000, 2, // back quote
    B00100000, B01010100, B01010100, B01010100, B01111000, 5, // a
    B01111111, B01000100, B01000100, B01000100, B00111000, 5, // b
    B00111000, B01000100, B01000100, B01000100, B01001000, 5, // c
    B00111000, B01000100, B01000100, B01000100, B01111111, 5, // d
    B00111000, B01010100, B01010100, B01010100, B01011000, 5, // e
    B00000100, B01111110, B00000101, B00000001, B00000000, 4, // f
    B10011000, B10100100, B10100100, B10100100, B01111000, 5, // g
    B01111111, B00000100, B00000100, B00000100, B01111000, 5, // h
    B01000100, B01111101, B01000000, B00000000, B00000000, 3, // i
    B01000000, B10000000, B10000100, B01111101, B00000000, 4, // j
    B01111111, B00010000, B00101000, B01000100, B00000000, 4, // k
    B01000001, B01111111, B01000000, B00000000, B00000000, 3, // l
    B01111100, B00000100, B01111100, B00000100, B01111000, 5, // m
    B01111100, B00000100, B00000100, B00000100, B01111000, 5, // n
    B00111000, B01000100, B01000100, B01000100, B00111000, 5, // o
    B11111100, B00100100, B00100100, B00100100, B00011000, 5, // p
    B00011000, B00100100, B00100100, B00100100, B11111100, 5, // q
    B00000100, B01111000, B00000100, B00000100, B00001000, 5, // r
    B01001000, B01010100, B01010100, B01010100, B00100100, 5, // s
    B00000100, B00111111, B01000100, B01000100, B00000000, 4, // t
    B00111100, B01000000, B01000000, B01000000, B01111100, 5, // u
    B00001100, B00110000, B01000000, B00110000, B00001100, 5, // v
    B00111100, B01000000, B00111100, B01000000, B00111100, 5, // w
    B01000100, B00101000, B00010000, B00101000, B01000100, 5, // x
    B00011100, B10100000, B10100000, B10100000, B01111100, 5, // y
    B01000100, B01100100, B01010100, B01001100, B01000100, 5, // z
    B00001000, B00110110, B01000001, B01000001, B00000000, 4, // {
    B01111111, B00000000, B00000000, B00000000, B00000000, 1, // |
    B01000001, B01000001, B00110110, B00001000, B00000000, 4, // }
    B00000010, B00000001, B00000010, B00000001, B00000000, 4, // ~
};

#define CHAR_BYTES 6
#define MIN_CHAR 32
#define MAX_CHAR 126

static byte readFont(char ch, int offset) {
    if (MIN_CHAR <= ch && ch <= MAX_CHAR) {
        return pgm_read_byte_near(DefaultFontBitmaps + (ch - MIN_CHAR) * CHAR_BYTES + offset);
    }
    else {
        return 0;
    }
}

class Glyph {
public:
    Glyph(const GFXglyph* pgmGlyph, const uint8_t* pgmBitmap) :
        _pgmGlyphBitmap(pgmBitmap + pgm_read_word(&pgmGlyph->bitmapOffset)),
        _pgmGlyph(pgmGlyph) {
    }

    inline uint8_t operator[](uint8_t index) const { return pgm_read_byte(_pgmGlyphBitmap + index); }
    inline uint8_t Glyph::width() const { return pgm_read_byte(&_pgmGlyph->width); }
    inline uint8_t Glyph::height() const { return pgm_read_byte(&_pgmGlyph->height); }
    inline uint8_t Glyph::xAdvance() const { return pgm_read_byte(&_pgmGlyph->xAdvance); }
    inline int8_t Glyph::xOffset() const { return pgm_read_byte(&_pgmGlyph->xOffset); }
    inline int8_t Glyph::yOffset() const { return pgm_read_byte(&_pgmGlyph->yOffset); }
private:
    const uint8_t* _pgmGlyphBitmap;
    const GFXglyph* _pgmGlyph;
};

class Font {
public:
    explicit Font(const GFXfont& pgmFont) {
        assign(pgmFont);
    }

    void assign(const GFXfont& pgmFont) {
        _first = pgm_read_byte(&pgmFont.first);
        _last = pgm_read_byte(&pgmFont.last);
        _pgmBitmap = pgm_read_ptr(&pgmFont.bitmap);
        _pgmGlyphs = pgm_read_ptr(&pgmFont.glyph);
        _yAdvance = pgm_read_byte(&pgmFont.yAdvance);
    }

    Glyph Font::glyph(char ch) const {
        return Glyph(_pgmGlyphs + (ch - _first), _pgmBitmap);
    }

    inline char first() const { return _first; }
    inline char last() const { return _last; }
    inline uint8_t yAdvance() const { return _yAdvance; }
private:
    char _first;
    char _last;
    const uint8_t* _pgmBitmap;
    const GFXglyph* _pgmGlyphs;
    uint8_t _yAdvance;
};

MonoGfx::MonoGfx(uint8_t width, uint8_t height) :
    _font(NULL),
    _height(height),
    _width(width) {
}

void MonoGfx::drawBitmap(int16_t x, int16_t y, const uint8_t* pgmBitmap, uint8_t w, uint8_t h, uint8_t mode) {
    const uint8_t byteWidth = (w + 7) / 8;
    uint8_t data;
    for(uint8_t yi = 0; yi < h; ++yi) {
        for(uint8_t xi = 0; xi < w; ++xi) {
            if (xi & 7) {
                data >>= 1;
            }
            else {
                data = pgm_read_byte(pgmBitmap + (yi * byteWidth + xi / 8));
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


void MonoGfx::setFont(const GFXfont& pgmFont) {
    delete _font;
    _font = new Font(pgmFont);
}

void MonoGfx::update() {
    doUpdate();
}

uint8_t MonoGfx::write(uint8_t x, uint8_t y, const char* text, uint8_t mode) {
    int i = 0;
    while (text[i] != '\0') {
        if (_font) {
            x = writeCharGfx(x, y, text[i], mode);
        }
        else {
            x = writeCharDefault(x, y, text[i], mode);
        }

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

uint8_t MonoGfx::writeCharGfx(uint8_t x, uint8_t y, char ch, uint8_t mode) {
    Glyph glyph = _font->glyph(ch);
    uint8_t h = glyph.height();
    uint8_t w = glyph.width();
    int8_t xOffset = glyph.xOffset();
    int8_t yOffset = glyph.yOffset();
    uint8_t data;
    uint8_t bit = 0;
    for (uint8_t yi = 0; yi < h; ++yi) {
        for (uint8_t xi = 0; xi < w; ++xi) {
            if (bit % 8 == 0) {
                data = glyph[bit / 8];
            }

            if (data & 0x80) {
                drawPixel(x + xOffset + xi, y + yOffset + yi, mode);
            }

            data <<= 1;
            ++bit;
        }
    }

    return x + glyph.xAdvance();
}

uint8_t MonoGfx::writeCharDefault(uint8_t x, uint8_t y, char ch, uint8_t mode) {
    uint8_t charWidth = readFont(ch, CHAR_BYTES - 1);
    uint8_t data;
    for (int i = 0; i < charWidth; ++i) {
        data = readFont(ch, i);
        for (uint8_t yi = 0; yi < 8; ++yi) {
            if (data & 1) {
                drawPixel(x, y - 7 + yi, mode);
            }

            data >>= 1;
        }

        ++x;
    }

    return x + 1;
}
