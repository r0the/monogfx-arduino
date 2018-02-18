#include <Wire.h>
#include "ssd1306.h"
#include "ssd1331.h"
#include "all_fonts.h"

SSD1306 gfx(0x3C);

//SSD1331 gfx2(1, 2, 3);

void setup() {
    Wire.begin();
    gfx.begin();

}

void fontSample(Font* font, const char* desc) {
    gfx.clear();
    gfx.setFont(font);
    gfx.drawText(0, 10, desc);
    gfx.update();
    delay(2000);
}

void loop() {
/*    
    fontSample(&GENERA_BIG, "Genera Big Regular");
    fontSample(&GENERA_BIG_B, "Genera Big Bold");
    fontSample(&HELVETICA_18, "Helvetica 18 Regular");
    fontSample(&HELVETICA_18_B, "Helvetica 18 Bold");
    fontSample(&HL_18, "HL 18 Regular");
    fontSample(&HL_18_B, "HL 18 Bold");
    fontSample(&SWISS_20, "Swiss 20 Regular");
    fontSample(&SWISS_20_B, "Swiss 20 Bold");
*/
    gfx.drawText(10, 10, "Hello World");
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

