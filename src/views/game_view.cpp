#include "game_view.h"
#include "end_view.h"
#include "display.h"
#include "audio.h"
#include "game_data.h"

extern const char *songs[];

enum {
  SONG_BOOT = 0,
  SONG_BEEP = 1,
  SONG_START = 2,
  SONG_CORRECT = 3,
  SONG_WRONG = 4,
  SONG_CASH = 5,
  SONG_OVER = 6,
  SONG_HI = 7,
};

static const int START_BAL = 500;
static const int MIN_CARD = 1;
static const int MAX_CARD = 99;

static int balance;
static int peak;
static int pot;
static int streak;
static int currentCard;
static int ante;
static const char *currentArtist;
static const char *currentVenue;

static int calcAnte(int bal) {
  if (bal < 1000) return 50;
  if (bal < 2500) return 100;
  if (bal < 5000) return 250;
  return 500;
}

static uint16_t cardBorderColor(int v) {
  if (v <= 25) return 0xC000;   // darker red
  if (v <= 50) return 0xCD60;   // darker yellow / gold
  if (v <= 75) return 0x0580;   // darker green
  return 0x0011;                // darker blue / navy
}

static int rngCard() {
  return (int)(ESP.random() % MAX_CARD) + 1;
}

static void newFlavor() {
  currentArtist = artists[ESP.random() % numArtists];
  currentVenue = venues[ESP.random() % numVenues];
}

static void drawBackground() {
  drawImageFS("/game.bin", 0, 0, 240, 240);
}

static void drawStats() {
  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  tft.setTextColor(TFT_WHITE);

  char buf[24];
  tft.setTextDatum(TL_DATUM);
  snprintf(buf, sizeof(buf), "BAL:$%d", balance);
  tft.drawString(buf, 6, 2);

  snprintf(buf, sizeof(buf), "POT:$%d", pot);
  tft.drawString(buf, 6, 26);

  tft.setTextDatum(TR_DATUM);
  snprintf(buf, sizeof(buf), "STRK:%d", streak);
  tft.drawString(buf, 234, 2);
  snprintf(buf, sizeof(buf), "HI:$%d", peak);
  tft.drawString(buf, 234, 26);
}

static void drawCard() {
  const int cx = 0, cy = 50, cw = 240, ch = 180;
  const uint16_t border = cardBorderColor(currentCard);

  for (int i = 0; i < 3; i++) {
    tft.drawRect(cx + i, cy + i, cw - 2 * i, ch - 2 * i, border);
  }

  for (int y = cy + 14; y < cy + ch - 14; y += 12) {
    tft.fillCircle(cx + 8, y, 2, border);
  }

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(border);
  tft.setFreeFont(&Roboto_Condensed_Bold_22);

  const int artistMaxW = 220;
  tft.setTextSize(2);
  if (tft.textWidth(currentArtist) > artistMaxW) tft.setTextSize(1);
  tft.drawString(currentArtist, cx + cw / 2 + 4, cy + 24);
  tft.setTextSize(1);

  tft.drawString(currentVenue, cx + cw / 2 + 4, cy + 60);

  tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(&LCD24pt7b);
  tft.drawNumber(currentCard, cx + cw / 2 + 4, cy + ch / 2 + 14);

  const int dotsY = cy + ch - 22;
  const int maxDots = 5;
  const int dotSpacing = 30;
  const int dotsStart = cx + cw / 2 - (dotSpacing * (maxDots - 1)) / 2;
  const int visible = streak > maxDots ? maxDots : streak;
  for (int i = 0; i < maxDots; i++) {
    int dx = dotsStart + i * dotSpacing;
    if (i < visible) tft.fillCircle(dx, dotsY, 5, border);
    else tft.drawCircle(dx, dotsY, 5, border);
  }
}

static void drawFooter() {
  tft.setTextFont(1);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("TAP:HI  HOLD:LO  2x:CASH", 120, 236);
}

static void redraw() {
  drawBackground();
  drawStats();
  drawCard();
  drawFooter();
}

static void gameOver() {
  int prevHi = prefs.getInt("highscore", 0);
  bool newHi = peak > prevHi;
  if (newHi) {
    prefs.putInt("highscore", peak);
  }
  playSound(newHi ? songs[SONG_HI] : songs[SONG_OVER]);
  setEndScore(peak, newHi);
  setView(&endView);
}

static void newRound() {
  if (balance <= 0) {
    gameOver();
    return;
  }
  ante = calcAnte(balance);
  if (ante > balance) ante = balance;
  balance -= ante;
  pot = ante;
  streak = 0;
  currentCard = rngCard();
  newFlavor();
  redraw();
  playSound(songs[SONG_START]);
}

static void resolveGuess(bool guessHigher) {
  int next = rngCard();

  if (next == currentCard) {
    int prev = currentCard;
    do { currentCard = rngCard(); } while (currentCard == prev);
    newFlavor();
    redraw();
    return;
  }

  bool correct = (guessHigher && next > currentCard) ||
                 (!guessHigher && next < currentCard);

  if (correct) {
    pot *= 2;
    streak++;
    currentCard = next;
    newFlavor();
    redraw();
    playSound(songs[SONG_CORRECT]);
  } else {
    pot = 0;
    streak = 0;
    currentCard = next;
    newFlavor();
    redraw();
    playSound(songs[SONG_WRONG]);
    if (balance <= 0) gameOver();
    else newRound();
  }
}

static void cashOut() {
  if (streak == 0) return;
  balance += pot;
  if (balance > peak) peak = balance;
  pot = 0;
  streak = 0;
  playSound(songs[SONG_CASH]);
  newRound();
}

static void enter() {
  tft.fillScreen(TFT_BLACK);
  balance = START_BAL;
  peak = START_BAL;
  pot = 0;
  streak = 0;
  currentCard = 0;
  newRound();
}

static void update() {}

static void onClick() {
  if (currentCard == MAX_CARD) return;
  resolveGuess(true);
}

static void onLongPress() {
  if (currentCard == MIN_CARD) return;
  resolveGuess(false);
}

static void onDoubleClick() {
  cashOut();
}

const View gameView = { enter, update, onClick, onDoubleClick, onLongPress };
