#include "start_view.h"
#include "game_view.h"
#include "display.h"
#include "audio.h"

extern const char *songs[];
static const int SONG_THEME = 8;

static bool blinkVisible = true;
static unsigned long lastBlinkMs = 0;
static const unsigned long BLINK_INTERVAL = 500;

static int16_t textRegionX, textRegionY, textRegionW, textRegionH;

static void drawStartText() {
  if (blinkVisible) {
    tft.setFreeFont(&Roboto_Condensed_Bold_22);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("START GAME", 120, 180);
  } else {
    drawImageFSRegion("/start.bin", 240, textRegionX, textRegionY, textRegionW, textRegionH);
  }
}

static void drawHighScore() {
  int highScore = prefs.getInt("highscore", 0);
  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_YELLOW);

  char buf[20];
  snprintf(buf, sizeof(buf), "HI: $%d", highScore);
  tft.drawString(buf, 120, 215);
}

static void enter() {
  fadeInImageFS("/start.bin", 0, 0, 240, 240, 1000);
  playSound(songs[SONG_THEME]);
  blinkVisible = true;
  lastBlinkMs = millis();

  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  textRegionW = tft.textWidth("START GAME") + 4;
  textRegionH = tft.fontHeight() + 4;
  textRegionX = 120 - textRegionW / 2;
  textRegionY = 180 - textRegionH / 2;

  drawStartText();
  drawHighScore();
}

static void update() {
  unsigned long now = millis();
  if (now - lastBlinkMs >= BLINK_INTERVAL) {
    lastBlinkMs = now;
    blinkVisible = !blinkVisible;
    drawStartText();
  }
}

static void onClick() {
  setView(&gameView);
}

static void onDoubleClick() {}
static void onLongPress() {}

const View startView = { enter, update, onClick, onDoubleClick, onLongPress };
