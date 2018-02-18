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

#ifndef MONOGFX_FONT_H
#define MONOGFX_FONT_H

#include <Arduino.h>

class Font {
public:
    virtual uint8_t byteHeight() const = 0;
    virtual uint8_t fontHeight() const = 0;
    virtual uint8_t glyphData(char ch, uint8_t i) const = 0;
    virtual uint8_t glyphWidth(char ch) const = 0;
};

class VariableWidthFont : public Font {
public:
    explicit VariableWidthFont(char firstChar, char lastChar, uint8_t height,
        const uint8_t* bitmapPgmPtr, const uint8_t* dataPgmPtr);
    virtual uint8_t byteHeight() const;
    virtual uint8_t fontHeight() const;
    virtual uint8_t glyphData(char ch, uint8_t i) const;
    virtual uint8_t glyphWidth(char ch) const;
private:
    uint8_t readPtrByte(const uint8_t* ptr, uint16_t offset) const;
    const uint8_t* _bitmapPgmPtr;
    const uint8_t* _dataPgmPtr;
    char _firstChar;
    uint8_t _height;
    char _lastChar;
};

#endif

