//#include "ssd1331.h"
#include "ssd1306.h"
#include "monogfx.h"
#include <Wire.h>
//#include "fonts/artos_sans_8.h"
//#include "fonts/victoria.h"
//#include "fonts/victoria_bold.h"



//DOGM132 lcd(10, 11, 12, 8, 9);

// 
//SSD1331 lcd(6, 13, 14);

// Feather 
//SSD1331 lcd(5, 1, 0);
//DOGM132 lcd(10, 11, 12);

SSD1306 gfx(0x3C);

void setup() {
    Wire.begin();
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }

    Serial.println("Initializing display...");

    gfx.begin();
    Serial.println("Initialized display...");
    if (!gfx.ready()) {
        Serial.println("Display not found");
    }
    else {
        Serial.println("Display ready");
    }

    gfx.turnOff();
    delay(1000);
        Serial.println("turn on...");
    gfx.turnOn();
    gfx.test(true);
        Serial.println("test ...");
    delay(1000);
    gfx.test(false);
    gfx.setInvert(true);

    Serial.println("test donw");
}

void loop() {
    gfx.clear();
//    gfx.setFont(&FONT_VICTORIA);
    gfx.drawText(0, 10, "Victoria");
//    gfx.setFont(&FONT_VICTORIA_BOLD);
    gfx.drawText(0, 20, "Victoria Bold");
//    gfx.setFont(&FONT_ARTOS_SANS_8);
    gfx.drawText(0, 30, "Artos Sans");
    gfx.update();
    delay(2000);

    gfx.clear();
    for (uint8_t r = 0; r < gfx.height(); r += 4) {
        gfx.drawCircle(gfx.width() / 2, gfx.height() / 2, r);
        gfx.update();
        delay(200);
    }

    delay(2000);

    gfx.clear();
    for (uint8_t x = 0; x < gfx.width(); x += 4) {
        gfx.drawLine(gfx.width() / 2, gfx.height() / 2, x, 0);
        gfx.drawLine(gfx.width() / 2, gfx.height() / 2, x, gfx.height() - 1);
        gfx.update();
        delay(200);
    }

    for (uint8_t y = 0; y < gfx.height(); y += 4) {
        gfx.drawLine(gfx.width() / 2, gfx.height() / 2, 0, y);
        gfx.drawLine(gfx.width() / 2, gfx.height() / 2, gfx.width() - 1, y);
        gfx.update();
        delay(200);
    }

    delay(1000);

    gfx.clear();
    gfx.drawRectangle(0, 0, gfx.width(), gfx.height());
    gfx.update();
    delay(2000);

    gfx.fill();
    gfx.setMode(MODE_CLEAR);
    gfx.drawRectangle(1, 1, gfx.width()-2, gfx.height()-2);
    gfx.setMode(MODE_SET);
    gfx.update();
    delay(2000);

    gfx.clear();
//    lcd.drawBitmap(16, 0, heart);
    gfx.update();
    delay(2000);
}

