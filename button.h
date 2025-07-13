#pragma once
#include <M5Cardputer.h>
#include "Theme.h"

class Button {
public:
  Button(int x, int y, int w, int h, const String& label)
    : _x(x), _y(y), _w(w), _h(h), _label(label) {}

  void draw(bool selected = false) {
    auto bg = selected ? Theme::SECONDARY_COLOR : Theme::PRIMARY_COLOR;
    M5Cardputer.Display.fillRoundRect(_x, _y, _w, _h, 5, bg);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
    M5Cardputer.Display.setCursor(_x + 6, _y + 8);
    M5Cardputer.Display.print(_label);
  }

  void onClick(void (*callback)()) {
    _callback = callback;
  }

  void click() {
    if (_callback) _callback();
  }

private:
  int _x, _y, _w, _h;
  String _label;
  void (*_callback)() = nullptr;
};
