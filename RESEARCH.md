# RESEARCH.md

Technical research and reference notes for StubLuck development.

## ESP8266 Constraints

- **RAM:** ~80KB usable — large bitmaps must live in PROGMEM or LittleFS, not heap
- **Flash:** 4MB on NodeMCU v2 — plenty for firmware + LittleFS assets
- **Single core** — blocking calls (like `delay()` or synchronous audio) freeze everything
- **No hardware PWM** — `analogWrite` is software PWM; fine for backlight, not for precise audio timing
- **GPIO count is limited** — only a handful of usable GPIOs after SPI for the display

## TFT_eSPI Performance Notes

- `tft.fillScreen()` is slow (~30ms for 240x240) — avoid full redraws in the game loop
- Use `tft.fillRect()` to redraw only changed regions
- `tft.pushImage()` for raw 16-bit RGB565 bitmap blitting — fastest way to render card graphics
- Sprite buffer (`TFT_eSprite`) can eliminate flicker for animated elements but costs RAM (~115KB for full 240x240 16-bit, too large for ESP8266)
- Partial sprites (e.g. 140x180 for the card area) are feasible (~50KB)

## OneButton Library Usage

```cpp
OneButton btn(BUTTON_PIN, true, true); // pin, activeLow, pullup

btn.attachClick(onShortPress);           // HIGHER
btn.attachLongPressStart(onLongPress);   // LOWER (fires at 400ms threshold)
btn.attachDoubleClick(onDoubleClick);    // CASH OUT

// Must call in loop():
btn.tick();
```

- `setClickTicks(300)` — max gap for double-click detection
- `setPressTicks(400)` — threshold for long press
- Button reading conflicts with buzzer mux — must ensure audio is done before calling `btn.tick()`

## RTTTL Format Reference

Format: `name:d=duration,o=octave,b=bpm:notes`

Example: `correct:d=16,o=6,b=200:c,e,g`

AnyRtttl provides non-blocking playback via `anyrtttl::nonblocking::play()` + `anyrtttl::nonblocking::playNonBlocking()` called each loop iteration. This is preferred over blocking `anyrtttl::blocking::play()`.

## LittleFS for Assets

```cpp
#include <LittleFS.h>
LittleFS.begin();
File f = LittleFS.open("/logo.raw", "r");
// read 16-bit RGB565 data into buffer, then tft.pushImage()
```

- Build filesystem image: `pio run --target buildfs`
- Upload: `pio run --target uploadfs`
- Place files in a `data/` directory at project root

## Random Number Generation

ESP8266 has a hardware RNG accessible via `ESP.random()` or `RANDOM_REG32`. Better entropy than `random()` seeded with `analogRead()`. Use for ticket number generation:

```cpp
int nextTicket = (ESP.random() % 99) + 1; // 1-99
```
