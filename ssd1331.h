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
 
#ifndef SSD1331_H
#define SSD1331_H

#include "monogfx.h"

class SSD1331 : public MonoGfx {
public:
    SSD1331(uint8_t csPin, uint8_t dcPin);
    SSD1331(uint8_t csPin, uint8_t dcPin, uint8_t clockPin, uint8_t dataPin);
    void begin();
protected:
    virtual void doDrawPixel(uint8_t x, uint8_t y, uint8_t mode);
    virtual void doSetBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
    virtual void doSetForegroundColor(uint8_t red, uint8_t green, uint8_t blue);
    virtual void doUpdate();
private:
    void sendByte(uint8_t data) const;
    void sendCommand(uint8_t command);
    void sendCommand(uint8_t command, uint8_t param1);
    void sendCommand(uint8_t command, uint8_t param1, uint8_t param2);
    void startTransfer();
    uint16_t _backgroundColor;
    uint8_t* _buffer;
    uint8_t _csPin;
    uint8_t _clockPin;
    uint8_t _dataPin;
    uint8_t _dcPin;
    uint16_t _foregroundColor;
    bool _hwSpi;
};

#endif // SSD1331_H

