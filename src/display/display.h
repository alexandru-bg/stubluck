#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <pins.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include <LittleFS.h>
#include <font1.h>
#include <font2.h>

extern TFT_eSPI tft;
extern Preferences prefs;

extern int brightness;

void showText(const String &text);
void drawImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h);
void drawImageFSRegion(const char *filename, int16_t img_w, int16_t x, int16_t y, int16_t w, int16_t h);
void fadeInImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t duration_ms = 500);
void slideDownImageFS(const char *filename, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t duration_ms = 1500);
void fadeOut(uint16_t duration_ms = 500);

#endif