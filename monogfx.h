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

#ifndef MONOGFX_H
#define MONOGFX_H

#include <Arduino.h>
#include "gfxfont.h"

#define MODE_SET 0
#define MODE_CLEAR 1
#define MODE_INVERT 2

class Bitmap {
public:
    Bitmap(uint8_t width, uint8_t height, const uint8_t* const pgmPtr);
    inline uint8_t operator[](uint8_t index) const { return pgm_read_byte(_pgmPtr + index); }
    inline uint8_t height() const { return _height; }
    inline uint8_t width() const { return _width; }
private:
    const uint8_t _height;
    const uint8_t* const _pgmPtr;
    const uint8_t _width;
};

class Glyph {
public:
    Glyph(const GFXglyph* pgmGlyph, const uint8_t* pgmBitmap);
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
    explicit Font(const GFXfont& pgmFont);
    void assign(const GFXfont& pgmFont);
    Glyph glyph(char ch) const;
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

class MonoGfx {
public:
    MonoGfx(uint8_t width, uint8_t height);
    void drawBitmap(int16_t x, int16_t y, const Bitmap& bitmap, uint8_t mode = MODE_SET);
    void drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t mode = MODE_SET);
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode = MODE_SET);
    void drawPixel(uint8_t x, uint8_t y, uint8_t mode = MODE_SET);
    void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t mode = MODE_SET);
    void fill(uint8_t mode = MODE_SET);
    void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t mode = MODE_SET);
    inline uint8_t height() const { return _height; }
    void setFont(const GFXfont& pgmFont);
    void update();
    inline uint8_t width() const { return _width; }
    uint8_t write(uint8_t x, uint8_t y, const char* text, uint8_t mode = MODE_SET);
protected:
    virtual void doDrawPixel(uint8_t x, uint8_t y, uint8_t mode = MODE_SET) = 0;
    virtual void doDrawHLine(uint8_t x, uint8_t y, uint8_t length, uint8_t mode);
    virtual void doDrawVLine(uint8_t x, uint8_t y, uint8_t length, uint8_t mode);
    virtual void doUpdate() = 0;

private:
    uint8_t writeCharGfx(uint8_t x, uint8_t y, char ch, uint8_t mode);
    uint8_t writeCharBuiltin(uint8_t x, uint8_t y, char ch, uint8_t mode);

    Font _font;
    uint8_t _height;
    uint8_t _width;
};

#endif // MONOGFX_H

