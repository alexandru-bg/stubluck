# DECISIONS.md

Architectural and design decisions for StubLuck.

## DEC-001: GPIO 4 Shared Between Button and Buzzer

**Date:** Project inception
**Status:** Accepted

The button and piezo buzzer share GPIO 4 via a multiplexing technique. `playSound()` toggles the pin between `OUTPUT` (audio) and `INPUT_PULLUP` (button reading), with a `waitForBuzzerRelease()` guard before resuming input.

**Rationale:** ESP8266 has very limited GPIOs. After SPI pins for the display and GPIO 5 for backlight PWM, GPIO 4 is the most accessible remaining pin. Sharing it avoids needing additional hardware.

**Consequence:** Audio playback blocks button input. All future code must respect this mux pattern — never read the button during sound playback.

---

## DEC-002: TFT_eSPI Configuration via User_Setup Override

**Date:** Project inception
**Status:** Accepted

Instead of modifying the library's `User_Setup.h`, display configuration lives in `src/config/tft.h` and is pulled in via build flags. This defines the ST7789 driver, pin assignments, color order, and loaded fonts.

**Rationale:** Keeps config in source control and avoids editing files inside `.pio/libdeps/` which get overwritten on library updates.

---

## DEC-003: RTTTL for All Game Audio

**Date:** Project inception
**Status:** Accepted

All sound effects use RTTTL (Ring Tone Text Transfer Language) melodies played through the piezo buzzer via the AnyRtttl library.

**Rationale:** RTTTL is simple, compact, and well-suited to single-channel piezo output. No DAC or audio codec needed. Songs are stored as `const char*` strings in PROGMEM.

---

## DEC-004: Preferences Library for High Score Persistence

**Date:** Project inception
**Status:** Accepted

High scores are stored using the Preferences library (NVS key-value store) with namespace `"game"`.

**Rationale:** Simple API, survives power cycles, no filesystem setup needed. LittleFS is reserved for larger assets (bitmaps).

---

## DEC-005: Single Button with Three Gestures

**Date:** Project inception
**Status:** Accepted

All gameplay input comes from one button using the OneButton library:
- Short press → HIGHER
- Long press (>400ms) → LOWER
- Double-click (<300ms gap) → CASH OUT

**Rationale:** Physical constraint of the handheld form factor. The OneButton library handles debouncing, timing, and gesture discrimination reliably.
