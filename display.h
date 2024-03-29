#include "Fonts/FreeMono_euro9pt8b.h"
#include "Fonts/FreeMonoBold_euro9pt8b.h"
#include "Fonts/FreeMonoBold_euro12pt8b.h"

#define FONT_SMALL FreeMono_euro9pt8b
#define FONT_NORMAL FreeMonoBold_euro9pt8b
#define FONT_BIG FreeMonoBold_euro12pt8b

#define EURO "\x80"

void drawBigText(int x, int y, char* text, int color) {
  display.setFont(&FONT_BIG);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawSmallText(int x, int y, char* text, int color) {
  display.setFont(&FONT_SMALL);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawText(int x, int y, char* text, int color) {
  display.setFont(&FONT_NORMAL);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void displayDays(Data* daily) {
  char date[3];
  for (int i = 0; i < DAYS; i++) {
    if (strlen(daily->days[i]) >= 9) { // str is like yyyy-mm-dd
      drawText(38 + i * 32, 172, &daily->days[i][8], GxEPD_BLACK);
    }
  }
}

int mapToY (int y) {
 int yy = y * 7;
 int maxY = 155;
 int r = yy < maxY ? maxY - yy : maxY;
 // Serial.printf("mapToY: %i ⁻> %i\n", y, r);
 return r;
}

void displayScale() {
  char s[4];
  Serial.println("Scale");
  for (int i = 0; i <= 15; i+=3) {
    int y = mapToY(i);
    sprintf(s, "%2dk", i);
    drawText(2, 5 + y, s, GxEPD_BLACK);
    display.fillRect(40, y, 220, 2, GxEPD_RED);
  }
}

void displayConsumption(Data* daily) {
  for (int i = 0; i < DAYS; i++) {
    int value = daily->values[i] / 1000;
    int x = 48 + i * 32;
    int y = mapToY(value);
    display.fillRect(x, y, 12, 155 - y, GxEPD_BLACK);
  }
}

void displayMaxPower(Data* power) {
  for (int i = 0; i < DAYS; i++) {
    int value = power->values[i] / 1000;
    int x = 40 + i * 32;
    int y = mapToY(value);
    display.fillRect(x, y, 6, 155 - y, GxEPD_RED);
  }
}

void displayPrices(Data* daily) {
  int baseY = 14;
  drawBigText(8, baseY + 8, EURO, GxEPD_BLACK);
  char price[10];
  for (int i = 0; i < DAYS; i++) {
    float p = daily->values[i] * KW_H_PRICE / 1000;
    sprintf(price, "%.1f", p);
    int x = 40 + i * 31,
        y = i % 2 == 0 ? baseY : baseY + 16;
    int color = p >= 1.5 ? GxEPD_BLACK : GxEPD_RED;
    drawText(x, y, price, color);
    Serial.printf("price: %s\n", price);
  }
}

void displayData(Data* daily, Data* power) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayScale();
    displayDays(daily);
    displayConsumption(daily);
    displayPrices(daily);
    displayMaxPower(power);
  } while (display.nextPage());
}

void displayError(char* text) {
  int x = 0;
  int y = display.height() - 20;
  int w = display.width() / 2;
  int h = 20;
  display.setFont(&FONT_SMALL);
  display.setTextColor(GxEPD_BLACK);
  
  display.setPartialWindow(x, y, w, h);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x + 5, y + 12);
    display.print(text);
  } while (display.nextPage());
}
