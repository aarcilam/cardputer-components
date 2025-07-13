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

  void setPosition(int x, int y) {
    _x = x;
    _y = y;
  }

  void showClickEffect() {
    // Guardar el estado actual
    auto bg = Theme::PRIMARY_COLOR;
    
    // Efecto de click: cambiar color de fondo brevemente
    M5Cardputer.Display.fillRoundRect(_x, _y, _w, _h, 5, Theme::ACCENT_COLOR);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
    M5Cardputer.Display.setCursor(_x + 6, _y + 8);
    M5Cardputer.Display.print(_label);
    
    // Pequeña pausa para el efecto visual
    delay(100);
    
    // Restaurar el color original
    M5Cardputer.Display.fillRoundRect(_x, _y, _w, _h, 5, bg);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
    M5Cardputer.Display.setCursor(_x + 6, _y + 8);
    M5Cardputer.Display.print(_label);
  }

private:
  int _x, _y, _w, _h;
  String _label;
  void (*_callback)() = nullptr;
};
