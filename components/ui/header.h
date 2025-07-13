#pragma once
#include <M5Cardputer.h>
#include "../../theme.h"

class Header {
public:
  Header(const String& title) : _title(title) {}

  void draw() {
    // Fondo del header
    M5Cardputer.Display.fillRect(0, 0, M5Cardputer.Display.width(), 25, Theme::PRIMARY_COLOR);
    
    // Título
    M5Cardputer.Display.setTextSize(Theme::FONT_TITLE);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, 5);
    M5Cardputer.Display.print(_title);
    
    // Indicador de batería
    drawBattery();
  }

private:
  void drawBattery() {
    int batteryX = M5Cardputer.Display.width() - 35;
    int batteryY = 5;
    int batteryWidth = 25;
    int batteryHeight = 12;
    
    // Obtener nivel de batería (0-100)
    int batteryLevel = M5Cardputer.Power.getBatteryLevel();
    
    // Color de la batería según el nivel
    uint32_t batteryColor;
    if (batteryLevel > 50) {
      batteryColor = 0x00FF00; // Verde
    } else if (batteryLevel > 20) {
      batteryColor = 0xFFFF00; // Amarillo
    } else {
      batteryColor = 0xFF0000; // Rojo
    }
    
    // Dibujar contorno de la batería
    M5Cardputer.Display.drawRect(batteryX, batteryY, batteryWidth, batteryHeight, Theme::TEXT_COLOR);
    
    // Dibujar el terminal positivo de la batería
    M5Cardputer.Display.fillRect(batteryX + batteryWidth, batteryY + 3, 2, 6, Theme::TEXT_COLOR);
    
    // Dibujar el nivel de batería
    int fillWidth = (batteryLevel * (batteryWidth - 2)) / 100;
    if (fillWidth > 0) {
      M5Cardputer.Display.fillRect(batteryX + 1, batteryY + 1, fillWidth, batteryHeight - 2, batteryColor);
    }
    
    // Mostrar porcentaje de batería
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(batteryX - 25, batteryY + 2);
    M5Cardputer.Display.print(batteryLevel);
    M5Cardputer.Display.print("%");
  }

  String _title;
}; 