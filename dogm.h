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

#ifndef DOGM_H
#define DOGM_H

#include "monogfx.h"

class DOGM : public MonoGfx {
public:
    DOGM(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin, uint8_t clockPin, uint8_t dataPin,
         uint8_t columns, uint8_t pages);
    void begin();
    void setPowerControl(bool booster, bool regulator, bool follower);
    void switchOn();
    void switchOff();
protected:
    virtual void doDrawPixel(uint8_t x, uint8_t y, uint8_t mode);
    virtual void doUpdate();
private:
    void beginTransferCmd();
    void endTransfer();
    void sendByte(uint8_t data);
    uint8_t _a0Pin;
    uint8_t* _buffer;
    uint16_t _bufferSize;
    uint8_t _clockPin;
    uint8_t _columns;
    uint8_t _csPin;
    uint8_t _dataPin;
    bool _hwSpi;
    uint8_t _pages;
    uint8_t _resetPin;
};

class DOGM128 : public DOGM {
public:
    DOGM128(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin);
    DOGM128(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin, uint8_t clockPin, uint8_t dataPin);
};

class DOGM132 : public DOGM {
public:
    DOGM132(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin);
    DOGM132(uint8_t csPin, uint8_t resetPin, uint8_t a0Pin, uint8_t clockPin, uint8_t dataPin);
};

#endif // DOGM_H

