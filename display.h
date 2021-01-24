void drawText(int x, int y, char* text, int color) {
  display.setFont(&FreeMonoBold12pt7b);
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

void drawBigText(int x, int y, char* text) {
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(x, y);
  display.println(text);
}

char* extractDate (char* str) { // str is like yyyy-mm-dd
  char date[3];
  date[0] = str[8];
  date[1] = str[9];
  date[2] = '\0';
  return date;
}

void displayDays(Data* daily) {
  char* date;
  int i = 0;
  //for (int i = 0; i < DAYS; i++) {
    date = extractDate(daily->days[i]);
    Serial.println(date);
    drawSmallText(10 + (i * 30), 10, date, GxEPD_BLACK);
  //}
}

void displayData(Data* daily) {
  display.fillScreen(GxEPD_WHITE);
  display.firstPage();
  do {
    displayDays(daily);
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
