void drawText(int x, int y, char* text, int color) {
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawTinyText(int x, int y, char* text, int color) {
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

void drawSmallText(int x, int y, char* text, int color) {
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.println(text);
}

char* extractDate (char* str, char* date) { // str is like yyyy-mm-dd
  date[0] = str[8];
  date[1] = str[9];
  date[2] = '\0';
}

void displayDays(Data* daily) {
  char date[3];
  for (int i = 0; i < DAYS; i++) {
    extractDate(daily->days[i], date);
    drawSmallText(38 + i * 32, 172, date, GxEPD_BLACK);
  }
}

int mapToY (int y) {
 int r = 155 - y * 10;
 Serial.printf("mapToY: %i ⁻> %i\n", y, r);
 return r;
}

void displayScale() {
  char s[4];
  Serial.println("Scale");
  for (int i = 0; i <= 14; i+=3) {
    int y = mapToY(i);
    sprintf(s, "%2dk", i);
    drawSmallText(2, 5 + y, s, GxEPD_BLACK);
    display.fillRect(40, y, 220, 2, GxEPD_RED);
  }
}

void displayConsumption(Data* daily) {
  for (int i = 0; i < DAYS; i++) {
    int value = daily->values[i] / 1000;
    int x = 48 + i * 32;
    int y = mapToY(value);
    display.fillRect(x, y, 10, 155 - y, GxEPD_BLACK);
    display.fillRect(x, y, 2, 2, GxEPD_RED);
  }
}

void displayPrices(Data* daily) {
  int baseY = 14;
  drawSmallText(4, baseY + 2, "eur", GxEPD_BLACK);
  char price[10];
  for (int i = 0; i < DAYS; i++) {
    float p = daily->values[i] * KW_H_PRICE / 1000;
    sprintf(price, "%.1f", p);
    int x = 38 + i * 32,
        y = i % 2 == 0 ? baseY : baseY + 16;
    int color = p >= 1.5 ? GxEPD_RED : GxEPD_BLACK;
    drawTinyText(x, y, price, color);
    Serial.printf("price: %s\n", price);
  }
}

void displayData(Data* daily) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayScale();
    displayDays(daily);
    displayConsumption(daily);
    displayPrices(daily);
  } while (display.nextPage());
}

void displayCenteredText(char* text) {
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
  // center bounding box by transposition of origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  // here we use paged drawing, even if the processor has enough RAM for full buffer
  // so this can be used with any supported processor board.
  // the cost in code overhead and execution time penalty is marginal
  // tell the graphics class to use paged drawing mode
  display.firstPage();
  do
  {
    // this part of code is executed multiple times, as many as needed,
    // in case of full buffer it is executed once
    // IMPORTANT: each iteration needs to draw the same, to avoid strange effects
    // use a copy of values that might change, don't read e.g. from analog or pins in the loop!
    display.fillScreen(GxEPD_WHITE); // set the background to white (fill the buffer with value for white)
    display.setCursor(x, y); // set the postition to start printing text
    display.print(text); // print some text
    // end of part executed multiple times
  }
  // tell the graphics class to transfer the buffer content (page) to the controller buffer
  // the graphics class will command the controller to refresh to the screen when the last page has been transferred
  // returns true if more pages need be drawn and transferred
  // returns false if the last page has been transferred and the screen refreshed for panels without fast partial update
  // returns false for panels with fast partial update when the controller buffer has been written once more, to make the differential buffers equal
  // (for full buffered with fast partial update the (full) buffer is just transferred again, and false returned)
  while (display.nextPage());
}
