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

#include "monogfx_font.h"

VariableWidthFont::VariableWidthFont(char firstChar, char lastChar, uint8_t height,
    const uint8_t* bitmapPgmPtr, const uint8_t* dataPgmPtr) :
    _bitmapPgmPtr(bitmapPgmPtr),
    _dataPgmPtr(dataPgmPtr),
    _firstChar(firstChar),
    _height(height),
    _lastChar(lastChar) {
}

uint8_t VariableWidthFont::byteHeight() const {
    return 1 + (_height - 1) / 8;
}

uint8_t VariableWidthFont::glyphData(char ch, uint8_t i) const {
    if (ch < _firstChar || _lastChar < ch) {
        return 0;
    }

    uint16_t charOffset = 3 * (ch - _firstChar);
    uint16_t bitmapOffset = readPtrByte(_dataPgmPtr, charOffset);
    bitmapOffset <<= 8;
    bitmapOffset += readPtrByte(_dataPgmPtr, charOffset + 1);
    uint8_t result = readPtrByte(_bitmapPgmPtr, bitmapOffset + i);
    return result;
}

uint8_t VariableWidthFont::fontHeight() const {
    return _height;
}

uint8_t VariableWidthFont::glyphWidth(char ch) const {
    if (ch < _firstChar || _lastChar < ch) {
        return 0;
    }

    return readPtrByte(_dataPgmPtr, 3 * (ch - _firstChar) + 2);
}

uint8_t VariableWidthFont::readPtrByte(const uint8_t* ptr, uint16_t offset) const {
    return pgm_read_byte_far(ptr + offset);
}

