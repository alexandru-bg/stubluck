#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <Arduino.h>

// View lifecycle callbacks
struct View {
  void (*enter)();       // called once when switching to this view
  void (*update)();      // called every loop iteration
  void (*onClick)();     // short press
  void (*onDoubleClick)(); // double click
  void (*onLongPress)(); // long press (>400ms)
};

void setView(const View *view);
const View *currentView();
void updateView();
void handleClick();
void handleDoubleClick();
void handleLongPress();

#endif
