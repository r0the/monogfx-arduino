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
#include "FreeSans7pt.h"

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
    #define pgm_read_ptr(addr) pgm_read_ptr_far(addr)
#else
    #define pgm_read_ptr(addr) pgm_read_ptr_near(addr)
#endif

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
    _font(new Font(FreeSans7pt7b)),
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
        x = writeCharGfx(x, y - 7, text[i], mode);
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

uint8_t MonoGfx::writeCharBuiltin(uint8_t x, uint8_t y, char ch, uint8_t mode) {
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

