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

#ifndef MONOGFX_H
#define MONOGFX_H

#include <Arduino.h>
#include "monogfx_font.h"

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2

#define MODE_SET 0
#define MODE_CLEAR 1
#define MODE_INVERT 2

extern VariableWidthFont HELVETICA_8;
extern VariableWidthFont HELVETICA_10;
extern VariableWidthFont HELVETICA_10_B;
extern VariableWidthFont HELVETICA_12;
extern VariableWidthFont HELVETICA_14;
extern VariableWidthFont HELVETICA_18;
extern VariableWidthFont HELVETICA_18_B;
extern VariableWidthFont SWISS_20;
extern VariableWidthFont SWISS_20_B;

class MonoGfx {
public:
    explicit MonoGfx(uint16_t width, uint16_t height);
    ~MonoGfx();
    void begin();
    void clear();
    void debugBegin();
    void debugShow();
    void drawBitmapPgm(int16_t x, int16_t y, const uint8_t* pgmBitmap, uint16_t width, uint16_t height);
    int16_t drawCharacter(int16_t x, int16_t y, char ch);
    void drawCircle(int16_t centerX, int16_t centerY, uint16_t radius);
    void drawHorizontalLine(int16_t x1, int16_t y, int16_t x2);
    void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void drawPixel(int16_t x, int16_t y);
    void drawRectangle(int16_t left, int16_t top, uint16_t width, uint16_t height);
    int16_t drawText(int16_t x, int16_t y, const char* text);
    void drawVerticalLine(int16_t x, int16_t y1, int16_t y2);
    void fill();
    void fillRectangle(int16_t left, int16_t top, uint16_t width, uint16_t height);
    inline uint8_t fontScale() const { return _fontScale; }
    inline uint16_t height() const { return _height; }
    inline uint8_t mode() const { return _mode; }
    inline bool ready() const { return _ready; }
    void setFont(Font* font);
    void setFontScale(uint8_t fontScale);
    void setMode(uint8_t mode);
    void setTextAlign(uint8_t textAlign);
    inline uint8_t textAlign() const { return _textAlign; }
    uint16_t textWidth(const char* text) const;
    void update();
    inline uint16_t width() const { return _width; }
protected:
    // hardware-specific implementation
    virtual bool doInitialize() = 0;
    virtual void doUpdate(uint8_t* buffer, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) = 0;
private:
    uint8_t* _buffer;
    Font* _font;
    uint8_t _fontScale;
    uint16_t _height;
    uint8_t _mode;
    bool _ready;
    uint8_t _textAlign;
    uint16_t _width;
};

#endif

