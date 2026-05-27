# TASKS.md

Task tracker for StubLuck development.

## Legend

- [ ] Not started
- [~] In progress
- [x] Done

## Infrastructure

- [x] PlatformIO project setup with ESP8266 target
- [x] TFT_eSPI display driver configured for ST7789 240x240
- [x] GPIO pin definitions (backlight, button/buzzer mux)
- [x] Audio module with RTTTL playback and GPIO mux handling
- [x] Preferences library integration for NVS storage
- [x] Custom bitmap fonts (Roboto Condensed Bold 22pt, LCD 24pt)
- [X] Create `src/views/` directory and view manager

## Display

- [X] Implement `showText()` in `display.cpp`
- [X] Boot screen — 240x240 logo bitmap
- [x] Start screen — bitmap with blinking "START GAME" + high score display
- [ ] Game screen — balance, pot, streak HUD + ticket card rendering
- [ ] End screen — bitmap with final score + blinking "CONTINUE"
- [ ] Ticket card renderer (zigzag edge, number, event name, color border)


## Input

- [X] Integrate OneButton for gesture detection
- [X] Short press → HIGHER
- [X] Long press (>400ms) → LOWER
- [X] Double-click → CASH OUT
- [ ] Edge case: disable LOWER when card=1
- [ ] Edge case: disable HIGHER when card=99

## Game Logic

- [ ] Random ticket generation (1-99) using `ESP.random()`
- [ ] Higher/lower comparison
- [ ] Tie handling (push — no loss, fresh card)
- [ ] Balance system starting at $500
- [ ] Escalating ante ($50/$100/$250/$500)
- [ ] Pot doubling and streak tracking
- [ ] Cash-out: return pot to balance, reset streak
- [ ] Game over detection (balance = $0)
- [ ] Peak balance tracking for scoring
- [x] Create a array of imaginary artsits and a array of imaginary venues use short names

## Persistence

- [ ] Save high score to NVS on game over
- [X] Load and display high score on Start screen
- [ ] Handle first-run (no saved score)

## Audio

- [ ] Map RTTTL songs to game events
- [ ] Round start sound
- [ ] Correct guess jingle (pitch rises with streak)
- [ ] Wrong guess sound
- [ ] Cash out sound
- [ ] Game over melody
- [ ] New high score fanfare
- [ ] Ensure non-blocking playback in game loop

## Polish

- [ ] Randomized event names on ticket cards
- [ ] Color-coded card borders by value range
- [ ] Backlight brightness control or fade effects
- [ ] Smooth screen transitions between views

Draw static UI elements (borders, labels) directly via tft.drawString() / tft.drawRect()
Use small sprites only for the parts that animate (ticket number reveal, balance counter, etc.)
Use tft.fillRect() to clear specific regions before redrawing, rather than clearing the whole screen
Want me to explore your current display code and plan out a sprite-based game view implementation?