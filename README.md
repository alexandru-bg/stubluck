# 🎟️ StubLuck

> *A push-your-luck ticket scalping game for the ESP8266 — one button, one screen, and your entire bankroll on the line.*

![Platform](https://img.shields.io/badge/platform-ESP8266-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-teal)
![IDE](https://img.shields.io/badge/IDE-VSCode%20%2B%20PlatformIO-purple)
![Display](https://img.shields.io/badge/display-ST7789%20240×240-orange)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 📖 Overview

StubLuck is an 8-bit–style card game running on a compact ESP8266-powered handheld device. You start with **$500** and a mystery ticket number. Guess whether the next ticket will be **higher or lower**, build streaks to multiply your pot, and know when to cash out before you bust.

It's simple. It's tense. You will lose everything on a 42.

---

## 🎮 Gameplay

A **ticket card** is shown face-up with a value from **1 to 99**. Each round, an ante is automatically deducted from your balance and placed on the table. Guess the next card correctly to double the pot — or lose it all.

### Winning & Losing
- **Correct guess** → pot doubles, streak increments, same card becomes the new reference.
- **Wrong guess** → pot is lost, streak resets, new round begins.
- **Tie (same number)** → push; no money lost, streak continues with a fresh card.
- **Balance hits $0** → Game Over. Your peak balance is recorded as your score.

### The Escalating Pot

| Streak | Pot on the Table |
|--------|-----------------|
| 0 (fresh round) | Ante deducted automatically |
| 1 | Ante × 2 |
| 2 | Ante × 4 |
| 3 | Ante × 8 |
| 4 | Ante × 16 |
| 5+ | Keeps doubling… |

### Escalating Ante

As your balance grows, so does the pressure:

| Balance | Ante |
|---------|------|
| $0 – $999 | $50 |
| $1,000 – $2,499 | $100 |
| $2,500 – $4,999 | $250 |
| $5,000+ | $500 |

### Edge Cases
- **Card value 1** → can only go higher (long press ignored).
- **Card value 99** → can only go lower (short press ignored).
- **Mid-range cards (40–60)** → true 50/50 territory. This is where streaks die.

---

## 🕹️ Controls

StubLuck uses a **single button** (GPIO 4) with three gestures:

| Gesture | Action |
|---------|--------|
| Short press | Guess **HIGHER** ▲ |
| Long press (>400ms) | Guess **LOWER** ▼ |
| Double-click (<300ms between taps) | **Cash out** current streak 💰 |

---

## 📺 Screen Layout

```
┌──────────────────────┐
│  BAL: $1,250         │
│  POT: $200  STREAK:2 │
│                      │
│   ┌──────────────┐   │
│   │~ POLKA FEST ~│   │
│   │              │   │
│   │     42       │   │
│   │              │   │
│   │  ★ ★ ★ ★ ★  │   │
│   └──────────────┘   │
│                      │
│  ▲ HIGH    ▼ LOW     │
└──────────────────────┘
```

Each ticket card (~140×180px) features:
- A **perforated zigzag edge** on the left
- A **large bold number** (1–99) in the center
- A randomized **flavor event name** (e.g. `MONSTER TRUCK BINGO`, `POLKA FEST 2K`, `CAT SHOW FINALS`)
- A **color-coded border** based on value range:

| Range | Color |
|-------|-------|
| 1–25 | 🔴 Red |
| 26–50 | 🟡 Yellow |
| 51–75 | 🟢 Green |
| 76–99 | 🔵 Blue |

---

## 🔊 Sound Design

All audio is handled via RTTTL melodies through a piezoelectric beeper:

| Event | Sound |
|-------|-------|
| Round start | Short two-note *bip-bip* |
| Correct guess | Quick ascending three-note jingle (pitch rises with streak) |
| Wrong guess | Descending *wah-wah-waaah* |
| Cash out | Cha-ching fast ascending arpeggio |
| Game over | Slow sad melody (4–5 notes) |
| New high score | Triumphant fanfare (8+ notes) |

---

## 🖥️ Screens

| Screen | Description |
|--------|-------------|
| **Boot** | Displays the 240×240 boot logo bitmap |
| **Start** | 240×240 bitmap with blinking `START GAME` and last high score |
| **Game** | Live gameplay with ticket card, balance, pot, and streak |
| **End** | 240×240 bitmap with final score and blinking `CONTINUE`; button press returns to Start |

---

## 🔧 Hardware

| Component | Details |
|-----------|---------|
| Microcontroller | ESP8266 |
| Display | 240×240 ST7789 TFT |
| Input | Single push button (GPIO 4) |
| Audio | Piezoelectric beeper (GPIO 4, multiplexed with button) |
| Persistent storage | Non-volatile via Preferences library |

> **Note:** The button and beeper share **GPIO 4** using a multiplexing technique, allowing both input detection and audio output on the same line.

---

## 📦 Dependencies

| Library | Purpose |
|---------|---------|
| [bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) | ST7789 display driver |
| [end2endzone/AnyRtttl](https://github.com/end2endzone/AnyRtttl) | RTTTL melody playback |
| [shymanskyy/Preferences](https://github.com/vshymanskyy/Preferences) | Non-volatile high score storage |
| [mathertel/OneButton] | For Game Control |(https://github.com/mathertel/OneButton)

---

## 🚀 Getting Started

### Prerequisites
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)

### Build & Flash

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-username/StubLuck.git
   cd StubLuck
   ```

2. **Open in VS Code with PlatformIO.**

3. **Configure `TFT_eSPI`** in `User_Setup.h` for the ST7789 240×240 display and your wiring.

4. **Build and upload:**
   ```
   PlatformIO: Upload
   ```

5. **Power on your device** — the boot logo appears, followed by the start screen.

---

## 🏆 Scoring

Your **score is your peak balance** reached during a session — not just streaks. A cautious player who cashes out at streak 2 consistently will outscore a reckless gambler who always pushes for streak 5. High scores are saved to non-volatile storage and displayed on the start screen.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

*Made with one button and a lot of bad luck.*