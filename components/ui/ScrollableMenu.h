#pragma once
#include <M5Cardputer.h>
#include "button.h"

class ScrollableMenu {
public:
  ScrollableMenu(int x, int y, int w, int maxVisible = 4) 
    : _x(x), _y(y), _w(w), _maxVisible(maxVisible) {}

  void addButton(const String& label, void (*callback)()) {
    if (_buttons < 20) { // Límite de botones
      _labels[_buttons] = label;
      _callbacks[_buttons] = callback;
      _buttons++;
    }
  }

  void draw() {
    // Limpiar área del menú
    M5Cardputer.Display.fillRect(_x, _y, _w, _maxVisible * (_h + Theme::PADDING), Theme::BACKGROUND_COLOR);
    
    // Calcular el rango de botones a mostrar
    int startIndex = (_selected / _maxVisible) * _maxVisible;
    int endIndex = min(startIndex + _maxVisible, _buttons);
    
    // Dibujar solo los botones visibles
    for (int i = startIndex; i < endIndex; i++) {
      int visibleIndex = i - startIndex;
      int yPos = _y + visibleIndex * (_h + Theme::PADDING);
      
      // Crear botón temporal para dibujar
      Button tempButton(_x, yPos, _w, _h, _labels[i]);
      tempButton.onClick(_callbacks[i]);
      tempButton.draw(i == _selected);
    }
    
    // Mostrar indicador de scroll si hay más páginas
    if (_buttons > _maxVisible) {
      drawScrollIndicator();
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
    // Crear botón temporal para el efecto de click
    int startIndex = (_selected / _maxVisible) * _maxVisible;
    int visibleIndex = _selected - startIndex;
    int yPos = _y + visibleIndex * (_h + Theme::PADDING);
    
    Button tempButton(_x, yPos, _w, _h, _labels[_selected]);
    tempButton.onClick(_callbacks[_selected]);
    tempButton.showClickEffect();
    tempButton.click();
  }

private:
  void drawScrollIndicator() {
    int totalPages = (_buttons + _maxVisible - 1) / _maxVisible;
    int currentPage = _selected / _maxVisible;
    
    // Dibujar indicador de página en la esquina inferior derecha
    int indicatorX = _x + _w - 20;
    int indicatorY = _y + _maxVisible * (_h + Theme::PADDING) - 10;
    
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(indicatorX, indicatorY);
    M5Cardputer.Display.print(currentPage + 1);
    M5Cardputer.Display.print("/");
    M5Cardputer.Display.print(totalPages);
  }

  int _x, _y, _w, _h = 30;
  int _maxVisible;
  String _labels[20]; // Almacenar las etiquetas
  void (*_callbacks[20])() = {nullptr}; // Almacenar los callbacks
  int _buttons = 0;
  int _selected = 0;
}; 