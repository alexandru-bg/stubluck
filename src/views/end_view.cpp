#include "end_view.h"
#include "start_view.h"
#include "display.h"

static const unsigned long BLINK_INTERVAL = 500;

static int finalScore = 0;
static bool newHighFlag = false;
static bool blinkVisible = true;
static unsigned long lastBlinkMs = 0;
static int16_t promptRegionX, promptRegionY, promptRegionW, promptRegionH;

void setEndScore(int score, bool newHigh) {
  finalScore = score;
  newHighFlag = newHigh;
}

static void drawContinueText() {
  if (blinkVisible) {
    tft.setFreeFont(&Roboto_Condensed_Bold_22);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("CONTINUE", 120, 200);
  } else {
    drawImageFSRegion("/end.bin", 240, promptRegionX, promptRegionY, promptRegionW, promptRegionH);
  }
}

static void drawScore() {
  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  tft.setTextDatum(MC_DATUM);

  if (newHighFlag) {
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("NEW HIGH SCORE!", 120, 130);
  }

  char buf[24];
  snprintf(buf, sizeof(buf), "SCORE: $%d", finalScore);
  tft.setTextColor(newHighFlag ? TFT_YELLOW : TFT_WHITE);
  tft.drawString(buf, 120, 160);
}

static void enter() {
  fadeInImageFS("/end.bin", 0, 0, 240, 240, 1000);

  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  promptRegionW = tft.textWidth("CONTINUE") + 4;
  promptRegionH = tft.fontHeight() + 4;
  promptRegionX = 120 - promptRegionW / 2;
  promptRegionY = 200 - promptRegionH / 2;

  blinkVisible = true;
  lastBlinkMs = millis();

  drawScore();
  drawContinueText();
}

static void update() {
  unsigned long now = millis();
  if (now - lastBlinkMs >= BLINK_INTERVAL) {
    lastBlinkMs = now;
    blinkVisible = !blinkVisible;
    drawContinueText();
  }
}

static void onClick() {
  setView(&startView);
}

static void onDoubleClick() {}
static void onLongPress() {}

const View endView = { enter, update, onClick, onDoubleClick, onLongPress };
