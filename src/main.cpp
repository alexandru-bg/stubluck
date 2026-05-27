#include "main.h"

TFT_eSPI tft = TFT_eSPI();
OneButton btn(BUTTON_PIN, true, true);

int brightness = 50;

Preferences prefs;
extern const char *songs[];

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
  }

  prefs.begin("stubluck", false);

  pinMode(BACKLIGHT_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  analogWrite(BACKLIGHT_PIN, 255);
  analogWriteRange(255);
  analogWriteFreq(1000);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  analogWrite(BACKLIGHT_PIN, brightness);

  btn.attachClick(handleClick);
  btn.attachDoubleClick(handleDoubleClick);
  btn.attachLongPressStop(handleLongPress);

  setView(&bootView);
}

void loop()
{
  btn.tick();
  updateView();
}
