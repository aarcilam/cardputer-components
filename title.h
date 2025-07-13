#pragma once
#include <M5Cardputer.h>
#include "Theme.h"

class Title {
public:
  Title(const String& text) : _text(text) {}

  void draw() {
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_TITLE);
    M5Cardputer.Display.setCursor(Theme::PADDING, Theme::PADDING);
    M5Cardputer.Display.print(_text);
  }

private:
  String _text;
};
