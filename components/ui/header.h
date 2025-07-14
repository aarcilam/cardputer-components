#pragma once
#include <M5Cardputer.h>
#include <WiFi.h>
#include "../../core/SDCardService.h"
#include "../../theme.h"

class Header {
public:
  Header(const String& title) : _title(title) {
    checkWifiConfig();
  }

  void draw() {
    // Actualizar conexión WiFi
    updateWifiConnection();
    
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

  void retryWifiConnection() {
    // Forzar reconexión WiFi
    if (_hasWifiConfig && !_sdError) {
      WiFi.disconnect();
      delay(1000);
      
      String ssid, password;
      SDCardService& sd = SDCardService::getInstance();
      if (sd.readFileLines("/wificon.txt", ssid, password)) {
        WiFi.begin(ssid.c_str(), password.c_str());
      }
    }
  }

  void updateWifiConnection() {
    static unsigned long lastRetry = 0;
    static int retryCount = 0;
    const unsigned long retryInterval = 10000; // 10 segundos
    
    // Si no hay configuración WiFi, no hacer nada
    if (!_hasWifiConfig || _sdError) {
      return;
    }
    
    // Si está conectado, resetear contador de intentos
    if (WiFi.status() == WL_CONNECTED) {
      retryCount = 0;
      return;
    }
    
    // Intentar reconectar cada 10 segundos
    if (millis() - lastRetry > retryInterval) {
      lastRetry = millis();
      retryCount++;
      
      // Solo intentar hasta 5 veces para evitar bucle infinito
      if (retryCount <= 5) {
        // Solo intentar reconectar si es posible
        if (canConnectWiFi()) {
          WiFi.disconnect();
          delay(1000);
          // Recargar configuración y reconectar
          String ssid, password;
          SDCardService& sd = SDCardService::getInstance();
          if (sd.readFileLines("/wificon.txt", ssid, password)) {
            WiFi.begin(ssid.c_str(), password.c_str());
          }
        }
      }
    }
  }
  
  // Helper para verificar si WiFi está disponible para conectar
  bool canConnectWiFi() {
    wl_status_t status = WiFi.status();
    // Solo conectar si no está conectado
    return status != WL_CONNECTED;
  }

private:
  void checkWifiConfig() {
    SDCardService& sd = SDCardService::getInstance();
    
    if(!sd.isInitialized()) {
      _sdError = true;
      return;
    }
    
    if(sd.exists("/wificon.txt")) {
      String ssid, password;
      if(sd.readFileLines("/wificon.txt", ssid, password)) {
        // Solo cargar configuración, no iniciar conexión automáticamente
        _hasWifiConfig = true;
        _sdError = false;
        
        // Solo iniciar conexión si es posible
        if (canConnectWiFi()) {
          WiFi.begin(ssid.c_str(), password.c_str());
        }
      } else {
        _sdError = true;
      }
    } else {
      _sdError = true;
    }
  }

  void drawWifiStatus() {
    int wifiX = M5Cardputer.Display.width() - 85;
    int wifiY = 5;
    uint32_t wifiColor;
    static int blinkState = 0;
    static unsigned long lastBlink = 0;

    if(_sdError) {
      wifiColor = 0xFFFFFF; // Blanco - Error SD
    } else if(!_hasWifiConfig) {
      wifiColor = 0xFF8C00; // Naranja - Sin configuración
    } else if(WiFi.status() == WL_CONNECTED) {
      wifiColor = 0x00FF00; // Verde - Conectado
    } else {
      // Rojo - Desconectado, con efecto de parpadeo
      if (millis() - lastBlink > 500) {
        blinkState = !blinkState;
        lastBlink = millis();
      }
      wifiColor = blinkState ? 0xFF0000 : 0x800000; // Rojo parpadeante
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
  bool _sdError = false;
}; 