#include <SPI.h>
#include <sb6432.h>

SB6432 lcd(3, 4);

void setup() {
    lcd.begin();
    lcd.setBacklightColor(255, 128, 0);
}

void loop() {
    lcd.fill(MODE_CLEAR);
    lcd.write(0, 10, "Hello World");
    lcd.update();
    delay(2000);

    lcd.fill(MODE_CLEAR);
    for (uint8_t r = 0; r < lcd.height(); r += 4) {
        lcd.drawCircle(lcd.width() / 2, lcd.height() / 2, r);
        lcd.update();
        delay(200);
    }

    delay(2000);
    
    lcd.fill(MODE_CLEAR);
    for (uint8_t x = 0; x < lcd.width(); x += 4) {
        lcd.drawLine(lcd.width() / 2, lcd.height() / 2, x, 0);
        lcd.drawLine(lcd.width() / 2, lcd.height() / 2, x, lcd.height() - 1);
        lcd.update();
        delay(200);
    }

    for (uint8_t y = 0; y < lcd.height(); y += 4) {
        lcd.drawLine(lcd.width() / 2, lcd.height() / 2, 0, y);
        lcd.drawLine(lcd.width() / 2, lcd.height() / 2, lcd.width() - 1, y);
        lcd.update();
        delay(200);
    }

    delay(1000);

    lcd.fill(MODE_CLEAR);
    lcd.drawRect(0, 0, lcd.width(), lcd.height());
    lcd.update();
    delay(2000);

    lcd.fill(MODE_SET);
    lcd.drawRect(1, 1, lcd.width()-2, lcd.height()-2, MODE_CLEAR);
    lcd.update();
    delay(2000);
}

