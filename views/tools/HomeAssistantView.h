#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/header.h"
#include "../../core/HomeAssistantService.h"
#include "../../core/NetworkService.h"

class HomeAssistantView : public RoutedView {
private:
  String _lastResponse = "";
  bool _showResponse = false;
  bool _isConfigured = false;

public:
  void onEnter() override {
    // Configurar Home Assistant al entrar
    HomeAssistantService& ha = HomeAssistantService::getInstance();
    ha.setBaseUrl("http://homeassistant.local:8123");
    // Por ahora sin token - puedes configurarlo aquí si tienes uno
    // ha.setToken("tu_token_aqui");
    _isConfigured = true;
  }

  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Header header("Home Assistant");
    header.draw();
    
    // Verificar conexión de red
    NetworkService& network = NetworkService::getInstance();
    
    int yPos = 35;
    
    // Estado de la conexión
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("Estado: ");
    M5Cardputer.Display.print(network.isConnected() ? "Conectado" : "Desconectado");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("HA: ");
    M5Cardputer.Display.print(_isConfigured ? "Configurado" : "No configurado");
    
    // Mostrar respuesta si existe
    if (_showResponse && _lastResponse != "") {
      yPos += 20;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Respuesta:");
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      // Mostrar solo los primeros 80 caracteres para que quepa
      String shortResponse = _lastResponse.substring(0, 80);
      if (_lastResponse.length() > 80) {
        shortResponse += "...";
      }
      M5Cardputer.Display.print(shortResponse);
    }
    
    yPos += 25;
    
    // Opciones disponibles
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("1. Obtener Estados");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("2. Encender Luz");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("3. Apagar Luz");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("4. Estado Sensor");
    
    // Instrucciones
    M5Cardputer.Display.setCursor(10, 140);
    M5Cardputer.Display.print("1-4: Acciones | Enter: Volver");
  }
  
  void handleInput(char key) override {
    if (!NetworkService::getInstance().isConnected()) {
      _lastResponse = "ERROR: Sin conexion WiFi";
      _showResponse = true;
      markForRedraw();
      return;
    }

    HomeAssistantService& ha = HomeAssistantService::getInstance();
    
    switch (key) {
      case '1':
        // Obtener todos los estados
        _lastResponse = ha.getAllStates();
        _showResponse = true;
        markForRedraw();
        break;
        
      case '2':
        // Encender una luz (ejemplo genérico)
        {
          String jsonData = "{\"entity_id\": \"light.living_room\"}";
          _lastResponse = ha.callService("light", "turn_on", jsonData);
          _showResponse = true;
          markForRedraw();
        }
        break;
        
      case '3':
        // Apagar una luz (ejemplo genérico)
        {
          String jsonData = "{\"entity_id\": \"light.living_room\"}";
          _lastResponse = ha.callService("light", "turn_off", jsonData);
          _showResponse = true;
          markForRedraw();
        }
        break;
        
      case '4':
        // Obtener estado de un sensor específico
        _lastResponse = ha.getState("sensor.temperature");
        _showResponse = true;
        markForRedraw();
        break;
    }
  }
  
  void onSelect() override {
    goBack();
  }
  
  void onGoBack() override {
    goBack();
  }
}; 