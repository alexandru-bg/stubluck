#include "boot_view.h"
#include "start_view.h"
#include "display.h"
#include "audio.h"

extern const char *songs[];

static void enter() {
  slideDownImageFS("/boot.bin", 0, 0, 240, 240, 3000);
  playSound(songs[1]);
  delay(2400);
  fadeOut(1000);
  setView(&startView);
}

static void update() {}
static void onClick() {}
static void onDoubleClick() {}
static void onLongPress() {}

const View bootView = { enter, update, onClick, onDoubleClick, onLongPress };
