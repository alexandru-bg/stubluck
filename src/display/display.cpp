#include "display.h"

void showText(const String &text)
{
  tft.setFreeFont(&Roboto_Condensed_Bold_22);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(text, 120, 120);
}

void fadeInImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t duration_ms)
{
  analogWrite(BACKLIGHT_PIN, 255);  // backlight OFF (active-low)
  drawImageFS(filename, x, y, w, h);

  uint8_t steps = 20;
  uint16_t step_delay = duration_ms / steps;
  for (uint8_t i = 1; i <= steps; i++) {
    int val = 255 - ((255 - brightness) * i) / steps;
    analogWrite(BACKLIGHT_PIN, val);
    delay(step_delay);
  }
  analogWrite(BACKLIGHT_PIN, brightness);
}

void fadeOut(uint16_t duration_ms)
{
  uint8_t steps = 20;
  uint16_t step_delay = duration_ms / steps;
  for (uint8_t i = 1; i <= steps; i++) {
    int val = brightness + ((255 - brightness) * i) / steps;
    analogWrite(BACKLIGHT_PIN, val);
    delay(step_delay);
  }
  analogWrite(BACKLIGHT_PIN, 255);  
  tft.fillScreen(TFT_BLACK);
  analogWrite(BACKLIGHT_PIN, brightness);  
}

// Animate the image sliding in from above the screen, Gameboy boot-logo style.
// Uses ST7789 hardware vertical scroll so we only draw one row of pixels per step
// instead of redrawing the whole 240x240 image each frame.
void slideDownImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t duration_ms)
{
  File f = LittleFS.open(filename, "r");
  if (!f) {
    Serial.printf("slideDownImageFS: cannot open %s\n", filename);
    return;
  }

  uint8_t palette[512];
  f.read(palette, 512);

  uint8_t idx[240];
  uint8_t buf[480];

  tft.fillScreen(TFT_WHITE);

  // VSCRDEF: TFA=0, VSA=h, BFA=320-h.  Scroll cycles within the visible rows;
  // the off-screen 80 RAM rows stay parked as BFA.
  uint16_t bfa = 320 - h;
  tft.writecommand(0x33);
  tft.writedata(0); tft.writedata(0);
  tft.writedata((uint8_t)(h >> 8)); tft.writedata((uint8_t)(h & 0xFF));
  tft.writedata((uint8_t)(bfa >> 8)); tft.writedata((uint8_t)(bfa & 0xFF));

  uint32_t start = millis();
  for (int16_t i = 1; i <= h; i++) {
    // Draw bottom-of-image first, at RAM row (h-i), and point VSP there so
    // the freshly-drawn row appears at screen row 0.  Content previously
    // drawn at higher RAM rows shifts down one row on screen.
    uint16_t ram_row = h - i;

    tft.writecommand(0x37);
    tft.writedata((uint8_t)(ram_row >> 8)); tft.writedata((uint8_t)(ram_row & 0xFF));

    f.seek(512 + (size_t)ram_row * w);
    f.read(idx, w);
    for (int16_t j = 0; j < w; j++) {
      uint16_t p = idx[j] * 2;
      buf[j * 2]     = palette[p];
      buf[j * 2 + 1] = palette[p + 1];
    }

    tft.startWrite();
    tft.setAddrWindow(x, y + ram_row, w, 1);
    tft.pushPixels(buf, w);
    tft.endWrite();

    uint32_t target = start + ((uint32_t)duration_ms * i) / h;
    int32_t wait = (int32_t)(target - millis());
    if (wait > 0) delay(wait);
  }

  // Restore default scroll state so subsequent views draw naturally.
  tft.writecommand(0x33);
  tft.writedata(0); tft.writedata(0);
  tft.writedata(0x01); tft.writedata(0x40);
  tft.writedata(0); tft.writedata(0);
  tft.writecommand(0x37);
  tft.writedata(0); tft.writedata(0);

  f.close();
}

void drawImageFSRegion(const char *filename, int16_t img_w, int16_t x, int16_t y, int16_t w, int16_t h)
{
  File f = LittleFS.open(filename, "r");
  if (!f) {
    Serial.printf("drawImageFSRegion: cannot open %s\n", filename);
    return;
  }

  uint8_t palette[512];
  f.read(palette, 512);

  uint8_t idx[240];
  uint8_t buf[480];

  tft.startWrite();
  for (int16_t row = 0; row < h; row++) {
    f.seek(512 + (size_t)(y + row) * img_w + x);
    f.read(idx, w);
    for (int16_t i = 0; i < w; i++) {
      uint16_t p = idx[i] * 2;
      buf[i * 2]     = palette[p];
      buf[i * 2 + 1] = palette[p + 1];
    }
    tft.setAddrWindow(x, y + row, w, 1);
    tft.pushPixels(buf, w);
  }
  tft.endWrite();

  f.close();
}

void drawImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h)
{
  File f = LittleFS.open(filename, "r");
  if (!f) {
    Serial.printf("drawImageFS: cannot open %s\n", filename);
    return;
  }

  // Read 256-color palette (big-endian RGB565)
  uint8_t palette[512];
  f.read(palette, 512);

  uint8_t idx[256];
  uint8_t buf[512];
  tft.startWrite();
  tft.setAddrWindow(x, y, w, h);

  size_t remaining = (size_t)w * h;
  while (remaining > 0) {
    size_t count = min(remaining, (size_t)256);
    f.read(idx, count);
    for (size_t i = 0; i < count; i++) {
      uint16_t p = idx[i] * 2;
      buf[i * 2]     = palette[p];
      buf[i * 2 + 1] = palette[p + 1];
    }
    tft.pushPixels(buf, count);
    remaining -= count;
  }

  tft.endWrite();
  f.close();
}
