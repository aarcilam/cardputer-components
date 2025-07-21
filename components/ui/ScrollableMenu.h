#pragma once
#include <M5Cardputer.h>
#include "button.h"

class ScrollableMenu {
public:
  ScrollableMenu(int x, int y, int w, int maxVisible = 4) 
    : _x(x), _y(y), _w(w), _maxVisible(maxVisible) {}

  void addButton(const String& label, void (*callback)()) {
    addButton(label, callback, ""); // Sin entity ID por defecto
  }

  void addButton(const String& label, void (*callback)(), const String& entityId) {
    if (_buttons < 20) { // Límite de botones
      _labels[_buttons] = label;
      _callbacks[_buttons] = callback;
      _entityIds[_buttons] = entityId;
      _buttonColors[_buttons] = 0; // Color por defecto (sin color personalizado)
      _buttons++;
    }
  }

  void setButtonStateColor(int index, uint32_t color) {
    if (index >= 0 && index < _buttons) {
      _buttonColors[index] = color;
    }
  }

  void setButtonStateColor(const String& entityId, uint32_t color) {
    for (int i = 0; i < _buttons; i++) {
      if (_entityIds[i] == entityId) {
        _buttonColors[i] = color;
        break;
      }
    }
  }

  void draw() {
    // Limpiar área del menú completamente
    M5Cardputer.Display.fillRect(_x, _y, _w, _maxVisible * (_h + Theme::PADDING), Theme::BACKGROUND_COLOR);
    
    // Calcular el rango de botones a mostrar
    int startIndex = (_selected / _maxVisible) * _maxVisible;
    int endIndex = min(startIndex + _maxVisible, _buttons);
    
    // Dibujar solo los botones visibles en la página actual
    for (int i = startIndex; i < endIndex; i++) {
      int visibleIndex = i - startIndex;
      int yPos = _y + visibleIndex * (_h + Theme::PADDING);
      
      // Crear botón temporal para dibujar
      Button tempButton(_x, yPos, _w, _h, _labels[i]);
      tempButton.onClick(_callbacks[i]);
      
      // Establecer color personalizado si existe
      if (_buttonColors[i] != 0) {
        tempButton.setStateColor(_buttonColors[i]);
      }
      
      tempButton.draw(i == _selected);
    }
    
    // Mostrar indicador de scroll si hay más páginas
    if (_buttons > _maxVisible) {
      drawScrollIndicator();
    }
  }
  
  void drawSelectionOnly() {
    // Limpiar completamente el área del menú antes de redibujar
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
      
      // Establecer color personalizado si existe
      if (_buttonColors[i] != 0) {
        tempButton.setStateColor(_buttonColors[i]);
      }
      
      tempButton.draw(i == _selected);
    }
    
    // Mostrar indicador de scroll si hay más páginas
    if (_buttons > _maxVisible) {
      drawScrollIndicator();
    }
  }

  void selectUp() {
    _selected = (_selected - 1 + _buttons) % _buttons;
    // No llamar draw() aquí, se maneja desde la vista
  }

  void selectDown() {
    _selected = (_selected + 1) % _buttons;
    // No llamar draw() aquí, se maneja desde la vista
  }

  void activateSelected() {
    // Crear botón temporal para el efecto de click
    int startIndex = (_selected / _maxVisible) * _maxVisible;
    int visibleIndex = _selected - startIndex;
    int yPos = _y + visibleIndex * (_h + Theme::PADDING);
    
    Button tempButton(_x, yPos, _w, _h, _labels[_selected]);
    tempButton.onClick(_callbacks[_selected]);
    
    // Establecer color personalizado si existe
    if (_buttonColors[_selected] != 0) {
      tempButton.setStateColor(_buttonColors[_selected]);
    }
    
    tempButton.showClickEffect();
    tempButton.click();
  }
  
  // Obtener la página actual
  int getCurrentPage() const {
    return _selected / _maxVisible;
  }

  // Obtener entity ID de un botón
  String getEntityId(int index) const {
    if (index >= 0 && index < _buttons) {
      return _entityIds[index];
    }
    return "";
  }

private:
  void drawScrollIndicator() {
    int totalPages = (_buttons + _maxVisible - 1) / _maxVisible;
    int currentPage = _selected / _maxVisible;
    
    // Solo dibujar si hay más de una página
    if (totalPages <= 1) {
      return;
    }
    
    // Dibujar indicador de página en la esquina inferior derecha
    int indicatorX = _x + _w - 20;
    int indicatorY = _y + _maxVisible * (_h + Theme::PADDING) - 20;
    
    // Limpiar el área del indicador antes de dibujar
    M5Cardputer.Display.fillRect(indicatorX - 5, indicatorY - 5, 25, 15, Theme::BACKGROUND_COLOR);
    
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
  String _entityIds[20]; // Almacenar los entity IDs para actualizar estados
  uint32_t _buttonColors[20]; // Almacenar colores personalizados por estado
  int _buttons = 0;
  int _selected = 0;
}; 