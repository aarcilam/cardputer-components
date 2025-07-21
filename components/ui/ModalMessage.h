#pragma once
#include <M5Cardputer.h>
#include "../../theme.h"

enum ModalType {
  MODAL_SUCCESS,
  MODAL_ERROR,
  MODAL_INFO,
  MODAL_WARNING
};

class ModalMessage {
private:
  String _message;
  ModalType _type;
  bool _visible;
  unsigned long _showTime;
  unsigned long _duration;
  int _x, _y, _width, _height;
  
  uint16_t getTypeColor() {
    switch (_type) {
      case MODAL_SUCCESS:
        return TFT_GREEN;
      case MODAL_ERROR:
        return TFT_RED;
      case MODAL_WARNING:
        return TFT_ORANGE;
      case MODAL_INFO:
      default:
        return TFT_BLUE;
    }
  }
  
  String getTypeIcon() {
    switch (_type) {
      case MODAL_SUCCESS:
        return "✓";
      case MODAL_ERROR:
        return "✗";
      case MODAL_WARNING:
        return "⚠";
      case MODAL_INFO:
      default:
        return "ℹ";
    }
  }

public:
  ModalMessage() : _visible(false), _duration(3000) {
    // Configurar dimensiones del modal (centrado en pantalla)
    _width = 200;
    _height = 80;
    _x = (M5Cardputer.Display.width() - _width) / 2;
    _y = (M5Cardputer.Display.height() - _height) / 2;
  }
  
  void show(const String& message, ModalType type = MODAL_SUCCESS, unsigned long duration = 3000) {
    _message = message;
    _type = type;
    _duration = duration;
    _visible = true;
    _showTime = millis();
  }
  
  void showSuccess(const String& message, unsigned long duration = 3000) {
    show(message, MODAL_SUCCESS, duration);
  }
  
  void showError(const String& message, unsigned long duration = 3000) {
    show(message, MODAL_ERROR, duration);
  }
  
  void showInfo(const String& message, unsigned long duration = 3000) {
    show(message, MODAL_INFO, duration);
  }
  
  void showWarning(const String& message, unsigned long duration = 3000) {
    show(message, MODAL_WARNING, duration);
  }
  
  void hide() {
    _visible = false;
  }
  
  bool isVisible() const {
    return _visible;
  }
  
  void update() {
    if (_visible && _duration > 0 && millis() - _showTime > _duration) {
      _visible = false;
    }
  }
  
  void draw() {
    if (!_visible) return;
    
    // Guardar el estado actual del display
    int16_t currentCursor_x = M5Cardputer.Display.getCursorX();
    int16_t currentCursor_y = M5Cardputer.Display.getCursorY();
    
    // Dibujar fondo semi-transparente (simulado con color gris oscuro)
    M5Cardputer.Display.fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), 
                                 M5Cardputer.Display.color565(0, 0, 0));
    
    // Dibujar el modal
    uint16_t modalColor = getTypeColor();
    
    // Borde del modal
    M5Cardputer.Display.fillRect(_x - 2, _y - 2, _width + 4, _height + 4, modalColor);
    
    // Fondo del modal
    M5Cardputer.Display.fillRect(_x, _y, _width, _height, Theme::BACKGROUND_COLOR);
    
    // Configurar texto
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
    
    // Dibujar icono
    String icon = getTypeIcon();
    M5Cardputer.Display.setTextColor(modalColor);
    M5Cardputer.Display.setCursor(_x + 10, _y + 10);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.print(icon);
    
    // Dibujar mensaje
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
    
    // Calcular posición del texto para centrarlo
    int textStartX = _x + 35;
    int textStartY = _y + 15;
    int maxWidth = _width - 45;
    
    // Dividir el mensaje en líneas si es muy largo
    String remainingText = _message;
    int lineHeight = 12;
    int currentLine = 0;
    int maxLines = (_height - 30) / lineHeight;
    
    while (remainingText.length() > 0 && currentLine < maxLines) {
      String line = "";
      int charWidth = 6; // Aproximado para el tamaño de fuente normal
      int maxCharsPerLine = maxWidth / charWidth;
      
      if (remainingText.length() <= maxCharsPerLine) {
        line = remainingText;
        remainingText = "";
      } else {
        // Buscar un espacio para cortar la línea
        int cutPoint = maxCharsPerLine;
        while (cutPoint > 0 && remainingText.charAt(cutPoint) != ' ') {
          cutPoint--;
        }
        if (cutPoint == 0) cutPoint = maxCharsPerLine; // Si no hay espacios, cortar forzadamente
        
        line = remainingText.substring(0, cutPoint);
        remainingText = remainingText.substring(cutPoint);
        if (remainingText.startsWith(" ")) remainingText = remainingText.substring(1);
      }
      
      M5Cardputer.Display.setCursor(textStartX, textStartY + (currentLine * lineHeight));
      M5Cardputer.Display.print(line);
      currentLine++;
    }
    
    // Dibujar instrucción para cerrar (si es manual)
    if (_duration == 0) {
      M5Cardputer.Display.setTextSize(1);
      M5Cardputer.Display.setTextColor(TFT_LIGHTGREY);
      M5Cardputer.Display.setCursor(_x + 10, _y + _height - 15);
      M5Cardputer.Display.print("Enter para cerrar");
    }
    
    // Restaurar el estado del display
    M5Cardputer.Display.setCursor(currentCursor_x, currentCursor_y);
  }
  
  bool handleInput(char key) {
    if (!_visible) return false;
    
    // Si es un modal manual (sin duración), permitir cerrarlo con Enter
    if (_duration == 0 && (key == '\n' || key == '\r')) {
      hide();
      return true;
    }
    
    // El modal consume todas las entradas mientras está visible
    return true;
  }
}; 