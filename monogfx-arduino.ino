#include "ssd1331.h"
#include "monogfx.h"
#include "fonts/artos_sans_8x8.h"
#include "fonts/victoria_8x8.h"



//DOGM132 lcd(10, 11, 12, 8, 9);
SSD1331 lcd(6, 13, 14);
MonoGfx gfx(&lcd);
//SSD1331 lcd(13, 11, 12);
//DOGM132 lcd(10, 11, 12);

void setup() {
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
    delay(500);
    lcd.begin();
//    gfx.setFontScale(2);
    lcd.setBackgroundColor(64+32, 0, 0);
   //    lcd.setFont(FreeMono12pt7b);
}

void loop() {
    gfx.fill(MODE_CLEAR);
    gfx.setFont(&FONT_VICTORIA_8x8);
    gfx.drawText(0, 20, "ABCDefghi");
    gfx.setFont(&FONT_ARTOS_SANS_8x8);
    gfx.drawText(0, 40, "ABCDEFefghi");
    gfx.update();
    delay(2000);
    return;

    gfx.fill(MODE_CLEAR);
    for (uint8_t r = 0; r < gfx.height(); r += 4) {
        gfx.drawCircle(gfx.width() / 2, gfx.height() / 2, r);
        gfx.update();
        delay(200);
    }

    delay(2000);

    gfx.fill(MODE_CLEAR);
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

    gfx.fill(MODE_CLEAR);
    gfx.drawRectangle(0, 0, gfx.width(), gfx.height());
    gfx.update();
    delay(2000);

    gfx.fill(MODE_SET);
    gfx.drawRectangle(1, 1, gfx.width()-2, gfx.height()-2, MODE_CLEAR);
    gfx.update();
    delay(2000);

    gfx.fill(MODE_CLEAR);
//    lcd.drawBitmap(16, 0, heart);
    gfx.update();
    delay(2000);
}

