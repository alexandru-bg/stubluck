# PLAN.md

Implementation plan for StubLuck — the push-your-luck ticket scalping game.

## Current State

Hardware abstraction is in place (pins, TFT config, audio driver with button/buzzer mux). The `loop()` function and display rendering (`showText`) are stubbed. No views exist yet.

## Implementation Phases

### Phase 1: Core Display & Views Scaffold

- Create `src/views/` directory with a view manager pattern
- Implement screen states: Start, Game, End
- Build the ticket card renderer (140x180px card with zigzag edge, number, event name, color-coded border)
- Implement `showText()` in `display.cpp`

### Phase 2: Input System

- Integrate OneButton library for gesture detection
- Wire up short press (HIGHER), long press >400ms (LOWER), double-click (CASH OUT)
- Handle edge cases: card=1 (only higher), card=99 (only lower)

### Phase 3: Game Logic

- Random ticket number generation (1-99)
- Higher/lower comparison with tie handling
- Balance tracking starting at $500
- Escalating ante system ($50/$100/$250/$500 at balance thresholds)
- Pot doubling on correct guesses, streak tracking
- Cash-out mechanic returning pot to balance
- Game over when balance hits $0

### Phase 4: Persistence & Scoring

- Track peak balance as score during session
- Save/load high score via Preferences (NVS)
- Display high score on Start screen

### Phase 5: Audio Integration

- Map RTTTL songs to game events (round start, correct, wrong, cash out, game over, new high score)
- Respect GPIO 4 mux pattern for all playback

### Phase 6: Polish

- Boot logo bitmap (240x240)
- Start screen bitmap with blinking "START GAME" text
- End screen bitmap with blinking "CONTINUE" text
- Color-coded card borders (Red 1-25, Yellow 26-50, Green 51-75, Blue 76-99)
- Randomized flavor event names on tickets
- Streak-based pitch increase on correct guess jingles

## Open Questions

- What view transition pattern to use? (state machine with enum vs function pointers)
- How to handle blinking text? (timer-based toggle in loop vs millis-based flag)

