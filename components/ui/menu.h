#pragma once
#include <M5Cardputer.h>
#include "button.h"

class Menu {
public:
  Menu(int x, int y, int w) : _x(x), _y(y), _w(w) {}

  void addButton(const String& label, void (*callback)()) {
    _items[_buttons] = new Button(_x, _y + _buttons * (_h + Theme::PADDING), _w, _h, label);
    _items[_buttons]->onClick(callback);
    _buttons++;
  }

  void draw() {
    for (int i = 0; i < _buttons; i++) {
      _items[i]->draw(i == _selected);
    }
  }

  void selectUp() {
    _selected = (_selected - 1 + _buttons) % _buttons;
    draw();
  }

  void selectDown() {
    _selected = (_selected + 1) % _buttons;
    draw();
  }

  void activateSelected() {
    _items[_selected]->showClickEffect();
    _items[_selected]->click();
  }

private:
  int _x, _y, _w, _h = 30;
  Button* _items[10]; // punteros a botones
  int _buttons = 0;
  int _selected = 0;
};
