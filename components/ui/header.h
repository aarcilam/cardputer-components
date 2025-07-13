#pragma once
#include <M5Cardputer.h>
#include <WiFi.h>
#include <SD.h>
#include "../../theme.h"

class Header {
public:
  Header(const String& title) : _title(title) {
    checkWifiConfig();
  }

  void draw() {
    // Fondo del header
    M5Cardputer.Display.fillRect(0, 0, M5Cardputer.Display.width(), 25, Theme::PRIMARY_COLOR);
    
    // Título
    M5Cardputer.Display.setTextSize(Theme::FONT_HEADER);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, 5);
    M5Cardputer.Display.print(_title);
    
    // Indicadores
    drawWifiStatus();
    drawBattery();
  }

private:
  void checkWifiConfig() {
    if(SD.exists("/wificon.txt")) {
      File file = SD.open("/wificon.txt", FILE_READ);
      if(file) {
        String ssid = file.readStringUntil('\n');
        String password = file.readStringUntil('\n');
        ssid.trim();
        password.trim();
        file.close();
        
        WiFi.begin(ssid.c_str(), password.c_str());
        _hasWifiConfig = true;
      }
    }
  }

  void drawWifiStatus() {
    int wifiX = M5Cardputer.Display.width() - 95;
    int wifiY = 5;
    uint32_t wifiColor;

    if(!_hasWifiConfig) {
      wifiColor = 0xFFFFFF; // Blanco
    } else if(WiFi.status() == WL_CONNECTED) {
      wifiColor = 0x00FF00; // Verde
    } else {
      wifiColor = 0xFF0000; // Rojo
    }

    // Dibujar icono WiFi
    M5Cardputer.Display.fillRect(wifiX, wifiY + 8, 3, 4, wifiColor);
    M5Cardputer.Display.fillRect(wifiX + 4, wifiY + 4, 3, 8, wifiColor);
    M5Cardputer.Display.fillRect(wifiX + 8, wifiY, 3, 12, wifiColor);
  }

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
  bool _hasWifiConfig = false;
}; 