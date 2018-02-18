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

#define FIXED_WIDTH_FONT_OFFSET 4

PROGMEM const uint8_t DEFAULT_FONT_PGM[] PROGMEM = {
    3, B00000000, B00000000, B00000000, // SPACE
    1, B01011111, // !
    3, B00000011, B00000000, B00000011, // "
    5, B00010100, B01111111, B00010100, B01111111, B00010100, // #
    5, B00100100, B00101010, B01101011, B00101010, B00010010, // $
    5, B00100011, B00010011, B00001000, B01100100, B01100010, // %
    1, B00000000, // & TODO
    1, B00000011, // '

    3, B00011100, B00100010, B01000001, // (
    3, B01000001, B00100010, B00011100, // )
    5, B00101010, B00011100, B00111110, B00011100, B00101010, // * IMPROVE
    5, B00001000, B00001000, B00111110, B00001000, B00001000, // +
    2, B10000000, B01000000, // ,
    5, B00001000, B00001000, B00001000, B00001000, B00001000, // -
    1, B01000000, // .
    5, B00100000, B00010000, B00001000, B00000100, B00000010, // /

    5, B00111110, B01010001, B01001001, B01000101, B00111110, // 0
    5, B01000100, B01000010, B01111111, B01000000, B01000000, // 1
    5, B01100010, B01010001, B01001001, B01001001, B01000110, // 2
    5, B00100010, B01000001, B01001001, B01001001, B00110110, // 3
    5, B00011000, B00010100, B00010010, B01111111, B00010000, // 4
    5, B00100111, B01000101, B01000101, B01000101, B00111001, // 5
    5, B00111100, B01001010, B01001001, B01001001, B00110000, // 6
    5, B00000001, B01110001, B00001001, B00000101, B00000011, // 7

    5, B00110110, B01001001, B01001001, B01001001, B00110110, // 8
    5, B00000110, B01001001, B01001001, B00101001, B00011110, // 9
    1, B01001000, // :
    2, B10000000, B01001000, // ;
    4, B00001000, B00010100, B00100010, B01000001, // <
    5, B00010100, B00010100, B00010100, B00010100, B00010100, // =
    4, B01000001, B00100010, B00010100, B00001000, // >
    5, B00000010, B00000001, B01010001, B00001001, B00000110, // ?

    5, B00111110, B01000001, B01011101, B01010101, B01011110, // @
    5, B01111110, B00001001, B00001001, B00001001, B01111110, // A
    5, B01111111, B01001001, B01001001, B01001001, B00110110, // B
    5, B00111110, B01000001, B01000001, B01000001, B00100010, // C
    5, B01111111, B01000001, B01000001, B00100010, B00011100, // D
    5, B01111111, B01001001, B01001001, B01001001, B01000001, // E
    5, B01111111, B00001001, B00001001, B00001001, B00000001, // F
    5, B00111110, B01000001, B01001001, B01001001, B00111010, // G

    5, B01111111, B00001000, B00001000, B00001000, B01111111, // H
    3, B01000001, B01111111, B01000001, // I
    5, B00110000, B01000000, B01000001, B00111111, B00000001, // J
    5, B01111111, B00001000, B00010100, B00100010, B01000001, // K
    5, B01111111, B01000000, B01000000, B01000000, B01000000, // L
    5, B01111111, B00000010, B00001100, B00000010, B01111111, // M
    5, B01111111, B00000100, B00001000, B00010000, B01111111, // N
    5, B00111110, B01000001, B01000001, B01000001, B00111110, // O

    5, B01111111, B00001001, B00001001, B00001001, B00000110, // P
    5, B00111110, B01000001, B01100001, B01000001, B10111110, // Q
    5, B01111111, B00001001, B00011001, B00101001, B01000110, // R
    5, B00100110, B01001001, B01001001, B01001001, B00110010, // S
    5, B00000001, B00000001, B01111111, B00000001, B00000001, // T
    5, B00111111, B01000000, B01000000, B01000000, B00111111, // U
    5, B00011111, B00100000, B01000000, B00100000, B00011111, // V
    5, B00111111, B01000000, B00111000, B01000000, B00111111, // W

    5, B01100011, B00010100, B00001000, B00010100, B01100011, // X
    5, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
    5, B01100001, B01010001, B01001001, B01000101, B01000011, // Z
    3, B01111111, B01000001, B01000001, // [
    5, B00000010, B00000100, B00001000, B00010000, B00100000, // backslash
    3, B01000001, B01000001, B01111111, // ]
    5, B00000100, B00000010, B00000001, B00000010, B00000100, // ^
    5, B01000000, B01000000, B01000000, B01000000, B01000000, // _

    2, B00000001, B00000010, // back quote
    5, B00100000, B01010100, B01010100, B01010100, B01111000, // a
    5, B01111111, B01000100, B01000100, B01000100, B00111000, // b
    5, B00111000, B01000100, B01000100, B01000100, B01001000, // c
    5, B00111000, B01000100, B01000100, B01000100, B01111111, // d
    5, B00111000, B01010100, B01010100, B01010100, B01011000, // e
    4, B00000100, B01111110, B00000101, B00000001, // f
    5, B10011000, B10100100, B10100100, B10100100, B01111000, // g

    5, B01111111, B00000100, B00000100, B00000100, B01111000, // h
    3, B01000100, B01111101, B01000000, // i
    4, B01000000, B10000000, B10000100, B01111101, // j
    4, B01111111, B00010000, B00101000, B01000100, // k
    3, B01000001, B01111111, B01000000, // l
    5, B01111100, B00000100, B01111100, B00000100, B01111000, // m
    5, B01111100, B00000100, B00000100, B00000100, B01111000, // n
    5, B00111000, B01000100, B01000100, B01000100, B00111000, // o

    5, B11111100, B00100100, B00100100, B00100100, B00011000, // p
    5, B00011000, B00100100, B00100100, B00100100, B11111100, // q
    5, B00000100, B01111000, B00000100, B00000100, B00001000, // r
    5, B01001000, B01010100, B01010100, B01010100, B00100100, // s
    4, B00000100, B00111111, B01000100, B01000100, // t
    5, B00111100, B01000000, B01000000, B01000000, B01111100, // u
    5, B00001100, B00110000, B01000000, B00110000, B00001100, // v
    5, B00111100, B01000000, B00111100, B01000000, B00111100, // w

    5, B01000100, B00101000, B00010000, B00101000, B01000100, // x
    5, B00011100, B10100000, B10100000, B10100000, B01111100, // y
    5, B01000100, B01100100, B01010100, B01001100, B01000100, // z
    4, B00001000, B00110110, B01000001, B01000001, // {
    1, B01111111, // |
    4, B01000001, B01000001, B00110110, B00001000, // }
    4, B00000010, B00000001, B00000010, B00000001, // ~
};

BuiltinFont  DEFAULT_FONT = BuiltinFont (' ', '~', DEFAULT_FONT_PGM);

Font::~Font() {
}

VariableWidthFont::VariableWidthFont(char firstChar, char lastChar, uint8_t height,
    const uint8_t* bitmapPgmPtr, const uint8_t* dataPgmPtr) :
    _bitmapPgmPtr(bitmapPgmPtr),
    _dataPgmPtr(dataPgmPtr),
    _firstChar(firstChar),
    _height(height),
    _lastChar(lastChar) {
}

VariableWidthFont::~VariableWidthFont() {
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


BuiltinFont ::BuiltinFont (char firstChar, char lastChar, const uint8_t* pgmPtr) :
    _firstChar(firstChar),
    _lastChar(lastChar),
    _pgmPtr(pgmPtr)
{
    uint8_t charCount = _lastChar - _firstChar + 1;
    _charOffset = new uint16_t[charCount];
    uint16_t offset = 0;
    for (uint8_t i = 0; i < charCount; ++i) {
        _charOffset[i] = offset;
        offset += pgm_read_byte_far(_pgmPtr + offset) + 1;
    }
}

BuiltinFont ::~BuiltinFont () {
}

uint8_t BuiltinFont::byteHeight() const {
    return 1;
}

uint8_t BuiltinFont ::glyphData(char ch, uint8_t i) const {
    if (ch < _firstChar || _lastChar < ch) {
        return 0;
    }
    
    return pgm_read_byte_far(_pgmPtr + _charOffset[ch - _firstChar] + 1 + i);
}

uint8_t BuiltinFont::fontHeight() const {
    return 8;
}

uint8_t BuiltinFont ::glyphWidth(char ch) const {
    if (ch < _firstChar || _lastChar < ch) {
        return 0;
    }

    return pgm_read_byte_far(_pgmPtr + _charOffset[ch - _firstChar]);
}

