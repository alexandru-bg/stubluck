#include "view_manager.h"

static const View *active = nullptr;

void setView(const View *view) {
  active = view;
  if (active && active->enter) {
    active->enter();
  }
}

const View *currentView() {
  return active;
}

void updateView() {
  if (active && active->update) {
    active->update();
  }
}

void handleClick() {
  if (active && active->onClick) {
    active->onClick();
  }
}

void handleDoubleClick() {
  if (active && active->onDoubleClick) {
    active->onDoubleClick();
  }
}

void handleLongPress() {
  if (active && active->onLongPress) {
    active->onLongPress();
  }
}
