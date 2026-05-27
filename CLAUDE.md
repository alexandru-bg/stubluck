# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

StubLuck — a push-your-luck ticket scalping card game running on an ESP8266 handheld device with a single button, ST7789 240x240 TFT display, and piezo buzzer. Written in C++ using the Arduino framework.

## Build Commands

This is a PlatformIO project. All commands use the `pio` CLI:

- **Build:** `pio run`
- **Upload to device:** `pio run --target upload`
- **Serial monitor:** `pio device monitor` (115200 baud)
- **Build filesystem image:** `pio run --target buildfs`
- **Upload filesystem:** `pio run --target uploadfs`

There are no tests, linting, or formatting tools configured.

## Architecture

Standard Arduino entry point in `src/main.cpp` with `setup()` and `loop()`. Code is organized into modules under `src/`:

- **config/** — Hardware constants: `pins.h` (GPIO assignments), `tft.h` (ST7789 driver config via TFT_eSPI `User_Setup.h` overrides)
- **display/** — TFT rendering API (`display.h`/`display.cpp`), custom bitmap fonts (`font1.h`, `font2.h`)
- **audio/** — RTTTL melody playback (`audio.h`/`audio.cpp`), RTTTL song definitions
- **views/** — Referenced in build flags but not yet created; intended for Boot/Start/Game/End screen implementations

Build flags in `platformio.ini` add `-Isrc/config -Isrc/display -Isrc/audio -Isrc/views` so headers can be included without path prefixes.

## Key Hardware Constraint

GPIO 4 is shared between the button and the piezo buzzer via multiplexing. The `playSound()` function in `audio.cpp` temporarily switches the pin to `OUTPUT` mode for audio, then restores `INPUT_PULLUP` for button reading after waiting for button release. Any new code touching GPIO 4 must respect this mux pattern.

## Global Objects

- `TFT_eSPI tft` — display driver (defined in `main.cpp`, `extern` in `display.h`)
- `Preferences prefs` — NVS storage for high score (defined in `main.cpp`, `extern` in `display.h`)
- `const char *songs[]` — RTTTL strings (defined in `audio.cpp`, `extern` in `main.cpp`)

## Game Rules Reference

- Player starts with $500, guesses higher/lower on ticket numbers 1-99
- Correct guess doubles the pot; wrong guess loses it; ties push
- Ante escalates with balance ($50/$100/$250/$500 at thresholds)
- Single button: short press = HIGHER, long press (>400ms) = LOWER, double-click = cash out
- Score = peak balance; persisted to NVS via Preferences library
